void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  char d=5;
  while(1){
    digitalWrite(13, HIGH);
    delay(d);
    digitalWrite(13, LOW);
    delay(d);

    
  }
}
