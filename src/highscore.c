#include "highscore.h"
#include "game.h"

void loadHighScores() {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    if (file != NULL) {
        for (int i = 0; i < 3; i++) {
            fscanf(file, "%d", &highScores[i]);
        }
        fclose(file);
    }
}

void saveHighScore() {
    if (score > highScores[difficulty]) {
        highScores[difficulty] = score;
    }

    FILE *file = fopen(HIGHSCORE_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < 3; i++) {
            fprintf(file, "%d\n", highScores[i]);
        }
        fclose(file);
    }
}
