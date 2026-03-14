#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 20
#define ENEMIES 5

int playerX;
int enemyX[ENEMIES], enemyY[ENEMIES];
int bulletX = -1, bulletY = -1;
int score = 0;
int gameOver = 0;
int direction = 1; // enemy movement direction

// Hide cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Set cursor position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Initialize game
void setup() {
    playerX = WIDTH / 2;

    for (int i = 0; i < ENEMIES; i++) {
        enemyX[i] = 5 + i * 6;
        enemyY[i] = 2;
    }
}

// Draw everything
void draw() {
    system("cls");

    // Draw top border
    for (int i = 0; i < WIDTH; i++)
        printf("-");
    printf("\n");

    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            int printed = 0;

            // Player
            if (y == HEIGHT - 1 && x == playerX) {
                printf("^");
                printed = 1;
            }

            // Enemies
            for (int i = 0; i < ENEMIES; i++) {
                if (x == enemyX[i] && y == enemyY[i]) {
                    printf("W");
                    printed = 1;
                }
            }

            // Bullet
            if (x == bulletX && y == bulletY) {
                printf("|");
                printed = 1;
            }

            if (!printed)
                printf(" ");
        }
        printf("\n");
    }

    printf("\nScore: %d\n", score);
}

// Input handling
void input() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 'a' && playerX > 0)
            playerX--;
        if (key == 'd' && playerX < WIDTH - 1)
            playerX++;
        if (key == ' ' && bulletY == -1) {
            bulletX = playerX;
            bulletY = HEIGHT - 2;
        }
    }
}

// Move enemies
void moveEnemies() {
    int changeDirection = 0;

    for (int i = 0; i < ENEMIES; i++) {
        enemyX[i] += direction;

        if (enemyX[i] <= 0 || enemyX[i] >= WIDTH - 1)
            changeDirection = 1;
    }

    if (changeDirection) {
        direction *= -1;
        for (int i = 0; i < ENEMIES; i++)
            enemyY[i]++;
    }

    // Check game over
    for (int i = 0; i < ENEMIES; i++) {
        if (enemyY[i] >= HEIGHT - 1)
            gameOver = 1;
    }
}

// Move bullet
void moveBullet() {
    if (bulletY != -1) {
        bulletY--;

        if (bulletY < 0) {
            bulletY = -1;
        }
    }
}

// Collision detection
void checkCollision() {
    for (int i = 0; i < ENEMIES; i++) {
        if (bulletX == enemyX[i] && bulletY == enemyY[i]) {
            score += 10;
            bulletY = -1;

            // Reset enemy
            enemyX[i] = rand() % (WIDTH - 2) + 1;
            enemyY[i] = 2;
        }
    }
}

// Main function
int main() {
    hideCursor();
    setup();

    while (!gameOver) {
        draw();
        input();
        moveEnemies();
        moveBullet();
        checkCollision();

        Sleep(80);
    }

    system("cls");
    printf("\n\n      GAME OVER!\n");
    printf("      Final Score: %d\n", score);

    return 0;
}