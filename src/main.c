#include <stdlib.h>  // Para rand() e srand()
#include <time.h>    // Para time()
#include "game.h"
#include "highscore.h"

int main() {
    int ch = 0;

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

    saveHighScore();

    screenGotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
    printf("Game Over!");

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
