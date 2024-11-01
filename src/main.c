/**
 * main.c
 * Snake Game using CLI-lib
 * Author: Caio Furtado
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define SCREEN_WIDTH MAXX - 1
#define SCREEN_HEIGHT MAXY - 1
#define MAX_LENGTH 100
#define MAX_OBSTACLES 20

typedef struct {
    int x;
    int y;
} Point;

Point snake[MAX_LENGTH];
int snakeLength = 5;
Point apple;
Point obstacles[MAX_OBSTACLES];
int numObstacles = 0;
int difficulty = 0; 
int direction = 0; 
int gameOver = 0;
int applesEaten = 0; 

// Função protótipos
void spawnApple();
void spawnObstacle();
int isPositionOccupied(int x, int y);
void initGame();
void drawSnake();
void drawApple();
void drawObstacles();
void clearPosition(int x, int y);
void updateGame();
void changeDirection(int newDirection);
void chooseDifficulty();

void initGame() {
    
    for (int i = 0; i < snakeLength; i++) {
        snake[i].x = 10 - i; 
        snake[i].y = 5;      
    }

    
    spawnApple();

    
    numObstacles = 0; 
    int initialObstacles = (difficulty == 0) ? 5 : (difficulty == 1) ? 10 : 15;

    for (int i = 0; i < initialObstacles; i++) {
        spawnObstacle();
    }
}

void drawSnake() {
    for (int i = 0; i < snakeLength; i++) {
        screenGotoxy(snake[i].x, snake[i].y);
        printf("O");
    }
}

void drawApple() {
    screenGotoxy(apple.x, apple.y);
    printf("A");
}

void drawObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        screenGotoxy(obstacles[i].x, obstacles[i].y);
        printf("X");
    }
}

void clearPosition(int x, int y) {
    screenGotoxy(x, y);
    printf(" ");
}

void spawnApple() {
    
    do {
        apple.x = rand() % (SCREEN_WIDTH - 2) + 1;
        apple.y = rand() % (SCREEN_HEIGHT - 2) + 1;
    } while (isPositionOccupied(apple.x, apple.y)); 

    
    screenGotoxy(apple.x, apple.y);
    printf("A");
}

int isPositionOccupied(int x, int y) {
    
    for (int i = 0; i < snakeLength; i++) {
        if (snake[i].x == x && snake[i].y == y) {
            return 1; 
        }
    }
    for (int i = 0; i < numObstacles; i++) {
        if (obstacles[i].x == x && obstacles[i].y == y) {
            return 1; 
        }
    }
    return 0; 
}

void spawnObstacle() {
    if (numObstacles < MAX_OBSTACLES) {
        Point newObstacle;
        do {
            newObstacle.x = rand() % (SCREEN_WIDTH - 2) + 1;
            newObstacle.y = rand() % (SCREEN_HEIGHT - 2) + 1;
        } while (isPositionOccupied(newObstacle.x, newObstacle.y)); 

        obstacles[numObstacles] = newObstacle;
        numObstacles++;
    }
}

void updateGame() {
    if (gameOver) return;

    
    Point newHead = snake[0];

    if (direction == 0) newHead.x++; 
    else if (direction == 1) newHead.y++; 
    else if (direction == 2) newHead.x--; 
    else if (direction == 3) newHead.y--; 

    
    if (newHead.x <= 0 || newHead.x >= SCREEN_WIDTH || newHead.y <= 0 || newHead.y >= SCREEN_HEIGHT) {
        gameOver = 1; 
        return;
    }

    
    for (int i = 0; i < snakeLength; i++) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            gameOver = 1; 
            return;
        }
    }

    
    for (int i = 0; i < numObstacles; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1;
            return;
        }
    }

    
    if (newHead.x == apple.x && newHead.y == apple.y) {
        
        snakeLength++;
        applesEaten++;

        
        spawnApple();

        
        if ((difficulty == 0 && applesEaten % 3 == 0) || 
            (difficulty == 1 && applesEaten % 2 == 0) || 
            (difficulty == 2 && applesEaten % 1 == 0)) { 
            spawnObstacle();
        }
    } else {
        
        clearPosition(snake[snakeLength - 1].x, snake[snakeLength - 1].y); 
    }

    
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    
    snake[0] = newHead;

    
    drawSnake();
    drawApple();
    drawObstacles();
}

void changeDirection(int newDirection) {
    
    if ((direction == 0 && newDirection != 2) ||
        (direction == 1 && newDirection != 3) ||
        (direction == 2 && newDirection != 0) ||
        (direction == 3 && newDirection != 1)) {
        direction = newDirection;
    }
}

void chooseDifficulty() {
    char choice;
    printf("Escolha a dificuldade (f - fácil, m - médio, d - difícil): ");
    while (1) {
        choice = getchar();
        if (choice == 'f') {
            difficulty = 0;
            break;
        } else if (choice == 'm') {
            difficulty = 1;
            break;
        } else if (choice == 'd') {
            difficulty = 2;
            break;
        } else {
            printf("Opção inválida. Tente novamente: ");
        }
    }
}

int main() {
    static int ch = 0;

    screenInit(1);
    keyboardInit();
    timerInit(100);
    srand(time(NULL));

    chooseDifficulty();
    initGame();

    while (ch != 10 && !gameOver) { 
        
        if (keyhit()) {
            ch = readch();
            if (ch == 'w') changeDirection(3); 
            if (ch == 's') changeDirection(1);
            if (ch == 'a') changeDirection(2); 
            if (ch == 'd') changeDirection(0);
        }

        
        if (timerTimeOver() == 1) {
            updateGame();
            screenUpdate();
        }
    }

    
    screenGotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
    printf("Game Over!");

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
