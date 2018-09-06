#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define BZ      3
#define BTN_S   6   
#define BTN_M   9
#define BTN_P   12
#define SW_A    2

#define ST_ALARM  0
#define ST_IDLE   1
#define ST_HRS    2
#define ST_MIN    3
#define ST_DAY    4
#define ST_MON    5
#define ST_YR     6
#define ST_WDAY   7
#define ST_ALHRS  8
#define ST_ALMIN  9

char hrs, min, sec;
char wday, day, mon, y;
char al_hrs, al_min, al_on;
char state, next_state; 
char btn_old, btn_new, btn;
unsigned char regs[7];
Adafruit_SSD1306 oled(-1);


void setup() {
  pinMode(BZ, OUTPUT);
  pinMode(SW_A, INPUT_PULLUP);
  pinMode(BTN_S, INPUT_PULLUP);
  pinMode(BTN_M, INPUT_PULLUP);
  pinMode(BTN_P, INPUT_PULLUP);
  digitalWrite(BZ, LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.dim(1);
  oled.display();

  Wire.begin();  
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.endTransmission();  
  Wire.requestFrom(0x68, 1);  
  while(!Wire.available());
  regs[0]=Wire.read();
  regs[0]&=0x7f;
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(regs[0]);
  Wire.endTransmission();

  state=ST_IDLE;
  btn_old=-1;
  btn_new=-1;
  eeprom_load();  
}

void loop() {
  btn_old=btn_new;
  btn_new=buttons_get();
  if(btn_old==btn_new) btn=-1;
  else btn=btn_new;
  next_state=state;
      
  switch(state){
    case ST_IDLE:   
      rtc_gettime(); 
      if(btn==BTN_S)  next_state=ST_ALHRS;      
      else if(btn==BTN_M) next_state=ST_HRS;   
      else if(btn==BTN_P) next_state=ST_DAY;     
      
      if(al_hrs==hrs && al_min==min && sec==0){
        next_state=ST_ALARM;
      }
      break; 
    case ST_ALARM: 
      if(btn!=-1 || !al_on){
        next_state=ST_IDLE;
      }else{
        alarm_play();
      }  
      break;  
    case ST_HRS:
      if(btn==BTN_S){
        next_state=ST_MIN;      
      }else if(btn==BTN_M){
        --hrs;   
        if(hrs<0) hrs=23;
      }else if(btn==BTN_P){
        ++hrs;
        if(hrs>23) hrs=0;
      }
      break;
    case ST_MIN:
      if(btn==BTN_S){
        next_state=ST_IDLE;
        rtc_settime();
      }else if(btn==BTN_M){
        --min;   
        if(min<0) min=59;
      }else if(btn==BTN_P){
        ++min;
        if(min>59) min=0;
      }
      break;  
    case ST_ALHRS:
      if(btn==BTN_S){
        next_state=ST_ALMIN;      
      }else if(btn==BTN_M){
        --al_hrs;   
        if(al_hrs<0) al_hrs=23;
      }else if(btn==BTN_P){
        ++al_hrs;
        if(al_hrs>23) al_hrs=0;
      }
      break; 
    case ST_ALMIN:
      if(btn==BTN_S){
        next_state=ST_IDLE;
        eeprom_save();
      }else if(btn==BTN_M){
        --al_min;   
        if(al_min<0) al_min=59;
      }else if(btn==BTN_P){
        ++al_min;
        if(al_min>59) al_min=0;
      }
      break;
    case ST_DAY:
      if(btn==BTN_S){
        next_state=ST_MON;
      }else if(btn==BTN_M){
        --day;   
        if(day<1) day=31;
      }else if(btn==BTN_P){
        ++day;
        if(day>31) day=1;
      }
      break;
    case ST_MON:
      if(btn==BTN_S){
        next_state=ST_YR;
      }else if(btn==BTN_M){
        --mon;   
        if(mon<1) mon=12;
      }else if(btn==BTN_P){
        ++mon;
        if(mon>12) mon=1;
      }
      break;
    case ST_YR:
      if(btn==BTN_S){
        next_state=ST_WDAY;
      }else if(btn==BTN_M){
        --y;   
        if(y<0) y=99;
      }else if(btn==BTN_P){
        ++y;
        if(y>99) y=0;
      }
      break;
    case ST_WDAY:
      if(btn==BTN_S){
        next_state=ST_IDLE;
        rtc_settime();
      }else if(btn==BTN_M){
        --wday;   
        if(wday<1) wday=7;
      }else if(btn==BTN_P){
        ++wday;
        if(wday>7) wday=1;
      }
      break;
  }
  display_idle();   
  if(state==ST_IDLE) delay(500);
  else delay(50);
  state=next_state;
}


void rtc_gettime(){
  int i=0;
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.endTransmission();
   
  Wire.requestFrom(0x68, 7);  
  while(Wire.available()){
    regs[i++]=Wire.read();
  }

  sec=(regs[0]&0x0f)+10*((regs[0]&0xf0)>>4);
  min=(regs[1]&0x0f)+10*((regs[1]&0xf0)>>4);
  hrs=10*((regs[2]&0x30)>>4)+(regs[2]&0x0F);
  wday=regs[3];
  day=10*((regs[4]&0x70)>>4)+(regs[4]&0x0F);
  mon=10*((regs[5]&0x10)>>4)+(regs[5]&0x0F);    
  y=10*((regs[6]&0xf0)>>4)+(regs[6]&0x0F);
}

void rtc_settime(){
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  regs[0]=0x00;
  Wire.write(regs[0]);
  regs[1]=(min%10)+((min/10)<<4);
  Wire.write(regs[1]);
  regs[2]=(hrs%10)+((hrs/10)<<4);
  Wire.write(regs[2]); 
  regs[3]=wday;
  Wire.write(regs[3]);
  regs[4]=(day%10)+((day/10)<<4);
  Wire.write(regs[4]);
  regs[5]=(mon%10)+((mon/10)<<4);
  Wire.write(regs[5]); 
  regs[6]=(y%10)+((y/10)<<4);
  Wire.write(regs[6]);
  Wire.endTransmission();
}

void eeprom_load(){
  al_hrs=EEPROM.read(0);
  al_min=EEPROM.read(1);
}

void eeprom_save(){
  EEPROM.write(0, al_hrs);
  EEPROM.write(1, al_min);
}

int buttons_get(){
  al_on=digitalRead(SW_A);  
  int s=digitalRead(BTN_S);
  if(!s){
    delay(10);
    s=digitalRead(BTN_S);
    if(!s) return BTN_S;
  }
  s=digitalRead(BTN_M);
  if(!s){
    delay(10);
    s=digitalRead(BTN_M);
    if(!s) return BTN_M;
  }
  s=digitalRead(BTN_P);
  if(!s){
    delay(10);
    s=digitalRead(BTN_P);
    if(!s) return BTN_P;
  }
  return -1;
}

void alarm_play(){
  int i;
  for(i=0;i<5;++i){
    digitalWrite(BZ, HIGH);
    delay(50);
    digitalWrite(BZ, LOW);
    delay(50);
  }
}

void display_idle(){
  oled.clearDisplay();
  oled.setTextColor(WHITE); 
  if(al_on){
    oled.setTextSize(2);   
    oled.setCursor(40,0);
    oled.print("AL");
    oled.setTextSize(1);
    oled.setCursor(60,0);
    oled.print(al_hrs, DEC);
    oled.print(':');
    oled.print(al_min, DEC);
  }
  
  oled.setTextSize(3);
  oled.setCursor(20,20);
  oled.print(hrs, DEC);
  oled.print(':');
  oled.print(min, DEC);

  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(20,55);
  oled.print(day, DEC);
  oled.print('.');
  oled.print(mon, DEC);
  oled.print('.');
  oled.print(20, DEC);
  oled.print(y, DEC);
  oled.print("  ");
  if(wday==1) oled.print("NED");
  else if(wday==2) oled.print("PON");
  else if(wday==3) oled.print("UTO");
  else if(wday==4) oled.print("SRI");
  else if(wday==5) oled.print("CET");
  else if(wday==6) oled.print("PET");
  else if(wday==7) oled.print("SUB");
  else oled.print("---");

  oled.setTextSize(1);
  if(state==ST_ALHRS || state==ST_ALMIN){
    oled.setCursor(0,0);
    oled.print("\x10");
  }else if(state==ST_HRS || state==ST_MIN){
    oled.setCursor(0, 30);
    oled.print("\x10");
  }else if(state==ST_DAY || state==ST_WDAY || state==ST_MON || state==ST_YR){
    oled.setCursor(0, 50);   
    oled.print("\x10");       
  }

  oled.display();
}


