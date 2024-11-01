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
int difficulty = 0; // 0: fácil, 1: médio, 2: difícil
int direction = 0; // 0: right, 1: down, 2: left, 3: up
int gameOver = 0;
int applesEaten = 0; // Para controlar quantas maçãs foram comidas

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
    // Initialize snake position
    for (int i = 0; i < snakeLength; i++) {
        snake[i].x = 10 - i; // Start snake horizontally
        snake[i].y = 5;      // Start snake at row 5
    }

    // Place the first apple
    spawnApple();

    // Initialize obstacles
    numObstacles = 0; // Reset obstacles count
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
    // Generate a random position for the apple
    do {
        apple.x = rand() % (SCREEN_WIDTH - 2) + 1;
        apple.y = rand() % (SCREEN_HEIGHT - 2) + 1;
    } while (isPositionOccupied(apple.x, apple.y)); // Ensure the apple doesn't spawn on the snake or obstacles

    // Draw the apple
    screenGotoxy(apple.x, apple.y);
    printf("A");
}

int isPositionOccupied(int x, int y) {
    // Check if the position is occupied by the snake or obstacles
    for (int i = 0; i < snakeLength; i++) {
        if (snake[i].x == x && snake[i].y == y) {
            return 1; // Position occupied by snake
        }
    }
    for (int i = 0; i < numObstacles; i++) {
        if (obstacles[i].x == x && obstacles[i].y == y) {
            return 1; // Position occupied by obstacle
        }
    }
    return 0; // Position is free
}

void spawnObstacle() {
    if (numObstacles < MAX_OBSTACLES) {
        Point newObstacle;
        do {
            newObstacle.x = rand() % (SCREEN_WIDTH - 2) + 1;
            newObstacle.y = rand() % (SCREEN_HEIGHT - 2) + 1;
        } while (isPositionOccupied(newObstacle.x, newObstacle.y)); // Ensure it doesn't spawn on the snake or existing obstacles

        obstacles[numObstacles] = newObstacle;
        numObstacles++;
    }
}

void updateGame() {
    if (gameOver) return;

    // Move the snake
    Point newHead = snake[0];

    if (direction == 0) newHead.x++; // Move right
    else if (direction == 1) newHead.y++; // Move down
    else if (direction == 2) newHead.x--; // Move left
    else if (direction == 3) newHead.y--; // Move up

    // Check for wall collisions
    if (newHead.x <= 0 || newHead.x >= SCREEN_WIDTH || newHead.y <= 0 || newHead.y >= SCREEN_HEIGHT) {
        gameOver = 1; // Set game over
        return;
    }

    // Check for self-collision
    for (int i = 0; i < snakeLength; i++) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            gameOver = 1; // Set game over
            return;
        }
    }

    // Check for obstacle collisions
    for (int i = 0; i < numObstacles; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1; // Set game over
            return;
        }
    }

    // Check for apple collision
    if (newHead.x == apple.x && newHead.y == apple.y) {
        // Grow the snake
        snakeLength++;
        applesEaten++;

        // Place a new apple
        spawnApple();

        // Generate a new obstacle based on difficulty
        if ((difficulty == 0 && applesEaten % 3 == 0) || // Fácil
            (difficulty == 1 && applesEaten % 2 == 0) || // Médio
            (difficulty == 2 && applesEaten % 1 == 0)) { // Difícil
            spawnObstacle();
        }
    } else {
        // Move the snake
        clearPosition(snake[snakeLength - 1].x, snake[snakeLength - 1].y); // Clear tail
    }

    // Shift the snake's body
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Update the head of the snake
    snake[0] = newHead;

    // Draw the snake, apple, and obstacles
    drawSnake();
    drawApple();
    drawObstacles();
}

void changeDirection(int newDirection) {
    // Prevent reversing direction
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

    while (ch != 10 && !gameOver) { // Enter key to exit
        // Handle user input
        if (keyhit()) {
            ch = readch();
            if (ch == 'w') changeDirection(3); // Up
            if (ch == 's') changeDirection(1); // Down
            if (ch == 'a') changeDirection(2); // Left
            if (ch == 'd') changeDirection(0); // Right
        }

        // Update game state
        if (timerTimeOver() == 1) {
            updateGame();
            screenUpdate();
        }
    }

    // Game over
    screenGotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
    printf("Game Over!");

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
