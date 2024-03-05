#include <stdio.h>
#include <pthread.h>

// Define the size of the Sudoku grid and the total number of threads
#define SIZE 9
#define THREAD_COUNT 28

// Declare the Sudoku grid and a variable to track validity
int sudoku[SIZE][SIZE];
int valid = 0;

// Function to check if a row in the Sudoku grid is valid
void *check_row(void *arg) {
    // Extract the row number from the argument
    // Cast the void pointer to an int pointer and dereference it to assign the value
    /* Prompts to help with this: https://chat.openai.com/share/97cf5838-b2b4-4980-8ead-082d0ccc0739*/
    int row = *(int *)arg;

    // Array to keep track of the occurrences of digits in the row
    int check[SIZE] = {0};

    // Iterate through each column in the row
    for (int i = 0; i < SIZE; i++) {
        // Get the number at the current cell
        int num = sudoku[row][i];

        // Check if the number has already occurred in the row
        if (check[num - 1] == 1) {
            // Print error message if the number is repeated in the row
            printf("Thread #%2d (row %d) is invalid\n", row + 1, row + 1);

            // Set the validity flag to indicate an invalid solution
            valid = -1;

            // Exit the thread
            // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
            pthread_exit(NULL);
        }

        // Mark the number as occurred in the row
        check[num - 1] = 1;
    }

    // Print message indicating the row is valid
    printf("Thread #%2d (row %d) is valid\n", row + 1, row + 1);

    // Exit the thread
    // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
    pthread_exit(NULL);
}

// Function to check if a column in the Sudoku grid is valid
void *check_col(void *arg) {
    // Extract the column number from the argument
    // Cast the void pointer to an int pointer and dereference it to assign the value
    /* Prompts to help with this: https://chat.openai.com/share/97cf5838-b2b4-4980-8ead-082d0ccc0739*/
    int col = *((int *)arg);

    // Array to keep track of the occurrences of digits in the column
    int check[SIZE] = {0};

    // Iterate through each row in the column
    for (int i = 0; i < SIZE; i++) {
        // Get the number at the current cell
        int num = sudoku[i][col];

        // Check if the number has already occurred in the column
        if (check[num - 1] == 1) {
            // Print error message if the number is repeated in the column
            printf("Thread #%2d (column %d) is invalid\n", col + 10, col + 1);

            // Set the validity flag to indicate an invalid solution
            valid = -1;

            // Exit the thread
            // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
            pthread_exit(NULL);
        }

        // Mark the number as occurred in the column
        check[num - 1] = 1;
    }

    // Print message indicating the column is valid
    printf("Thread #%2d (column %d) is valid\n", col + 10, col + 1);

    // Exit the thread
    // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
    pthread_exit(NULL);
}

// Function to check if a 3x3 subgrid in the Sudoku grid is valid
void *check_subgrid(void *arg) {
    // Calculate the starting row and column of the subgrid
    // Cast the void pointer to an int pointer and dereference it to assign the value
    /* Prompts to help with this: https://chat.openai.com/share/97cf5838-b2b4-4980-8ead-082d0ccc0739*/
    int start_row = *((int *)arg) / 3 * 3;
    int start_col = *((int *)arg) % 3 * 3;

    // Array to keep track of the occurrences of digits in the subgrid
    int check[SIZE] = {0};

    // Iterate through each cell in the subgrid
    for (int i = start_row; i < start_row + 3; i++) {
        for (int j = start_col; j < start_col + 3; j++) {
            // Get the number at the current cell
            int num = sudoku[i][j];

            // Check if the number has already occurred in the subgrid
            if (check[num - 1] == 1) {
                // Print error message if the number is repeated in the subgrid
                printf("Thread #%2d (subgrid %d) is invalid\n", *((int *)arg) + 19, *((int *)arg) + 1);

                // Set the validity flag to indicate an invalid solution
                valid = -1;

                // Exit the thread
                // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
                pthread_exit(NULL);
            }

            // Mark the number as occurred in the subgrid
            check[num - 1] = 1;
        }
    }

    // Print message indicating the subgrid is valid
    printf("Thread #%2d (subgrid %d) is valid\n", *((int *)arg) + 19, *((int *)arg) + 1);

    // Exit the thread
    // Prompts to help with this: https://chat.openai.com/share/440224e8-48dd-4b9d-ae34-5ea38af45f05
    pthread_exit(NULL);
}

// Main function
int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    // Open the file containing the Sudoku grid
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read the Sudoku grid from the file
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }
    fclose(file);

    // Declare an array to hold thread IDs and thread arguments
    pthread_t threads[THREAD_COUNT];
    int thread_args[SIZE];

    // Create threads for checking rows
    for (int i = 0; i < SIZE; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, check_row, &thread_args[i]);
    }

    // Wait for all row threads to complete
    for (int i = 0; i < THREAD_COUNT/3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Create threads for checking columns
    for (int i = 0; i < SIZE; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i + SIZE], NULL, check_col, &thread_args[i]);
    }

    // Wait for all column threads to complete
    for (int i = 0; i < THREAD_COUNT/3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Create threads for checking subgrids
    for (int i = 0; i < SIZE; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i + 2 * SIZE], NULL, check_subgrid, &thread_args[i]);
    }

    // Wait for all subgrid threads to complete
    for (int i = 0; i < THREAD_COUNT/3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print whether the Sudoku solution is valid or invalid
    if (valid == 0) {
        printf("\n%s contains a valid solution\n", argv[1]);
    } else {
        printf("\n%s contains an INVALID solution\n", argv[1]);
    }

    return 0;
}
