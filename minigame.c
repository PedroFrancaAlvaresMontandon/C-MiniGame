#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 15
#define COLS 30
#define MAX_LIFE 3
#define MAX_BOMBS 3
#define MAX_ENERGY 450

// Function prototypes
void initialize_board(char board[ROWS][COLS], char display_board[ROWS][COLS], int *x, int *y, int *exit_x, int *exit_y);
void print_board(char display_board[ROWS][COLS], int x, int y);
void game_loop(char board[ROWS][COLS], char display_board[ROWS][COLS], int *lifepoints, int *bombs, int *x, int *y, int *energy, int exit_x, int exit_y);
void move(int *x, int *y, int direction, char display_board[ROWS][COLS], char board[ROWS][COLS]);
void safety_check(char board[ROWS][COLS], char display_board[ROWS][COLS], int x, int y);
void neutralize_zones(char board[ROWS][COLS], char display_board[ROWS][COLS], int x, int y, int *bombs);
void clear_input_buffer();

// Main function
int main() {
    srand(time(0));

    char board[ROWS][COLS];            // Actual gameboard with all information
    char display_board[ROWS][COLS];     // What is shown to the user (unexplored zones hidden)
    int lifepoints = MAX_LIFE;
    int bombs = MAX_BOMBS;
    int energy = MAX_ENERGY;
    int x, y, exit_x, exit_y;

    initialize_board(board, display_board, &x, &y, &exit_x, &exit_y);
    game_loop(board, display_board, &lifepoints, &bombs, &x, &y, &energy, exit_x, exit_y);

    return 0;
}

// Initialize the game board and the display board
void initialize_board(char board[ROWS][COLS], char display_board[ROWS][COLS], int *x, int *y, int *exit_x, int *exit_y) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (rand() % 100 < 20) {  // 20% chance of unsafe zone
                board[i][j] = '!';
            } else {
                board[i][j] = ' ';
            }
            display_board[i][j] = ' ';  // Unexplored zones shown as blank spaces initially
        }
    }
    
    // Place the character at a random starting position
    *x = rand() % ROWS;
    *y = rand() % COLS;
    board[*x][*y] = 'T';
    display_board[*x][*y] = 'T';  // Initial position is shown on the display board

    // Place the exit at a different random position
    do {
        *exit_x = rand() % ROWS;
        *exit_y = rand() % COLS;
    } while (*exit_x == *x && *exit_y == *y);
    
    board[*exit_x][*exit_y] = 'X';  // Exit placed on the actual board, but not revealed yet
}

// Print the current state of the display board (only show explored zones)
void print_board(char display_board[ROWS][COLS], int x, int y) {
    printf("\nGameboard:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", display_board[i][j]);
        }
        printf("\n");
    }
}

// Recursive game loop
void game_loop(char board[ROWS][COLS], char display_board[ROWS][COLS], int *lifepoints, int *bombs, int *x, int *y, int *energy, int exit_x, int exit_y) {
    // Base case: check if user has won or lost
    if (*lifepoints <= 0) {
        printf("You lost! No more lifepoints left.\n");
        return;
    }
    if (*x == exit_x && *y == exit_y) {
        printf("Congratulations! You found the exit and won!\n");
        return;
    }

    // Print current board state
    print_board(display_board, *x, *y);
    printf("Lifepoints: %d, Bombs: %d, Energy: %d\n", *lifepoints, *bombs, *energy);

    // Present user with possible actions
    int action;
    printf("Choose an action (1: Move, 2: Safety Check, 3: Use Bomb): ");
    
    // Validate action input (must be a number between 1 and 3)
    while (scanf("%d", &action) != 1 || action < 1 || action > 3) {
        printf("Invalid action! Please enter a number between 1 and 3.\n");
        clear_input_buffer();  // Clear invalid input
    }

    // Store the previous position of the player
    int prev_x = *x;
    int prev_y = *y;

    switch (action) {
        case 1: {
            // Move the character
            int direction;
            printf("Choose direction (1: Up, 2: Down, 3: Left, 4: Right): ");
            
            // Validate movement direction input (must be a number between 1 and 4)
            while (scanf("%d", &direction) != 1 || direction < 1 || direction > 4) {
                printf("Invalid direction! Please enter a number between 1 and 4.\n");
                clear_input_buffer();  // Clear invalid input
            }
            
            move(x, y, direction, display_board, board);

            // Check if moved to an unsafe zone
            if (board[*x][*y] == '!') {
                printf("You moved to an unsafe zone! Lost 1 lifepoint.\n");
                (*lifepoints)--;
                display_board[*x][*y] = '#';  // Mark unsafe zone as hit
                // Revert back to the previous position
                *x = prev_x;
                *y = prev_y;
                display_board[*x][*y] = 'T';  // Keep the player in the previous position
            } else {
                display_board[prev_x][prev_y] = 'E';  // Mark previous position as explored
                display_board[*x][*y] = 'T';  // Mark zone as current position
            }
            break;
        }
        case 2:
            // Perform a safety check
            safety_check(board, display_board, *x, *y);
            break;
        case 3:
            // Use a bomb to neutralize neighboring zones
            if (*bombs > 0) {
                neutralize_zones(board, display_board, *x, *y, bombs);
            } else {
                printf("No bombs left!\n");
            }
            break;
        default:
            printf("Invalid action! Try again.\n");
            break;
    }

    // Reduce energy after each action, or lose a lifepoint if no energy left
    if (*energy > 0) {
        (*energy)--;
    } else {
        (*lifepoints)--;
        printf("Out of energy! Lost 1 lifepoint.\n");
    }

    // Recursive call to continue the game loop
    game_loop(board, display_board, lifepoints, bombs, x, y, energy, exit_x, exit_y);
}

// Handle character movement with edge wrapping and store number of safe zones in the previous position
void move(int *x, int *y, int direction, char display_board[ROWS][COLS], char board[ROWS][COLS]) {
    // Movement logic remains the same, no need for prev_x or prev_y here anymore

    switch (direction) {
        case 1:  // Move up
            *x = (*x == 0) ? ROWS - 1 : *x - 1;
            break;
        case 2:  // Move down
            *x = (*x == ROWS - 1) ? 0 : *x + 1;
            break;
        case 3:  // Move left
            *y = (*y == 0) ? COLS - 1 : *y - 1;
            break;
        case 4:  // Move right
            *y = (*y == COLS - 1) ? 0 : *y + 1;
            break;
        default:
            printf("Invalid direction!\n");
            break;
    }
}

// Perform a safety check to count safe neighboring zones
void safety_check(char board[ROWS][COLS], char display_board[ROWS][COLS], int x, int y) {
    int safe_zones = 0;

    // Check the four orthogonal directions (up, down, left, right)
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = (x + dx[i] + ROWS) % ROWS;
        int ny = (y + dy[i] + COLS) % COLS;

        if (board[nx][ny] != '!' && board[nx][ny] != '#') {
            safe_zones++;
        }
    }

    // Replace the 'T' with the number of safe zones immediately
    display_board[x][y] = '0' + safe_zones;
    board[x][y] = display_board[x][y];  // Keep the board consistent
    printf("Safety Check: %d safe zones.\n", safe_zones);
}

// Neutralize neighboring zones using a bomb, but don't affect positions with numbers or 'T'
void neutralize_zones(char board[ROWS][COLS], char display_board[ROWS][COLS], int x, int y, int *bombs) {
    printf("Using a bomb to neutralize neighboring zones!\n");
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            int row = (i + ROWS) % ROWS;
            int col = (j + COLS) % COLS;
            
            // Don't affect positions with numbers (result of safety check) or the current position 'T'
            if (display_board[row][col] != 'T' && (display_board[row][col] < '1' || display_board[row][col] > '4')) {
                if (board[row][col] == '!') {
                    display_board[row][col] = '!';  // Unsafe zones remain as '!'
                } else if (board[row][col] == 'X') {
                    display_board[row][col] = 'X';  // Show exit if it's adjacent
                } else {
                    display_board[row][col] = 'E';  // Mark other zones as explored
                }
            }
        }
    }
    (*bombs)--;
}

// Function to clear the input buffer in case of invalid input
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard characters from the input buffer
    }
}
