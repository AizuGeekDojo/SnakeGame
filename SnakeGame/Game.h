#pragma once
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define OVER 1
#define APPLE -1
#define EMPTY 0
class SnakeGame {
private:
	int x,y,max_x, max_y,direction=RIGHT,len=3;
	void put_apple() {
		int randx, randy;
		while (1) {
			randx = Random(max_x-1);
			randy = Random(max_y-1);
			if (map[randy][randx] != EMPTY) {
				continue;
			}
			switch (direction) {
				case UP:
					if (x != randx || randy > y) {
						map[randy][randx] = APPLE;
						return;
					}
					break;
				case DOWN:
					if (x != randx || randy < y) {
						map[randy][randx] = APPLE;
						return;
					}
					break;
				case RIGHT:
					if (y != randy || randx > x) {
						map[randy][randx] = APPLE;
						return;
					}
					break;
				case LEFT:
					if (y != randy || randx < x) {
						map[randy][randx] = APPLE;
						return;
					}
					break;
			}
		}
	}
public:
	int map[128][128];
	SnakeGame(int _x, int _y) {
		max_x = _x;
		max_y = _y;
		x = max_x / 2;
		y = max_y / 2;
		for (int i = 0; i < max_y; i++) {
			for (int j = 0; j < max_x; j++) {
				map[i][j] = EMPTY;
			}
		}
		map[y][x] = len;
		put_apple();
	}
	void Chg_direction(int key) {
		switch (key) {
			case UP:
			case DOWN:
				if (direction == LEFT || direction == RIGHT) {
					direction = key;
				}
				break;
			case RIGHT:
			case LEFT:
				if (direction == UP || direction == DOWN) {
					direction = key;
				}
		}
	}
	int update() {
		switch (direction) {
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
		if (map[y][x] != APPLE && map[y][x] != EMPTY) {
			return OVER;
		}
		if (map[y][x] == APPLE) {
			len++;
			put_apple();
		} else {
			for (int i = 0; i < max_y; i++) {
				for (int j = 0; j < max_x; j++) {
					if (map[i][j] > 0)map[i][j]--;
				}
			}
		}
		map[y][x] = len;
		return 0;
	}
};
