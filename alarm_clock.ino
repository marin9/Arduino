#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>

#define LCD_RS 7
#define LCD_EN 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12
#define LCD_A 3
#define BZ 6

#define BTN_DELAY 200
#define BTN_NOP 0
#define BTN_A 1
#define BTN_B 2
#define BTN_M 3
#define BTN_P 4

#define AL 0
#define OK 1
#define UP_DOWN 2
#define LEFT_RIGHT 3
byte ch[4][8]={{
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100
},{
  B00000,
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
},{
  B00100,
  B01110,
  B11111,
  B00000,
  B11111,
  B01110,
  B00100,
},{
  B00000,
  B00000,
  B00000,
  B01010,
  B11011,
  B01010,
  B00000,
}};

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
char LIGHT_TIME[2]={23, 7};
const unsigned char LIGHT_MODS[3]={1, 30, 200};
int light_mod=1;
int light_count=0;

char soundOn=0;
char soundType=0;

const char WEEKDAYS[7][4]={"PON", "UTO", "SRI", "CET", "PET", "SUB", "NED"};
char hours, minutes;
char weekday, day, month;
int year;
char alarms[3][3]; //en, hrs, min
char alarmFlags[3];

#define TIME 0
#define ALARM_1 1
#define ALARM_2 2
#define ALARM_3 3
#define TIME_MENU 4
#define SOUND_SELECT 5
char stanje=TIME;
char selected; //0, 1
int select; //n

char last_btn, last_btn_count=0;



void setup() {
  pinMode(LCD_A, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(BZ, OUTPUT);
  
  Wire.begin(); 
  lcd.begin(16, 2);
  lcd.createChar(0, ch[0]);
  lcd.createChar(1, ch[1]);
  lcd.createChar(2, ch[2]);
  lcd.createChar(3, ch[3]);
  analogWrite(LCD_A, 200);
}


void loop(void){
  char btn=getButton();
  changeLightMod(btn);
 
  if(stanje==TIME) getTimeAction(btn);
  else if(stanje==ALARM_1) getAlarmMenuAction(btn, ALARM_1);
  else if(stanje==ALARM_2) getAlarmMenuAction(btn, ALARM_2);
  else if(stanje==ALARM_3) getAlarmMenuAction(btn, ALARM_3);
  else if(stanje==TIME_MENU) getTimeMenuAction(btn);
  else if(stanje==SOUND_SELECT) getSoundSelectAction(btn);

  ++light_count;
  if(light_count==700){
      analogWrite(LCD_A, LIGHT_MODS[light_mod]);
  }

  if(soundOn){
    //TODO play sound if alarm on
  }
  //TODO add week day settings
} 

void getTimeAction(char btn){  
  if(btn!=BTN_NOP){
    lightOn();
    lcd.clear();
  }else{
    getTime();
    printTime();  
  }
  if(btn==BTN_A){     
      stanje=ALARM_1;
      getAlarmDataEEPROM(stanje-1);
      select=0; 
      selected=0;   
  }else if(btn==BTN_B){
      stanje=TIME_MENU;
      select=0; 
      selected=0;  
  }else if(btn==BTN_M){
      stanje=SOUND_SELECT;
      select=soundType=EEPROM.read(9); 
      selected=0;  
  }
}

void getAlarmMenuAction(char btn, char n){  
  if(btn!=BTN_NOP){
    lightOn();
    lcd.clear();
  }else{
    printAlarmMenu(stanje-1);
  }
  if(btn==BTN_A){ 
      if(stanje==ALARM_1){
        storeAlarmDataEEPROM(stanje-1); 
        stanje=ALARM_2;     
        getAlarmDataEEPROM(stanje-1);
      }else if(stanje==ALARM_2){
        storeAlarmDataEEPROM(stanje-1);
        stanje=ALARM_3;       
        getAlarmDataEEPROM(stanje-1);
      }else if(stanje==ALARM_3){
        storeAlarmDataEEPROM(stanje-1);
        stanje=TIME;     
      }
  }else if(btn==BTN_B){
      selected=!selected;   
  }else if(btn==BTN_M){
      if(selected){
          if(select==0) alarms[stanje-1][0]=!alarms[stanje-1][0];
          else if(select==1){
             alarms[stanje-1][1]=alarms[stanje-1][1]-1;
             if(alarms[stanje-1][1]<0) alarms[stanje-1][1]=23;
          }else if(select==2){
              alarms[stanje-1][2]=alarms[stanje-1][2]-1;
              if(alarms[stanje-1][2]<0) alarms[stanje-1][2]=59;
          }         
      }else{
          select=select-1;
          if(select<0) select=2;
      }   
  }else if(btn==BTN_P){
      if(selected){
         if(select==0) alarms[stanje-1][0]=!alarms[stanje-1][0];
          else if(select==1) alarms[stanje-1][1]=(alarms[stanje-1][1]+1)%24;
          else if(select==2) alarms[stanje-1][2]=(alarms[stanje-1][2]+1)%60;
      }else{
          select=(select+1)%3;
      } 
  }
}

void getTimeMenuAction(char btn){  
  if(btn!=BTN_NOP){
    lightOn();
    lcd.clear();
  }else{
    printTimeMenu();
  }
  if(btn==BTN_A){
     stanje=TIME;
     setNewTime();
  }else if(btn==BTN_B){
     selected=!selected;
  }else if(btn==BTN_M){
     if(selected){
          if(select==0){
              hours=hours-1;
              if(hours<0) hours=23;
          }else if(select==1){
              minutes=minutes-1;
              if(minutes<0) minutes=59;
          }else if(select==2){
              day=day-1;
              if(day<1) day=31;
          }else if(select==3){
              month=month-1;
              if(month<1) month=12;
          }else if(select==4){
              year=year-1;
              if(year<2018) year=2100; 
          }
      }else{
          select=select-1;
          if(select<0) select=4;
      } 
  }else if(btn==BTN_P){
      if(selected){
          if(select==0) hours=(hours+1)%24;
          else if(select==1) minutes=(minutes+1)%60;
          else if(select==2) day=1+day%31;
          else if(select==3) month=1+month%12;
          else if(select==4) year=1+year%2100;          
      }else{
          select=(select+1)%5;
      } 
  }
}

void getSoundSelectAction(char btn){
  if(btn!=BTN_NOP){
    lightOn();
    lcd.clear();
  }else{
    printSoundMenu();
  }
  if(btn==BTN_A){
     stanje=TIME;
     soundType=select;
     EEPROM.write(9, select);
  }else if(btn==BTN_B){
     if(select==0) playBeep();
     else if(select==1) playAlarm();
  }else if(btn==BTN_M){
     select=select-1;
     if(select<0) select=1;      
  }else if(btn==BTN_P){
      select=(select+1)%2;    
  }  
}

void setNewTime(){
  //TODO


  
  
}

void getTime(){
  //TODO




  
}

void getAlarmDataEEPROM(int n){
  alarms[n][0]=EEPROM.read(n*3+0);
  alarms[n][1]=EEPROM.read(n*3+1);
  alarms[n][2]=EEPROM.read(n*3+2);
}

void storeAlarmDataEEPROM(int n){
  EEPROM.write(n*3+0, alarms[n][0]);
  EEPROM.write(n*3+1, alarms[n][1]);
  EEPROM.write(n*3+2, alarms[n][2]);
}

void getSoundDataEEPROM(){
  soundType=EEPROM.read(9);
}

void lightOn(){
  analogWrite(LCD_A, 200);
  light_count=0;
}

void printTime(){
  lcd.setCursor(5, 0);
  lcd.print(hours, DEC);
  lcd.print(':');
  lcd.print(minutes, DEC);
  lcd.setCursor(13, 0);
  if(alarms[0][0]) lcd.write(byte(0));
  else lcd.print(' ');
  if(alarms[1][0]) lcd.write(byte(0));
  else lcd.print(' ');
  if(alarms[2][0]) lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.print(WEEKDAYS[(int)weekday]);
  lcd.setCursor(5, 1);
  lcd.print(day, DEC);
  lcd.print('.');
  lcd.print(month, DEC);
  lcd.print('.');
  lcd.print(year, DEC);
  lcd.print('.');  
}

void printAlarmMenu(int n){
  lcd.setCursor(0, 0);
  lcd.print("Alarm ");
  lcd.print(n+1, DEC);
  lcd.print(' ');
  lcd.write(byte(AL));
  if(alarms[n][0]) lcd.write(byte(OK));
  else lcd.print('-');
  lcd.setCursor(11, 0);
  lcd.print(alarms[n][1], DEC);
  lcd.print(':');
  lcd.setCursor(14, 0);
  lcd.print(alarms[n][2], DEC);

  char c_position=0;
  if(select==0) c_position=9;
  else if(select==1) c_position=11;
  else if(select==2) c_position=14;
  lcd.setCursor(c_position, 1);
  
  if(selected){
      lcd.write(byte(UP_DOWN));
  }else{
      lcd.write(byte(LEFT_RIGHT));
  }
}

void printTimeMenu(){
  lcd.setCursor(0, 0);
  lcd.print(hours, DEC);
  lcd.setCursor(2, 0);
  lcd.print(':');
  lcd.print(minutes, DEC);
  lcd.setCursor(6, 0);
  lcd.print(day, DEC);
  lcd.print('.');
  lcd.setCursor(9, 0);
  lcd.print(month, DEC);
  lcd.print('.');
  lcd.setCursor(12, 0);
  lcd.print(year, DEC);

  char c_position=0;
  if(select==0) c_position=0;
  else if(select==1) c_position=3;
  else if(select==2) c_position=6;
  else if(select==3) c_position=9;
  else if(select==4) c_position=12;
  lcd.setCursor(c_position, 1);
  
  if(selected){
      lcd.write(byte(UP_DOWN));
  }else{
      lcd.write(byte(LEFT_RIGHT));
  }
}

void printSoundMenu(){
  lcd.setCursor(0, 0);
  lcd.print("Sound ");
  lcd.print(soundType, DEC);
  lcd.setCursor(6, 1);
  lcd.write(byte(UP_DOWN));
}

char getButton(){
  digitalWrite(13, HIGH);
  digitalWrite(A0, LOW);
  if(digitalRead(A2)){
    delay(BTN_DELAY);
    return BTN_B;
  }else if(digitalRead(A3)){
    delay(BTN_DELAY);
    return BTN_A;
  }
  digitalWrite(13, LOW);
  digitalWrite(A0, HIGH);
  if(digitalRead(A2)){
    delay(BTN_DELAY);
    return BTN_P;
  }else if(digitalRead(A3)){
    delay(BTN_DELAY);
    return BTN_M;
  }
  return BTN_NOP;
}

void changeLightMod(char btn){
  if(btn==last_btn && btn!=BTN_NOP) ++last_btn_count;
  else{
      last_btn=btn;
      last_btn_count=0;
  }
  
  if(last_btn_count==10){
    last_btn_count=0;
    light_mod=1-light_mod;
    lcd.setCursor(0, 0);
    if(light_mod) lcd.print("Day");
    else lcd.print("Night");
    delay(1000);
  }
}

void playBeep(){
  int i, d=179;
  for(i=0;i<50;++i){
    digitalWrite(BZ, HIGH);
    delayMicroseconds(d);
    digitalWrite(BZ, LOW);
    delayMicroseconds(d); 
  }
}

void playAlarm(){
  int a, i, d=300;
  for(a=0;a<12;++a){
    for(i=0;i<50;++i){
      digitalWrite(BZ, HIGH);
      delayMicroseconds(d);
      digitalWrite(BZ, LOW);
      delayMicroseconds(d); 
    }
    delayMicroseconds(200);
  }
}


