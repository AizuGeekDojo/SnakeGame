#include"Game.h"
#include"Web.h"

const int RGB1[3] = {2, 0, 4}, RGB2[3] = {16, 17, 5};
const int CLK = 15, LAT = 32, OE = 33, ABC[3] = {25, 26, 27}, WIDTH = 32, HEIGHT = 16;

SnakeGame *game = new SnakeGame(32, 16);
_Web webs("SnakeGameAP");
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
}

void loop() {
  static int dir = EMPTY;
  static unsigned long long int game_t = millis();
  int dirW=webs._update();
  if(dirW!=EMPTY){
    dir=dirW;
    game->Chg_direction(dirW);
  }
  if (millis() - game_t >= 300) {
    game_t = millis();
    if (dir != EMPTY && game->_update() == OVER) {
      delete game;
      game = new SnakeGame(32, 16);
      dir = EMPTY;
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
}
