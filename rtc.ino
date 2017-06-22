#include <Wire.h> 

void setup(void) { 
  Wire.begin();
  Serial.begin(9600);  
} 

void loop(void) { 
  Wire.beginTransmission(0x68);
  Wire.write(0x00);    
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  while(1){  
    Serial.print("Minute: ");
    Serial.println(readMinute(), DEC);
    Serial.print("Seconds: ");
    Serial.println(readSeconds(), DEC);
    Serial.println("");
    delay(1000);
  }
} 

char readSeconds(){
  char reg, sec;
  Wire.beginTransmission(0x68);
  Wire.write(0x00);  
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);  
  while(Wire.available()){ 
    reg=Wire.read();           
  }

  sec=10*((reg&0x70)>>4)+(reg&0x0F);
  return sec;
}

char readMinute(){
  char reg, min;
  Wire.beginTransmission(0x68);
  Wire.write(0x01);  
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);  
  while(Wire.available()){ 
    reg=Wire.read();           
  }

  min=10*((reg&0x70)>>4)+(reg&0x0F);
  return min;
}


