#include"Game.h"
#include"Web.h"

const int RGB1[3] = {2, 0, 4}, RGB2[3] = {16, 17, 5};
const int CLK = 15, LAT = 32, OE = 33, ABC[3] = {25, 26, 27}, WIDTH = 32, HEIGHT = 16;

const int numbers[10][8]={
  {0b111111,
   0b111111,
   0b110011,
   0b110011,
   0b110011,
   0b110011,
   0b111111,
   0b111111,
  },
  {0b011100,
   0b111100,
   0b001100,
   0b001100,
   0b001100,
   0b001100,
   0b111111,
   0b111111,
  },
  {0b011110,
   0b110011,
   0b000110,
   0b001100,
   0b011000,
   0b110000,
   0b111111,
   0b111111,
  },
  {0b011110,
   0b111111,
   0b000011,
   0b001111,
   0b001111,
   0b000011,
   0b111111,
   0b011110,
  },
  {0b110011,
   0b110011,
   0b110011,
   0b111111,
   0b011111,
   0b000011,
   0b000011,
   0b000011,
  },
  {0b111111,
   0b111111,
   0b110000,
   0b111111,
   0b011111,
   0b000011,
   0b111111,
   0b111111,
  },
  {0b011110,
   0b110011,
   0b110000,
   0b111111,
   0b110011,
   0b110011,
   0b111111,
   0b011110,
  },
  {0b111111,
   0b111111,
   0b110011,
   0b110011,
   0b000011,
   0b000011,
   0b000011,
   0b000011,
  },
  {0b011110,
   0b111111,
   0b110011,
   0b111111,
   0b111111,
   0b110011,
   0b111111,
   0b011110,
  },
  {0b011110,
   0b111111,
   0b110011,
   0b111111,
   0b011111,
   0b000011,
   0b000011,
   0b000011,
  }
};

SnakeGame *game = new SnakeGame(32, 16);
_Web webs("SnakeGameAP");

void showScore(int score){
  int cnt=0;
  do{
    for(int i=0;i<8;i++){
      for(int j=0;j<6;j++){
        game->_map[i][26-cnt*7+j]=((numbers[score%10][i]>>(5-j))&1)==1?0b100:0;
      }
    }
    cnt++;
    score/=10;
  }while(score>0);
}
void showMap(){
  for(int i=0;i<16;i++){
    for(int j=0;j<32;j++){
      Serial.print(game->_map[i][j]);
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  webs.init();
  for (int i = 0; i < 3; i++) {
    pinMode(RGB1[i], OUTPUT);
    pinMode(RGB2[i], OUTPUT);
    pinMode(ABC[i], OUTPUT);
  }
  pinMode(CLK, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(CLK, LOW);
  digitalWrite(LAT, LOW);
  digitalWrite(OE, LOW);
  showScore(0);
}

void loop() {
  static int dir = OVER,score=0;
  static unsigned long long int game_t = millis();
  int dirW=webs._update();
  if(dirW!=EMPTY){
    if(dir==OVER){
      for(int i=0;i<8;i++){
        for(int j=0;j<32;j++){
          game->_map[i][j]=0;
        }
      }
    }
    dir=dirW;
    game->Chg_direction(dirW);
  }
  if (millis() - game_t >= 300) {
    game_t = millis();
    if(dir != OVER && game->_update() == OVER){
      score=max(score,game->len);
      delete game;
      game = new SnakeGame(32, 16);
      dir = OVER;
      showScore(score);
    }
  }
  for (int i = 0; i < HEIGHT / 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int p1 = game->_map[i][j] > 0 ? 0b001 : game->_map[i][j] == APPLE ? 0b100 : 0;
      int p2 = game->_map[i + HEIGHT / 2][j] > 0 ? 0b001 : game->_map[i + HEIGHT / 2][j] == APPLE ? 0b100 : 0;
      for (int k = 0; k < 3; k++) {
        digitalWrite(RGB1[k], (p1 >> (2 - k)) & 0b001);
        digitalWrite(RGB2[k], (p2 >> (2 - k)) & 0b001);
      }
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
    }
    digitalWrite(LAT, HIGH);
    digitalWrite(OE, HIGH);
    delayMicroseconds(3);
    for (int k = 0; k < 3; k++) {
      digitalWrite(ABC[k], (i >> k) & 0b001 );
    }
    delayMicroseconds(3);
    digitalWrite(LAT, LOW);
    digitalWrite(OE, LOW);
  }
  //デバッグ用
  /*
  static unsigned long showTime=millis();
  if(millis()-showTime>=1000){
    showTime=millis();
    showMap();
    Serial.println();
  }*/
}
