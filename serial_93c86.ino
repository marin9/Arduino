int CS=5;
int CK=4;
int DI=3;
int DO=2;
int T=1;

void setup() {
	pinMode(CS, OUTPUT);
	pinMode(CK, OUTPUT);
	pinMode(DI, OUTPUT);
	pinMode(DO, INPUT);
	
	Serial.begin(9600);
}

//MAIN
void loop() {
	digitalWrite(CS, LOW);
	digitalWrite(CK, LOW);
	digitalWrite(DI, LOW);
	Serial.write("Start.\n");

  //writeEnableFlash();
  //writeFlash(0, 1);
  //writeFlash(1, 2);
  //writeFlash(2, 4);
  //writeFlash(3, 8);
  //writeFlash(50, 250);
	while(1){
		readFlash(0);
		readFlash(1);
    readFlash(2);
    readFlash(3);
    readFlash(50);

    Serial.write("\n");
		delay(2000);
	}
	//TODO
}



void ck(){
	delay(T);
	digitalWrite(CK, HIGH);
	delay(T);
	digitalWrite(CK, LOW);
}

unsigned char readFlash(int addr){	
	ck();
	digitalWrite(CS, HIGH);
	digitalWrite(DI, HIGH);
	ck();
	digitalWrite(DI, HIGH);
	ck();
	digitalWrite(DI, LOW);
	ck();
	
	//Addr
	digitalWrite(DI, (addr>>10)&1);
	ck();
	digitalWrite(DI, (addr>>9)&1);
	ck();
	digitalWrite(DI, (addr>>8)&1);
	ck();
	digitalWrite(DI, (addr>>7)&1);
	ck();
	digitalWrite(DI, (addr>>6)&1);
	ck();
	digitalWrite(DI, (addr>>5)&1);
	ck();
	digitalWrite(DI, (addr>>4)&1);
	ck();
	digitalWrite(DI, (addr>>3)&1);
	ck();
	digitalWrite(DI, (addr>>2)&1);
	ck();
	digitalWrite(DI, (addr>>1)&1);
	ck();
	digitalWrite(DI, (addr>>0)&1);
	ck();
	
	ck();
	char c7=digitalRead(DO);
	ck();
	char c6=digitalRead(DO);
	ck();
	char c5=digitalRead(DO);
	ck();
	char c4=digitalRead(DO);
	ck();
	char c3=digitalRead(DO);
	ck();
	char c2=digitalRead(DO);
	ck();
	char c1=digitalRead(DO);
	ck();
	char c0=digitalRead(DO);
	
	if(c7) Serial.write("1");
	else Serial.write("0");
	if(c6) Serial.write("1");
	else Serial.write("0");
	if(c5) Serial.write("1");
	else Serial.write("0");
	if(c4) Serial.write("1");
	else Serial.write("0");
	if(c3) Serial.write("1");
	else Serial.write("0");
	if(c2) Serial.write("1");
	else Serial.write("0");
	if(c1) Serial.write("1");
	else Serial.write("0");
	if(c0) Serial.write("1");
	else Serial.write("0");
	Serial.write("\n");

	digitalWrite(CS, LOW);
	
	unsigned char data=0;
	data=data|c7;
	data=data<<1;
	data=data|c6;
	data=data<<1;
	data=data|c5;
	data=data<<1;
	data=data|c4;
	data=data<<1;
	data=data|c3;
	data=data<<1;
	data=data|c2;
	data=data<<1;
	data=data|c1;
	data=data<<1;
	data=data|c0;
	
	digitalWrite(DI, LOW);
	
	return data;
}

void writeEnableFlash(){
	ck();
	digitalWrite(CS, HIGH);
	digitalWrite(DI, HIGH);
	ck();
	digitalWrite(DI, LOW);
	ck();
	digitalWrite(DI, LOW);
	ck();
	
	
	digitalWrite(DI, HIGH);
	ck();
	digitalWrite(DI, HIGH);
	ck();
	
	ck();
	ck();
	ck();
	ck();	
	ck();
	ck();
	ck();
	ck();
	ck();
	
	digitalWrite(CS, LOW);
	digitalWrite(DI, LOW);
}

void writeFlash(int addr, unsigned char data){
	ck();
	digitalWrite(CS, HIGH);
	digitalWrite(DI, HIGH);
	ck();
	digitalWrite(DI, LOW);
	ck();
	digitalWrite(DI, HIGH);
	ck();
	
	//Addr
	digitalWrite(DI, (addr>>10)&1);
	ck();
	digitalWrite(DI, (addr>>9)&1);
	ck();
	digitalWrite(DI, (addr>>8)&1);
	ck();
	digitalWrite(DI, (addr>>7)&1);
	ck();
	digitalWrite(DI, (addr>>6)&1);
	ck();
	digitalWrite(DI, (addr>>5)&1);
	ck();
	digitalWrite(DI, (addr>>4)&1);
	ck();
	digitalWrite(DI, (addr>>3)&1);
	ck();
	digitalWrite(DI, (addr>>2)&1);
	ck();
	digitalWrite(DI, (addr>>1)&1);
	ck();
	digitalWrite(DI, (addr>>0)&1);
	ck();
	
	//Data
	digitalWrite(DI, (data>>7)&1);
	ck();
	digitalWrite(DI, (data>>6)&1);
	ck();
	digitalWrite(DI, (data>>5)&1);
	ck();
	digitalWrite(DI, (data>>4)&1);
	ck();
	digitalWrite(DI, (data>>3)&1);
	ck();
	digitalWrite(DI, (data>>2)&1);
	ck();
	digitalWrite(DI, (data>>1)&1);
	ck();
	digitalWrite(DI, (data>>0)&1);
	ck();
	
	while(1){
		char c=digitalRead(DO);
		if(c) break;
		ck();
	}
	
	digitalWrite(CS, LOW);
	digitalWrite(DI, LOW);
	
	Serial.write("Write finish.\n");
}



