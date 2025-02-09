// first comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dimensions{
    int rows;
    int cols;
};
int sizecsv(FILE *file, struct Dimensions *obj) {
    char line[1024];
    int rows = 0;
    int columns = 0;
    int flag = 0;
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            token = strtok(NULL, ",");
            if (flag == 0){
            columns++;
            }
        }
        if (flag == 0){
            flag++;
        };
        rows++;
    }
    obj->cols = columns;
    obj->rows = rows;
}
int** create_matrix(int m, int n) {
    // Allocate memory for m rows (array of int pointers)
    int** matrix = (int**)malloc(m * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        return NULL; // Exit if memory allocation fails
    }

    // Allocate memory for n columns in each row
    for (int i = 0; i < m; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns in row %d.\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}
void free_matrix(int** matrix, int m) {
     if (matrix != NULL){
        for (int i = 0; i < m; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

void populate_matrix_from_csv(FILE *ptr , int** matrix, int rows, int cols){
    char line[1024];
    int rowIndex = 0;
    while (rowIndex < rows && fgets(line, sizeof(line), ptr) != NULL) {
        line[strcspn(line, "\r\n")] = '\0'; 

        int colIndex = 0;
        char *token = strtok(line, ",");
        
        // Read each token (column)
        while (colIndex < cols && token != NULL) {
            // Convert the token to an integer
            int value = atoi(token);

            // Store in the matrix
            matrix[rowIndex][colIndex] = value;

            // Move to the next token
            token = strtok(NULL, ",");
            colIndex++;
        }

        // Move to the next row
        rowIndex++;
    }
}
void print_matrix(int rows, int cols, int** matrix) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}
int min(int a, int b) {
  return (a < b) ? a : b;
}

// Returns the maximum of two integers.
int max(int a, int b) {
  return (a > b) ? a : b;
}

double RowMean(int** matrix,int row,int cols){
    int sum = 0;
    for (int c=0;c<cols;c++){
        sum += matrix[row][c];
    }
    return sum/(double)cols;
};
// given the row return the max in that row.
int RowMax(int** matrix, int row, int cols) {
    // Start with the first element of the row
    int maxVal = matrix[row][0];  
    
    // Iterate through the row
    for (int c = 1; c < cols; c++) {
        if (matrix[row][c] > maxVal) {
            maxVal = matrix[row][c];
        }
    }
    return maxVal;
}
int RowMin(int** matrix, int row, int cols) {
    // Start with the first element of the row
    int minVal = matrix[row][0];
    
    // Iterate through the row
    for (int c = 1; c < cols; c++) {
        if (matrix[row][c] < minVal) {
            minVal = matrix[row][c];
        }
    }
    return minVal;
}
double ColMean(int** matrix, int col, int rows) {
    int sum = 0;
    for (int r = 0; r < rows; r++) {
        sum += matrix[r][col];
    }
    // Simple integer division
    return sum / (double)rows;
}

// Return the maximum value in a given column
int ColMax(int** matrix, int col, int rows) {
    int maxVal = matrix[0][col];
    for (int r = 1; r < rows; r++) {
        if (matrix[r][col] > maxVal) {
            maxVal = matrix[r][col];
        }
    }
    return maxVal;
}

// Return the minimum value in a given column
int ColMin(int** matrix, int col, int rows) {
    int minVal = matrix[0][col];
    for (int r = 1; r < rows; r++) {
        if (matrix[r][col] < minVal) {
            minVal = matrix[r][col];
        }
    }
    return minVal;
}

void formatOutput(char *file , char *plane, double mean, int max, int min){
    printf("%s %s %.2f %d %d\n",file,plane,mean,max,min);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <filename.csv> <r|c> <index>\n", argv[0]);
        return -1;
    }
    FILE *file = fopen(argv[1], "r"); // file passed in 
    if (file == NULL) {
       // perror("Error opening file");
        //fprintf(stderr, "exit with status -1\n");  // or a more specific message
        return -1;
    }
    // dir should be a single character: 'r' or 'c'
    char dir = argv[2][0];   
    // plane is 0-based index for row or column
    int plane = atoi(argv[3]);

    struct Dimensions d1;
    sizecsv(file,&d1);
    int** my_matrix = create_matrix(d1.rows, d1.cols);
    rewind(file); // Moves the file position indicator back to the beginning
    populate_matrix_from_csv(file, my_matrix, d1.rows, d1.cols);
    // read through csv file again now and assighn values to each row and repeat for # of columns 
    if (dir == 'r') {
        if (plane < 0 || plane >= d1.rows) {
            fprintf(stderr, "error in input format at line %d\n", plane);
            free_matrix(my_matrix, d1.rows);
            fclose(file);
            return -1;
        } else {
            // It's a valid row
            int maxVal  = RowMax(my_matrix, plane, d1.cols);
            int minVal  = RowMin(my_matrix, plane, d1.cols);
            double meanVal = RowMean(my_matrix, plane, d1.cols);
            formatOutput(argv[1], "row", meanVal, maxVal, minVal);
        }
    } 
    else if (dir == 'c') {
        if (plane < 0 || plane >= d1.cols) {
            fprintf(stderr, "error in input format at line %d\n", plane);
            free_matrix(my_matrix, d1.rows);
            fclose(file);
            return -1;
        } else {
            // It's a valid column
            int maxVal  = ColMax(my_matrix, plane, d1.rows);
            int minVal  = ColMin(my_matrix, plane, d1.rows);
            double meanVal = ColMean(my_matrix, plane, d1.rows);
            formatOutput(argv[1], "column", meanVal, maxVal, minVal);
        }
    } 
    else {
        // If the second argument isn't 'r' or 'c'
        free_matrix(my_matrix, d1.rows);
        fclose(file);
        fprintf(stderr, "error in input format at line 1\n");
        return -1;
    }
    fclose(file);
    free_matrix(my_matrix,d1.rows);
    return 0;
}