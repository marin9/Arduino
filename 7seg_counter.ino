#include "TimerOne.h"

void setup(){
 pinMode(19, OUTPUT);
 pinMode(18, OUTPUT);
 pinMode(17, OUTPUT);
 pinMode(16, OUTPUT);
 pinMode(15, OUTPUT);
 pinMode(14, OUTPUT);
 pinMode(13, OUTPUT);
 
 pinMode(9, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(11, OUTPUT);
 
 //Timer1.initialize(1000000); 
 //Timer1.attachInterrupt(intr);
}

int num=0;
int m=0;

int h=255;
int l=60;
int time=1;


void intr(){
 ++num; 
}

void loop(){
  if(digitalRead(2))++m;
  if(m==10){
    ++num;
    m=0;
  }
  if(num>999) num=0;
  
  analogWrite(9, h);
  analogWrite(10, h);
  analogWrite(11, h);
  
  showDigit(0);
  analogWrite(9, l);
  analogWrite(10, h);
  analogWrite(11, h);
  delay(time);
  analogWrite(9, h);
  analogWrite(10, h);
  analogWrite(11, h);
  
  showDigit(1);
  analogWrite(9, h);
  analogWrite(10, l);
  analogWrite(11, h);
  delay(time);
  analogWrite(9, h);
  analogWrite(10, h);
  analogWrite(11, h);
  
  showDigit(2);
  analogWrite(9, h);
  analogWrite(10, h);
  analogWrite(11, l);
  delay(time);
  analogWrite(9, h);
  analogWrite(10, h);
  analogWrite(11, h);
}


void showDigit(int n){
  int digit;
  
  if(n==0) digit=num%10;
  else if(n==1) digit=num/10%10;
  else if(n==2) digit=num/100;
  
  if(digit==0) show0();
  else if(digit==1) show1();
  else if(digit==2) show2();
  else if(digit==3) show3();
  else if(digit==4) show4();
  else if(digit==5) show5();
  else if(digit==6) show6();
  else if(digit==7) show7();
  else if(digit==8) show8();
  else if(digit==9) show9();
}


void show0(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, HIGH); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, LOW); 
}

void show1(){
  digitalWrite(19, LOW); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, LOW); 
  digitalWrite(15, LOW); 
  digitalWrite(14, LOW); 
  digitalWrite(13, LOW); 
}

void show2(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, LOW); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, HIGH); 
  digitalWrite(14, LOW); 
  digitalWrite(13, HIGH); 
}

void show3(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, LOW); 
  digitalWrite(14, LOW); 
  digitalWrite(13, HIGH); 
}

void show4(){
  digitalWrite(19, LOW); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, LOW); 
  digitalWrite(15, LOW); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, HIGH); 
}

void show5(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, LOW); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, LOW); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, HIGH); 
}

void show6(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, LOW); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, HIGH); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, HIGH); 
}

void show7(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, LOW); 
  digitalWrite(15, LOW); 
  digitalWrite(14, LOW); 
  digitalWrite(13, LOW); 
}

void show8(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, HIGH); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, HIGH); 
}

void show9(){
  digitalWrite(19, HIGH); 
  digitalWrite(18, HIGH); 
  digitalWrite(17, HIGH); 
  digitalWrite(16, HIGH); 
  digitalWrite(15, LOW); 
  digitalWrite(14, HIGH); 
  digitalWrite(13, HIGH); 
}
