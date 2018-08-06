// Buttons
#define A   4
#define B   0
#define AB  5
char btn=-1;

// Display
#define SCL 2
#define SDA 1
#define DC  3
#define C   0
#define D   1
#define CONTRAST  30
unsigned char b, dc;

// Board
#define E 0
#define X 1
#define O 2
char board[6][6];
char selX, selY;
char moves;
char turn;
char scoreX, scoreO;
char win;
char tmp;


void setup() {
  // Buttons init
  pinMode(A, INPUT);
  pinMode(B, INPUT);

  // Display init
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);
  pinMode(DC, OUTPUT);
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, LOW);
  delay(3000);

  dc=C;
  b=0xAF;
  oled();
  b=0xA0;
  oled();
  b=0x00;
  oled();

  // set pull up on inputs(0,4)
  MCUCR=MCUCR & 0b10111111;
  PORTB=PORTB | 0b00010001;

  dc=C;
  b=0x81;     oled();
  b=CONTRAST; oled();
  b=0x82;     oled();
  b=CONTRAST; oled();
  b=0x83;     oled();
  b=CONTRAST; oled();
}


void loop() {
  start();
  drawBoard();
  
  while(1){
    drawXscore(scoreX);
    drawOscore(scoreO);
    if(win==X){
      drawUp(2);
      drawDown(0);
    }else if(win==O){
      drawUp(0);
      drawDown(3);
    }else if(win==-1){
      drawUp(0);
      drawDown(0);
    }else if(turn==X){
      drawUp(5);
      drawDown(0);
    }else if(turn==O){
      drawUp(0);
      drawDown(5);
    }
    
    // Read inputs
    while(btn==-1) getInput(); 
    if(btn==AB){
      btn=-1;
      break;
    }else if(btn==A){
      if(win==0) next();
    }else if(btn==B){
      if(select()){
        scoreX=search(X);
        scoreO=search(O);
        if(moves==36){
          if(scoreX>scoreO) win=X;
          else if(scoreO>scoreX) win=O;
          else win=-1;
        }
      }
    }
    btn=-1;
  }
}



// Game controls

void start(){
  char x, y;
  for(y=0;y<6;++y){
    for(x=0;x<6;++x){
      board[y][x]=0;
    }
  }
  moves=0;
  win=0;
  turn=X;
  scoreX=0;
  scoreO=0;
  selX=-1;
  selY=-1;
}

void next(){
  drawE(selX, selY);
  if(selX<0){
    selX=0;
    selY=0;
  }else{
    ++selX;
    if(selX>5){
      selX=0;
      selY++;
      if(selY>5) selY=0;
    }
  }

  while(board[selY][selX]!=0){
    ++selX;
    if(selX>5){
      selX=0;
      ++selY;
      if(selY>5){
        selY=0;
      }
    }
  }
  drawS(selX, selY);
}

char select(){
  if(selX<0) return 0;
  drawE(selX, selY);
  board[selY][selX]=turn;
  
  if(board[selY][selX]==X) drawX(selX, selY);
  else if(board[selY][selX]==O) drawO(selX, selY);
  
  ++moves;
  if(turn==X) turn=O;
  else turn=X;
  selX=-1;
  selY=-1;  
  return 1;
}

char search(char p){
  tmp=0;
  // Vertical
  if(board[5][0]==p && board[4][0]==p && board[3][0]==p && board[2][0]==p) ++tmp;
  if(board[4][0]==p && board[3][0]==p && board[2][0]==p && board[1][0]==p) ++tmp;
  if(board[3][0]==p && board[2][0]==p && board[1][0]==p && board[0][0]==p) ++tmp;

  if(board[5][1]==p && board[4][1]==p && board[3][1]==p && board[2][1]==p) ++tmp;
  if(board[4][1]==p && board[3][1]==p && board[2][1]==p && board[1][1]==p) ++tmp;
  if(board[3][1]==p && board[2][1]==p && board[1][1]==p && board[0][1]==p) ++tmp;

  if(board[5][2]==p && board[4][2]==p && board[3][2]==p && board[2][2]==p) ++tmp;
  if(board[4][2]==p && board[3][2]==p && board[2][2]==p && board[1][2]==p) ++tmp;
  if(board[3][2]==p && board[2][2]==p && board[1][2]==p && board[0][2]==p) ++tmp;

  if(board[5][3]==p && board[4][3]==p && board[3][3]==p && board[2][3]==p) ++tmp;
  if(board[4][3]==p && board[3][3]==p && board[2][3]==p && board[1][3]==p) ++tmp;
  if(board[3][3]==p && board[2][3]==p && board[1][3]==p && board[0][3]==p) ++tmp;

  if(board[5][4]==p && board[4][4]==p && board[3][4]==p && board[2][4]==p) ++tmp;
  if(board[4][4]==p && board[3][4]==p && board[2][4]==p && board[1][4]==p) ++tmp;
  if(board[3][4]==p && board[2][4]==p && board[1][4]==p && board[0][4]==p) ++tmp;

  if(board[5][5]==p && board[4][5]==p && board[3][5]==p && board[2][5]==p) ++tmp;
  if(board[4][5]==p && board[3][5]==p && board[2][5]==p && board[1][5]==p) ++tmp;
  if(board[3][5]==p && board[2][5]==p && board[1][5]==p && board[0][5]==p) ++tmp;

  // Horizontal
  if(board[0][0]==p && board[0][1]==p && board[0][2]==p && board[0][3]==p) ++tmp;
  if(board[0][1]==p && board[0][2]==p && board[0][3]==p && board[0][4]==p) ++tmp;
  if(board[0][2]==p && board[0][3]==p && board[0][4]==p && board[0][5]==p) ++tmp;

  if(board[1][0]==p && board[1][1]==p && board[1][2]==p && board[1][3]==p) ++tmp;
  if(board[1][1]==p && board[1][2]==p && board[1][3]==p && board[1][4]==p) ++tmp;
  if(board[1][2]==p && board[1][3]==p && board[1][4]==p && board[1][5]==p) ++tmp;

  if(board[2][0]==p && board[2][1]==p && board[2][2]==p && board[2][3]==p) ++tmp;
  if(board[2][1]==p && board[2][2]==p && board[2][3]==p && board[2][4]==p) ++tmp;
  if(board[2][2]==p && board[2][3]==p && board[2][4]==p && board[2][5]==p) ++tmp;

  if(board[3][0]==p && board[3][1]==p && board[3][2]==p && board[3][3]==p) ++tmp;
  if(board[3][1]==p && board[3][2]==p && board[3][3]==p && board[3][4]==p) ++tmp;
  if(board[3][2]==p && board[3][3]==p && board[3][4]==p && board[3][5]==p) ++tmp;

  if(board[4][0]==p && board[4][1]==p && board[4][2]==p && board[4][3]==p) ++tmp;
  if(board[4][1]==p && board[4][2]==p && board[4][3]==p && board[4][4]==p) ++tmp;
  if(board[4][2]==p && board[4][3]==p && board[4][4]==p && board[4][5]==p) ++tmp;

  if(board[5][0]==p && board[5][1]==p && board[5][2]==p && board[5][3]==p) ++tmp;
  if(board[5][1]==p && board[5][2]==p && board[5][3]==p && board[5][4]==p) ++tmp;
  if(board[5][2]==p && board[5][3]==p && board[5][4]==p && board[5][5]==p) ++tmp;

  //  -/-
  if(board[3][0]==p && board[2][1]==p && board[1][2]==p && board[0][3]==p) ++tmp;
  if(board[4][0]==p && board[3][1]==p && board[2][2]==p && board[1][3]==p) ++tmp;
  if(board[3][1]==p && board[2][2]==p && board[1][3]==p && board[0][4]==p) ++tmp;

  if(board[5][0]==p && board[4][1]==p && board[3][2]==p && board[2][3]==p) ++tmp;
  if(board[4][1]==p && board[3][2]==p && board[2][3]==p && board[1][4]==p) ++tmp;
  if(board[3][2]==p && board[2][3]==p && board[1][4]==p && board[0][5]==p) ++tmp;

  if(board[5][1]==p && board[4][2]==p && board[3][3]==p && board[2][4]==p) ++tmp;
  if(board[4][2]==p && board[3][3]==p && board[2][4]==p && board[1][5]==p) ++tmp;
  if(board[5][2]==p && board[4][3]==p && board[3][4]==p && board[2][5]==p) ++tmp;
  
  //  -\\-
  if(board[2][0]==p && board[3][1]==p && board[4][2]==p && board[5][3]==p) ++tmp;
  if(board[1][0]==p && board[2][1]==p && board[3][2]==p && board[4][3]==p) ++tmp;
  if(board[2][1]==p && board[3][2]==p && board[4][3]==p && board[5][4]==p) ++tmp;

  if(board[0][0]==p && board[1][1]==p && board[2][2]==p && board[3][3]==p) ++tmp;
  if(board[1][1]==p && board[2][2]==p && board[3][3]==p && board[4][4]==p) ++tmp;
  if(board[2][2]==p && board[3][3]==p && board[4][4]==p && board[5][5]==p) ++tmp;

  if(board[0][1]==p && board[1][2]==p && board[2][3]==p && board[3][4]==p) ++tmp;
  if(board[1][2]==p && board[2][3]==p && board[3][4]==p && board[4][5]==p) ++tmp;
  if(board[0][2]==p && board[1][3]==p && board[2][4]==p && board[3][5]==p) ++tmp;
  
  return tmp;
}


// Hardware functions

void getInput(){
  char a1=digitalRead(A);
  char b1=digitalRead(B);
  delay(30);
  char a2=digitalRead(A);
  char b2=digitalRead(B);
  
  if(!a1 && !a2 && b1 && b2) btn=A;
  else if(!b1 && !b2 && a1 && a2) btn=B;
  else if(!a1 && !a2 && !b1 && !b2){
    delay(2000);
    a2=digitalRead(A);
    b2=digitalRead(B);
    if(!a1 && !a2 && !b1 && !b2) btn=AB; 
    else btn=-1;
  }
  else btn=-1;
}

void drawUp(char color){
  drawLine(78, 8, 80, 8, color);
  drawLine(76, 9, 82, 9, color);
  drawLine(74, 10, 84, 10, color);
  drawLine(72, 11, 86, 11, color);
  drawLine(70, 12, 88, 12, color);
}

void drawDown(char color){
  drawLine(70, 20, 88, 20, color);
  drawLine(72, 21, 86, 21, color);
  drawLine(74, 22, 84, 22, color);
  drawLine(76, 23, 82, 23, color);
  drawLine(78, 24, 80, 24, color);
}

void drawOscore(char digit){
  drawLine(71, 57, 71, 61, 0);
  drawLine(74, 57, 74, 61, 0);// | left
  drawLine(78, 57, 78, 61, 0);// | right
  drawLine(74, 57, 78, 57, 0);// up
  drawLine(74, 59, 78, 59, 0);//center
  drawLine(74, 61, 78, 61, 0);//down
  
  char m=digit%10;
  if(digit>9) drawLine(71, 57, 71, 61, 4);
  
  if(m==0 || m==8){
    drawLine(74, 57, 74, 61, 4);// | left
    drawLine(78, 57, 78, 61, 4);// | right
    drawLine(74, 57, 78, 57, 4);// up
    if(m==8) drawLine(74, 59, 78, 59, 4);//center
    drawLine(74, 61, 78, 61, 4);//down
  }else if(m==1){
    drawLine(78, 57, 78, 61, 4);  
  }else if(m==2 || m==3){
    drawLine(74, 57, 78, 57, 4);
    drawLine(74, 59, 78, 59, 4);
    drawLine(74, 61, 78, 61, 4);
    if(m==2){
      drawLine(74, 59, 74, 61, 4);
      drawLine(78, 57, 78, 59, 4);// | right (upper)
    }
    else  drawLine(78, 57, 78, 61, 4);
  }else if(m==4){
    drawLine(78, 57, 78, 61, 4);
    drawLine(74, 59, 78, 59, 4);
    drawLine(74, 57, 74, 59, 4);// | left (upper)
  }else if(m==5 || m==6){
    drawLine(74, 57, 78, 57, 4);// up
    drawLine(74, 59, 78, 59, 4);//center
    drawLine(74, 61, 78, 61, 4);//down
    drawLine(74, 57, 74, 59, 4);// | left (upper)
    drawLine(78, 59, 78, 61, 4);// | right (lower)
    if(m==6){
      drawLine(74, 59, 74, 61, 4);// | left (lower)
    }
  }else{
    drawLine(78, 57, 78, 61, 4);// | right
    drawLine(74, 57, 78, 57, 4);// up
    if(m==9){
      drawLine(74, 59, 78, 59, 4);//center
      drawLine(74, 61, 78, 61, 4);//down
      drawLine(74, 57, 74, 59, 4);// | left (upper)
    }
  }
  
}

void drawXscore(char digit){
  drawLine(71, 34, 71, 38, 0);
  drawLine(74, 34, 74, 38, 0);// | left
  drawLine(78, 34, 78, 38, 0);// | right
  drawLine(74, 34, 78, 34, 0);// up
  drawLine(74, 36, 78, 36, 0);//center
  drawLine(74, 38, 78, 38, 0);//down
  
  char m=digit%10;
  if(digit>9) drawLine(71, 34, 71, 38, 4);
  
  if(m==0 || m==8){
    drawLine(74, 34, 74, 38, 4);// | left
    drawLine(78, 34, 78, 38, 4);// | right
    drawLine(74, 34, 78, 34, 4);// up
    if(m==8) drawLine(74, 36, 78, 36, 4);//center
    drawLine(74, 38, 78, 38, 4);//down
  }else if(m==1){
    drawLine(78, 34, 78, 38, 4);  
  }else if(m==2 || m==3){
    drawLine(74, 34, 78, 34, 4);
    drawLine(74, 36, 78, 36, 4);
    drawLine(74, 38, 78, 38, 4);
    if(m==2){
      drawLine(74, 36, 74, 38, 4);
      drawLine(78, 34, 78, 36, 4);// | right (upper)
    }
    else  drawLine(78, 34, 78, 38, 4);
  }else if(m==4){
    drawLine(78, 34, 78, 38, 4);
    drawLine(74, 36, 78, 36, 4);
    drawLine(74, 34, 74, 36, 4);// | left (upper)
  }else if(m==5 || m==6){
    drawLine(74, 34, 78, 34, 4);// up
    drawLine(74, 36, 78, 36, 4);//center
    drawLine(74, 38, 78, 38, 4);//down
    drawLine(74, 34, 74, 36, 4);// | left (upper)
    drawLine(78, 36, 78, 38, 4);// | right (lower)
    if(m==6) drawLine(74, 36, 74, 38, 4);// | left (lower)
  }else{
    drawLine(78, 34, 78, 38, 4);// | right
    drawLine(74, 34, 78, 34, 4);// up
    if(m==9){
      drawLine(74, 36, 78, 36, 4);//center
      drawLine(74, 38, 78, 38, 4);//down
      drawLine(74, 34, 74, 36, 4);// | left (upper)
    }
  }
  
}

void drawBoard(){
  int a;
  //clear screen
  for(a=0;a<64;++a){
    drawLine(0, a, 95, a, 0);
  }
  //vertical
  for(a=1;a<32;a=a+5){
    drawLine(1, a, 61, a, 1);
  }
  //horizontal
  for(a=1;a<62;a=a+10){
    drawLine(a, 1, a, 31, 1);
  }
  //right
  drawLine(65, 1, 93, 1, 1);
  drawLine(65, 31, 93, 31, 1);
  drawLine(65, 1, 65, 31, 1);
  drawLine(93, 1, 93, 31, 1);

  //Red
  for(a=0;a<7;++a){
    if(a%2==0) drawLine(83, (6+a)/2, 89, (6+a)/2, 2);
    else       drawLine(83, (6+a)/2+32, 89, (6+a)/2+32, 2);
  }
  //Blue
  for(a=0;a<7;++a){
    if(a%2==0) drawLine(83, (52+a)/2, 89, (52+a)/2, 3);
    else       drawLine(83, (52+a)/2+32, 89, (52+a)/2+32, 3);
  }
}

void drawX(char i, char j){
  drawLine(3+i*10, (4+j*10)/2,    4+i*10, (4+j*10)/2, 2);
  drawLine(8+i*10, (4+j*10)/2,    9+i*10, (4+j*10)/2, 2);
  drawLine(3+i*10, (5+j*10)/2+32,    5+i*10, (5+j*10)/2+32, 2);
  drawLine(7+i*10, (5+j*10)/2+32,    9+i*10, (5+j*10)/2+32, 2);

  drawLine(4+i*10, (6+j*10)/2,    8+i*10, (6+j*10)/2, 2);
  drawLine(5+i*10, (7+j*10)/2+32,    7+i*10, (7+j*10)/2+32, 2);
  drawLine(4+i*10, (8+j*10)/2,    8+i*10, (8+j*10)/2, 2);

  drawLine(3+i*10, (9+j*10)/2+32,    5+i*10, (9+j*10)/2+32, 2);
  drawLine(7+i*10, (9+j*10)/2+32,    9+i*10, (9+j*10)/2+32, 2);
  drawLine(3+i*10, (10+j*10)/2,    4+i*10, (10+j*10)/2, 2);
  drawLine(8+i*10, (10+j*10)/2,    9+i*10, (10+j*10)/2, 2);
}

void drawO(char i, char j){
  drawLine(4+i*10, (4+j*10)/2,    8+i*10, (4+j*10)/2,    3);
  drawLine(3+i*10, (5+j*10)/2+32, 9+i*10, (5+j*10)/2+32, 3);
  drawLine(3+i*10, (9+j*10)/2+32, 9+i*10, (9+j*10)/2+32, 3);
  drawLine(4+i*10, (10+j*10)/2,   8+i*10, (10+j*10)/2,   3);
  
  drawLine(3+i*10, (6+j*10)/2, 3+i*10, (8+j*10)/2, 3);
  drawLine(4+i*10, (6+j*10)/2, 4+i*10, (8+j*10)/2, 3);
  drawLine(8+i*10, (6+j*10)/2, 8+i*10, (8+j*10)/2, 3);
  drawLine(9+i*10, (6+j*10)/2, 9+i*10, (8+j*10)/2, 3);

  //center
  drawLine(3+i*10, (7+j*10)/2+32, 4+i*10, (7+j*10)/2+32, 3);
  drawLine(8+i*10, (7+j*10)/2+32, 9+i*10, (7+j*10)/2+32, 3);
}

void drawS(char i, char j){
  drawLine(5+10*i, (6+j*10)/2, 7+10*i,    (6+j*10)/2,    4);
  drawLine(5+10*i, (7+j*10)/2+32, 7+10*i, (7+j*10)/2+32, 4);
  drawLine(5+10*i, (8+j*10)/2, 7+10*i,    (8+j*10)/2,    4);
}

void drawE(char i, char j){
  drawLine(5+10*i, (6+j*10)/2, 7+10*i,    (6+j*10)/2,    0);
  drawLine(5+10*i, (7+j*10)/2+32, 7+10*i, (7+j*10)/2+32, 0);
  drawLine(5+10*i, (8+j*10)/2, 7+10*i,    (8+j*10)/2,    0);
}

void drawLine(char x1, char y1, char x2, char y2, unsigned char c){
  dc=C;
  b=0x21; oled();
  
  b=x1; oled();
  b=y1; oled();
  b=x2; oled();
  b=y2; oled();
  if(c==0){ 
    b=0; oled();
    b=0; oled();
    b=0; oled();
  }else if(c==1){
    b=63; oled();
    b=31; oled();
    b=0; oled();
  }else if(c==2){
    b=255; oled();
    b=0; oled();
    b=0; oled();
  }else if(c==3){
    b=0; oled();
    b=0; oled();
    b=255; oled();
  }else if(c==4){
    b=0; oled();
    b=255; oled();
    b=0; oled();
  }else if(c==5){
    b=7; oled();
    b=6; oled();
    b=7; oled();
  }else if(c==6){
    b=255; oled();
    b=255; oled();
    b=255; oled();
  }else if(c==7){
    b=0; oled();
    b=0; oled;
    b=255; oled();
  }
}

void oled(){
  digitalWrite(DC, dc);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x80);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x40);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x20);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x10);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x08);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x04);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x02);
  digitalWrite(SCL, HIGH);

  digitalWrite(SCL, LOW);
  digitalWrite(SDA, b&0x01);
  digitalWrite(SCL, HIGH);
}

