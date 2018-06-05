#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>

#define LCD_RS  7
#define LCD_EN  8
#define LCD_D4  9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12
#define LCD_A   3
#define BZ      6

#define BTN_DELAY 200
#define BTN_NOP   0
#define BTN_HOME  1
#define BTN_SET   2
#define BTN_MINUS 3
#define BTN_PLUS  4

#define AL_ON   0
#define AL_OFF  1
#define UP_DOWN 2

byte ch[3][8]={{
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000
},{
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100
},{
  B00100,
  B01110,
  B11111,
  B00000,
  B11111,
  B01110,
  B00100,
}};

#define LIGHT_TIME 150
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
const char WEEKDAYS[8][4]={"---", "NED", "PON", "UTO", "SRI", "CET", "PET", "SUB"};
const unsigned char LIGHT_MODS[3]={1, 30, 200};
int light_mod=1;
int light_on;
int light_count;

char soundType=0;

char last_btn, last_btn_count=0;

char seconds;
char hours, minutes, weekday;
char day, month, year;
char alarm[3];
char alarmOffsetMin;
char changed;

#define TIME_CURRENT  0
#define ALARM_CHANGE  1
#define TIME_CHANGE   3
#define SOUND_CHANGE  4
#define ALARM_RING    5
char screen=TIME_CURRENT;
int select;



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

  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.endTransmission();  
  Wire.requestFrom(0x68, 1);  
  while(!Wire.available());
  unsigned char reg=Wire.read();
  reg=reg&0x7f;
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(reg);
  Wire.endTransmission();  

  getSettingsEEPROM();
  lightOn();
}


void loop(void){   
  char btn=getButton();
  
  if(screen==TIME_CURRENT){
    SCREEN_Time();
    GetAction(btn);
  }else if(screen==ALARM_CHANGE){
    SCREEN_Alarm();
    GetAction(btn);
  }else if(screen==TIME_CHANGE){
    SCREEN_SetTime();
    GetAction(btn);
  }else if(screen==SOUND_CHANGE){
    SCREEN_Sound();
    GetAction(btn);
  }else if(screen==ALARM_RING){
    SCREEN_Time();
    lightOn();
    if(soundType==0) playBeep();
    else if(soundType==1) playAlarm();
    delay(100);
    GetAction(btn);  
  }

  changeLightMod(btn);

  //Ring alarm
  if(alarm[0]){
    char t_min=(alarm[2]+alarmOffsetMin)%60;
    char t_hrs=alarm[1]+((alarm[2]+alarmOffsetMin)/60);
    if(hours==t_hrs && minutes==t_min && seconds==0){
        screen=ALARM_RING;  
    }
  }

  delay(50);
} 



void GetAction(char btn){
  //Current time
  if(screen==TIME_CURRENT){
    select=0;
    if(btn==BTN_HOME){
      changed=0;
      screen=ALARM_CHANGE;
      getSettingsEEPROM();
    }else if(btn==BTN_SET){
      changed=0;
      screen=TIME_CHANGE;      
    }else if(btn==BTN_MINUS){
      screen=SOUND_CHANGE;
      getSettingsEEPROM();
    }
    
  //Alarm settings
  }else if(screen==ALARM_CHANGE){
    if(btn==BTN_HOME){
      if(changed){
          alarmOffsetMin=0;
          storeSettingsEEPROM();
      }     
      screen=TIME_CURRENT;
    }else if(btn==BTN_SET){
      ++select;
      if(select==3) select=0;
    }else if(btn==BTN_MINUS){
      changed=1;
      --alarm[select];
      if(select==0 && alarm[select]<0) alarm[select]=1;
      else if(select==1 && alarm[select]<0) alarm[select]=23;
      else if(select==2 && alarm[select]<0) alarm[select]=59;
    }else if(btn==BTN_PLUS){
      changed=1;
      ++alarm[select];
      if(select==0 && alarm[select]>1) alarm[select]=0;
      else if(select==1 && alarm[select]>23) alarm[select]=0;
      else if(select==2 && alarm[select]>59) alarm[select]=0;
    }
    
  //Time settings
  }else if(screen==TIME_CHANGE){
      if(btn==BTN_HOME){
          if(changed) setNewTime();
          screen=TIME_CURRENT;
      }else if(btn==BTN_SET){
          ++select;
          if(select==6) select=0;
      }else if(btn==BTN_MINUS){
          changed=1;
          if(select==0){
            --hours;
            if(hours<0) hours=23;
          }else if(select==1){
            --minutes;
            if(minutes<0) minutes=59;
          }else if(select==2){
            --weekday;
            if(weekday<1) weekday=7;
          }else if(select==3){
            --day;
            if(day<1) day=31;
          }else if(select==4){
            --month;
            if(month<1) month=12;
          }else if(select==5){
            --year;
            if(year<0) year=99; 
          }
        
      }else if(btn==BTN_PLUS){
          changed=1;
          if(select==0){
            ++hours;
            if(hours>23) hours=0;
          }else if(select==1){
            ++minutes;
            if(minutes>59) minutes=0;
          }else if(select==2){
            ++weekday;
            if(weekday>7) weekday=1;
          }else if(select==3){
            ++day;
            if(day>31) day=1;
          }else if(select==4){
            ++month;
            if(month>12) month=1;
          }else if(select==5){
            ++year;
            if(year>99) year=0; 
          }
      }

  //Sound settings
  }else if(screen==SOUND_CHANGE){
     if(btn==BTN_HOME){
        storeSettingsEEPROM();
        screen=TIME_CURRENT;
     }else if(btn==BTN_SET){
        if(soundType==0) playBeep();
        else if(soundType==1) playAlarm();
     }else if(btn==BTN_MINUS){
        --soundType;
        if(soundType<0) soundType=1;
     }else if(btn==BTN_PLUS){
        ++soundType;
        if(soundType>1) soundType=0;
     }
     
   //Alarm ring
  }else if(screen==ALARM_RING){
    if(btn==BTN_HOME || btn==BTN_SET || btn==BTN_MINUS){
        screen=TIME_CURRENT;
        alarmOffsetMin=0;
      }else if(btn==BTN_PLUS){
        alarmOffsetMin=alarmOffsetMin+5; 
        screen=TIME_CURRENT;
      }
  }
}

void SCREEN_Time(){
  getCurrentTime();
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print(hours, DEC);
  lcd.print(':');
  lcd.print(minutes/10, DEC);
  lcd.print(minutes%10, DEC);
  lcd.setCursor(15, 0);
  if(alarm[0]) lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print(WEEKDAYS[(int)weekday]);
  if(light_count<LIGHT_TIME){
    lcd.setCursor(5, 1);
    lcd.print(day, DEC);
    lcd.print('.');
    lcd.print(month, DEC);
    lcd.print('.');
    lcd.print(20, DEC);
    lcd.print(year, DEC);
    lcd.print('.');  
  }
  if(alarmOffsetMin!=0){
    lcd.setCursor(13, 0);
    lcd.print("+5");
  }
}

void SCREEN_Alarm(){
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write(byte(alarm[0]));
  lcd.setCursor(7, 0);
  lcd.print(alarm[1], DEC);
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.setCursor(12, 0);
  lcd.print(alarm[2], DEC);
  if(select==0) lcd.setCursor(4, 1);
  else if(select==1) lcd.setCursor(7, 1);
  else if(select==2) lcd.setCursor(12, 1);
  lcd.write(byte(2));
}

void SCREEN_SetTime(){
  lcd.clear();
  lcd.print(hours, DEC);
  lcd.setCursor(2, 0);
  lcd.print(":");
  lcd.print(minutes, DEC);
  lcd.setCursor(4, 1);
  lcd.print(WEEKDAYS[(int)weekday]);
  lcd.setCursor(6, 0);
  lcd.print(day, DEC);
  lcd.setCursor(8, 0);
  lcd.print(".");
  lcd.print(month, DEC);
  lcd.setCursor(10, 0);
  lcd.print(".20");
  lcd.print(year, DEC);

  if(select==0) lcd.setCursor(0, 1);
  else if(select==1) lcd.setCursor(3, 1);
  else if(select==2) lcd.setCursor(5, 0);
  else if(select==3) lcd.setCursor(7, 1);
  else if(select==4) lcd.setCursor(9, 1);
  else if(select==5) lcd.setCursor(12, 1);
  lcd.write(byte(2));
}

void SCREEN_Sound(){
  lcd.clear();
  lcd.print("Sound: ");
  lcd.print(soundType, DEC);
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
}

void getCurrentTime(){
  int i=0;
  unsigned char regs[7];
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.endTransmission();
   
  Wire.requestFrom(0x68, 7);  
  while(Wire.available()){
    regs[i++]=Wire.read();
  }

  seconds=(regs[0]&0x0f)+10*((regs[0]&0xf0)>>4);
  minutes=(regs[1]&0x0f)+10*((regs[1]&0xf0)>>4);
  hours=10*((regs[2]&0x30)>>4)+(regs[2]&0x0F);
  weekday=regs[3];
  day=10*((regs[4]&0x70)>>4)+(regs[4]&0x0F);
  month=10*((regs[5]&0x10)>>4)+(regs[5]&0x0F);    
  year=10*((regs[6]&0xf0)>>4)+(regs[6]&0x0F);
}

void setNewTime(){
  unsigned char reg0=0;
  unsigned char reg1=(minutes%10)+((minutes/10)<<4);
  unsigned char reg2=(hours%10)+((hours/10)<<4);
  unsigned char reg3=weekday;
  unsigned char reg4=(day%10)+((day/10)<<4);
  unsigned char reg5=(month%10)+((month/10)<<4);
  unsigned char reg6=(year%10)+((year/10)<<4);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(reg0);
  Wire.write(reg1); 
  Wire.write(reg2);
  Wire.write(reg3);
  Wire.write(reg4);
  Wire.write(reg5);
  Wire.write(reg6);
  Wire.endTransmission();
}

void getSettingsEEPROM(){
  alarm[0]=EEPROM.read(0);
  alarm[1]=EEPROM.read(1);
  alarm[2]=EEPROM.read(2);
  soundType=EEPROM.read(3);
}

void storeSettingsEEPROM(){
  EEPROM.write(0, alarm[0]);
  EEPROM.write(1, alarm[1]);
  EEPROM.write(2, alarm[2]);
  EEPROM.write(3, alarm[3]);
}


char getButton(){
  digitalWrite(13, HIGH);
  digitalWrite(A0, LOW);
  if(digitalRead(A2)){
    delay(BTN_DELAY);
    return BTN_SET;
  }else if(digitalRead(A3)){
    delay(BTN_DELAY);
    return BTN_HOME;
  }
  digitalWrite(13, LOW);
  digitalWrite(A0, HIGH);
  if(digitalRead(A2)){
    delay(BTN_DELAY);
    return BTN_PLUS;
  }else if(digitalRead(A3)){
    delay(BTN_DELAY);
    return BTN_MINUS;
  }
  return BTN_NOP;
}

void changeLightMod(char btn){
  if(btn==BTN_NOP){
    if(hours==0 && minutes==0) light_mod=0;
    else if(hours==7 && minutes==0) light_mod=1;
  }
  
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
  if(btn!=BTN_NOP) lightOn();
  if(light_on) ++light_count;
  if(light_count==LIGHT_TIME){
      analogWrite(LCD_A, LIGHT_MODS[light_mod]);
      light_on=0;
  }
}

void lightOn(){
  analogWrite(LCD_A, 200);
  light_count=0;
  light_on=1;
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
//Marin Bralic
