#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define OVER 5
#define APPLE -1
#define EMPTY 0
class SnakeGame {
  private:
    int max_x, max_y, Direction = RIGHT;
    void put_apple() {
      int randx, randy;
      while (1) {
        randx = random(max_x);
        randy=random(max_y);
        if (_map[randy][randx] != EMPTY) {
          continue;
        }
        switch (Direction) {
          case UP:
            if (x != randx || randy > y) {
              _map[randy][randx] = APPLE;
              return;
            }
            break;
          case DOWN:
            if (x != randx || randy < y) {
              _map[randy][randx] = APPLE;
              return;
            }
            break;
          case RIGHT:
            if (y != randy || randx > x) {
              _map[randy][randx] = APPLE;
              return;
            }
            break;
          case LEFT:
            if (y != randy || randx < x) {
              _map[randy][randx] = APPLE;
              return;
            }
            break;
        }
      }
    }
  public:
    int _map[128][128],x, y,len = 3;
    SnakeGame(int _x, int _y) {
      max_x = _x;
      max_y = _y;
      x = max_x / 2;
      y = max_y / 2+1;
      for (int i = 0; i < max_y; i++) {
        for (int j = 0; j < max_x; j++) {
          _map[i][j] = EMPTY;
        }
      }
      _map[y][x] = len;
      _map[y][x+3]=APPLE;
    }
    void Chg_direction(int key) {
      switch (key) {
        case UP:
        case DOWN:
          if (Direction == LEFT || Direction == RIGHT) {
            Direction = key;
          }
          break;
        case RIGHT:
        case LEFT:
          if (Direction == UP || Direction == DOWN) {
            Direction = key;
          }
      }
    }
    int _update() {
      switch (Direction) {
        case UP:
          y--;
          break;
        case DOWN:
          y++;
          break;
        case RIGHT:
          x++;
          break;
        case LEFT:
          x--;
          break;
      }
      if (0 > y || y >= max_y || 0 > x || x >= max_x) {
        return OVER;
      }
      if (_map[y][x] != APPLE && _map[y][x] != EMPTY) {
        return OVER;
      }
      if (_map[y][x] == APPLE) {
        len++;
        put_apple();
      } else {
        for (int i = 0; i < max_y; i++) {
          for (int j = 0; j < max_x; j++) {
            if (_map[i][j] > 0)_map[i][j]--;
          }
        }
      }
      _map[y][x] = len;
      return 0;
    }
};
