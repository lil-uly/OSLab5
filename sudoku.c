#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9},
    {4, 5, 6, 7, 8, 9, 1, 2, 3},
    {7, 8, 9, 1, 2, 3, 4, 5, 6},
    {2, 1, 4, 3, 6, 5, 8, 9, 7},
    {3, 6, 5, 8, 9, 7, 2, 1, 4},
    {8, 9, 7, 2, 1, 4, 3, 6, 5},
    {5, 3, 1, 6, 4, 2, 9, 7, 8},
    {6, 4, 2, 9, 7, 8, 5, 3, 1},
    {9, 7, 8, 5, 3, 1, 6, 4, 2},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    int seen[SIZE];
    int n = 0;
    for(n = 0; n < SIZE; n++){
        seen[n] = 0;
    }
    int row = *((int *)args);

    for (n = 0; n < SIZE; n++){
        seen[board[n][row] - 1] += 1;
    }
    row_check[row] = true;
    for (n = 0; n < SIZE; n++){
        if(seen[n] != 1){row_check[n] = false;}
    }
    // return NULL;

}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    int seen[SIZE];
    int n = 0;
    for (n = 0; n < SIZE; n++){
        seen[n] = 0;
    }
    int col = *((int *)args);
    for (n = 0; n < SIZE; n++){seen[board[col][n] - 1] += 1;}
    col_check[col] = true;
    for(n = 0; n < SIZE; n++){if (seen[n] != 1) col_check[col] = false;}
    return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    int seen[SIZE];
    int n = 0;
    for(n = 0; n < SIZE; n++){
        seen[n] = 0;
    }
    int box = *((int *)args);
    int i = 0;
    int x = (box / 3) * 3;
    int y = (box % 3) * 3;

    for (int n = 0; n < 3; n++){
        for (int i = 0; i < 3; i++){
            seen[board[n + x][i + y] - 1] += 1;
        }
    }
    box_check[box] = true;
    for(n = 0; n < SIZE; n++){if (seen[n] != 1) {box_check[box] = false; }}
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t row_threads[SIZE];
    pthread_t col_threads[SIZE];
    pthread_t box_threads[SIZE];
    // 3. Create a thread for each cell of each matrix operation.
    int n = 0;
    for(n = 0; n < SIZE; n++){
        int *x = malloc(sizeof(int));
        int *y = malloc(sizeof(int));
        int *z = malloc(sizeof(int));
        *x = n;
        *y = n;
        *z = n;
        pthread_create(&row_threads[n], NULL, checkRow, x);
        pthread_create(&col_threads[n], NULL, checkCol, y);
        pthread_create(&box_threads[n], NULL, checkBox, z);

    }
    // 4. Wait for all threads to finish.
    for(n = 0; n < SIZE; n++){
        pthread_join(row_threads[n], NULL);
        pthread_join(col_threads[n], NULL);
        pthread_join(box_threads[n], NULL);
    }
    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

