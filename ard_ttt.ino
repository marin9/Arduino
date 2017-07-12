#include "TimerOne.h"

void setup(){
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(13, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
Serial.begin(9600);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(timerInt);
}

#define X 1
#define O 2
#define PLAY 0
#define DRAW 1
#define WIN 2

char board[9]; //0-empty, 1-x, 2-o
char signDraw;	
char moveNum;
char gameStatus;
char pressedNum;

void loop(){
  initGame();
  
  while(1){   
    if(gameStatus==WIN) blink();
    else draw(5);
  } 
}


void initGame(){
  int i;
  for(i=0;i<9;++i) 
    board[i]=0; 
   
  gameStatus=PLAY;
  signDraw=X;
  moveNum=0;
  pressedNum=0;
}

void timerInt(){ 
  if(gameStatus!=PLAY) return;
  getInput();
  isGameOver();
}

void isGameOver(){
  if(board[0]!=0 && board[0]==board[1] && board[1]==board[2]) gameStatus=WIN;
  else if(board[3]!=0 && board[3]==board[4] && board[4]==board[5]) gameStatus=WIN;
  else if(board[6]!=0 && board[6]==board[7] && board[7]==board[8]) gameStatus=WIN;

  else if(board[0]!=0 && board[0]==board[3] && board[3]==board[6]) gameStatus=WIN;
  else if(board[1]!=0 && board[1]==board[4] && board[4]==board[7]) gameStatus=WIN;
  else if(board[2]!=0 && board[2]==board[5] && board[5]==board[8]) gameStatus=WIN;

  else if(board[0]!=0 && board[0]==board[4] && board[4]==board[8]) gameStatus=WIN;
  else if(board[2]!=0 && board[2]==board[4] && board[4]==board[6]) gameStatus=WIN;

  else if(moveNum==9) gameStatus=DRAW;
}

char getInput(){
  int used=0;
  
  digitalWrite(13, LOW); //first row
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  
  if(digitalRead(14)==0) used=pmove(0);
  else if(digitalRead(15)==0) used=pmove(1);
  else if(digitalRead(16)==0) used=pmove(2);
  
  digitalWrite(13, HIGH); 
  digitalWrite(18, LOW);
  digitalWrite(19, HIGH);
  
  if(digitalRead(14)==0) used=pmove(3);
  else if(digitalRead(15)==0) used=pmove(4);
  else if(digitalRead(16)==0) used=pmove(5);
  
  digitalWrite(13, HIGH); 
  digitalWrite(18, HIGH);
  digitalWrite(19, LOW);
  
  if(digitalRead(14)==0) used=pmove(6);
  else if(digitalRead(15)==0) used=pmove(7);
  else if(digitalRead(16)==0) used=pmove(8);
  
   
  if(used) ++pressedNum;
  else pressedNum=0;
  
  if(used && pressedNum>5){
     blinkSign();
  }
  
  return used;
}

char pmove(char n){
  if(board[n]==0){
    board[n]=signDraw;
    ++moveNum;
    if(signDraw==X) signDraw=O;
    else signDraw=X;  
    return 0; 
  }else{
    return 1;
  }
}


void blink(){
  int a, tmp, time=35; 
  Serial.println("ok");
  if(board[0]!=0 && board[0]==board[1] && board[1]==board[2]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[0];
    board[0]=board[1]=board[2]=3;   
    for(a=0;a<time;++a) draw(5);
    board[0]=board[1]=board[2]=tmp; 
    
  }else if(board[3]!=0 && board[3]==board[4] && board[4]==board[5]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[3];
    board[3]=board[4]=board[5]=3;   
    for(a=0;a<time;++a) draw(5);
    board[3]=board[4]=board[5]=tmp;
    
  }else if(board[6]!=0 && board[6]==board[7] && board[7]==board[8]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[6];
    board[6]=board[7]=board[8]=3;   
    for(a=0;a<time;++a) draw(5);
    board[6]=board[7]=board[8]=tmp;
    
  }else if(board[0]!=0 && board[0]==board[3] && board[3]==board[6]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[0];
    board[0]=board[3]=board[6]=3;   
    for(a=0;a<time;++a) draw(5);
    board[0]=board[3]=board[6]=tmp;
    
  }else if(board[1]!=0 && board[1]==board[4] && board[4]==board[7]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[1];
    board[1]=board[4]=board[7]=3;   
    for(a=0;a<time;++a) draw(5);
    board[1]=board[4]=board[7]=tmp;
    
  }else if(board[2]!=0 && board[2]==board[5] && board[5]==board[8]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[2];
    board[2]=board[5]=board[8]=3;   
    for(a=0;a<time;++a) draw(5);
    board[2]=board[5]=board[8]=tmp;
    
  }else if(board[0]!=0 && board[0]==board[4] && board[4]==board[8]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[0];
    board[0]=board[4]=board[8]=3;   
    for(a=0;a<time;++a) draw(5);
    board[0]=board[4]=board[8]=tmp;
    
  }else if(board[2]!=0 && board[2]==board[4] && board[4]==board[6]){
    for(a=0;a<time;++a) draw(5);
    tmp=board[2];
    board[2]=board[4]=board[6]=3;   
    for(a=0;a<time;++a) draw(5);
    board[2]=board[4]=board[6]=tmp;
    
  }
}

void blinkSign(){
  char pom[9];
  int i;
  for(i=0;i<9;++i){
    pom[i]=board[i];
    board[i]=0;
  }
  
  if(signDraw==X) board[0]=board[2]=board[4]=board[6]=board[8]=X;
  else board[1]=board[3]=board[5]=board[7]=O;
 
  for(i=0;i<100;++i) draw(5);
  
  for(i=0;i<9;++i){
    board[i]=pom[i];
  }
}

void draw(int time_ms){
  //draw first row
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);  
  
  if(board[0]==1) digitalWrite(8, HIGH);
  else if(board[0]==2) digitalWrite(9, HIGH);
  else if(board[0]!=0){
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  
  if(board[1]==1) digitalWrite(6, HIGH);
  else if(board[1]==2) digitalWrite(7, HIGH);
  else if(board[1]!=0){
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  
  if(board[2]==1) digitalWrite(4, HIGH);
  else if(board[2]==2) digitalWrite(5, HIGH);
  else if(board[2]!=0){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(time_ms);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  
  //draw secons row
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);  
  
  if(board[3]==1) digitalWrite(8, HIGH);
  else if(board[3]==2) digitalWrite(9, HIGH);
  else if(board[3]!=0){
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  
  if(board[4]==1) digitalWrite(6, HIGH);
  else if(board[4]==2) digitalWrite(7, HIGH);
  else if(board[4]!=0){
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  
  if(board[5]==1) digitalWrite(4, HIGH);
  else if(board[5]==2) digitalWrite(5, HIGH);
  else if(board[5]!=0){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(time_ms);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  
  //draw third row
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);  
  
  if(board[6]==1) digitalWrite(8, HIGH);
  else if(board[6]==2) digitalWrite(9, HIGH);
  else if(board[6]!=0){
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  
  if(board[7]==1) digitalWrite(6, HIGH);
  else if(board[7]==2) digitalWrite(7, HIGH);
  else if(board[7]!=0){
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  
  if(board[8]==1) digitalWrite(4, HIGH);
  else if(board[8]==2) digitalWrite(5, HIGH);
  else if(board[8]!=0){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(time_ms);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
}


