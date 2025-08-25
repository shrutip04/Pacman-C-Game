#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  
#include <termios.h>
#include <unistd.h>

    int getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }


// Game constants
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'

// Global variables
int pacman_x, pacman_y;
int score = 0;
int res = 0;
int food = 0;
int curr = 0;
char board[HEIGHT][WIDTH];

// Function prototypes
void initialize();
void draw();
void move(int move_x, int move_y);

// Function to initialize the game board
void initialize() {
    srand(time(NULL));  // Seed random generator

    // Set up the game board with walls and empty spaces
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1) {
                board[i][j] = WALL;  // Outer walls
            } else {
                board[i][j] = EMPTY;  // Empty spaces
            }
        }
    }

    // Randomly place internal walls
    int count = 50;
    while (count > 0) {
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        if (board[i][j] == EMPTY) {
            board[i][j] = WALL;
            count--;
        }
    }

    // Add some horizontal barriers
    int val = 5;
    while (val--) {
        int row = rand() % HEIGHT;
        for (int j = 3; j < WIDTH - 3; j++) {
            if (board[row][j] == EMPTY) {
                board[row][j] = WALL;
            }
        }
    }

    // Place demons randomly
    count = 10;
    while (count > 0) {
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        if (board[i][j] == EMPTY) {
            board[i][j] = DEMON;
            count--;
        }
    }

    // Place Pacman at the center
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;

    // Place food items
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            if (i % 2 == 0 && j % 2 == 0 && board[i][j] == EMPTY) {
                board[i][j] = FOOD;
                food++;
            }
        }
    }
}

// Function to show the game screen
void draw() {
    
        system("cls");  // clears screen
    

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

// Function handles Pacman movement
void move(int move_x, int move_y) {
    int new_x = pacman_x + move_x;
    int new_y = pacman_y + move_y;

    if (board[new_y][new_x] == WALL) {
        return;  // Pacman cannot move into walls
    }

    if (board[new_y][new_x] == FOOD) {
        score++;
        food--;
        curr++;
    } else if (board[new_y][new_x] == DEMON) {
        res = 1;
        return;
    }

    board[pacman_y][pacman_x] = EMPTY;  // Clear old position
    pacman_x = new_x;
    pacman_y = new_y;
    board[pacman_y][pacman_x] = PACMAN;  // Update new position
}

// Main game loop
int main() {
    initialize();
    char ch;
    int totalFood = food;

    printf("Controls: W (Up), A (Left), S (Down), D (Right), Q (Quit)\n");
    printf("Press Y to start: ");
    ch = getch();
    if (ch != 'Y' && ch != 'y') {
        printf("Game exited!\n");
        return 0;
    }

    while (1) {
        draw();
        printf("Total Food: %d | Eaten: %d\n", totalFood, curr);

        if (res == 1) {
            printf("Game Over! You were caught by a Demon!\nYour Score: %d\n", score);
            return 1;
        }

        if (food == 0) {
            printf("You Win! Your Score: %d\n", score);
            return 1;
        }

        ch = getch();
        switch (ch) {
            case 'w': move(0, -1); break;
            case 's': move(0, 1); break;
            case 'a': move(-1, 0); break;
            case 'd': move(1, 0); break;
            case 'q': printf("Game Over! Your Score: %d\n", score); return 0;
        }
    }

    return 0;
}
