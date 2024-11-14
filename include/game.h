#ifndef GAME_H
#define GAME_H

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAX_LENGTH 100
#define MAX_OBSTACLES 20
#define SCREEN_WIDTH MAXX - 1
#define SCREEN_HEIGHT MAXY - 1

typedef struct {
    int x;
    int y;
} Point;

extern Point* snake;
extern int snakeLength;
extern Point* apple;
extern Point* obstacles;
extern int numObstacles;
extern int difficulty;
extern int direction;
extern int gameOver;
extern int applesEaten;
extern int score;
extern int highScores[3];

void initGame();
void drawSnake();
void drawApple();
void drawObstacles();
void clearPosition(int x, int y);
void updateGame();
void changeDirection(int newDirection);
void chooseDifficulty();
void drawScore();
void loadHighScores();
void saveHighScore();
void spawnApple(Point* apple);
void spawnObstacle(Point* obstacles);
int isPositionOccupied(int x, int y);

#endif // GAME_H
