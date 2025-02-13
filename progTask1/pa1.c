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

unsigned int** create_matrix(int m, int n) {
    unsigned int** matrix = (unsigned int**)malloc(m * sizeof(unsigned int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        return NULL; 
    }

    for (int i = 0; i < m; i++) {
        matrix[i] = (unsigned int*)malloc(n * sizeof(unsigned int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns in row %d.\n", i);
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(unsigned int** matrix, int m) {
    if (matrix != NULL){
        for (int i = 0; i < m; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

void populate_matrix_from_csv(FILE *ptr , unsigned int** matrix, int rows, int cols){
    char line[1024];
    int rowIndex = 0;
    while (rowIndex < rows && fgets(line, sizeof(line), ptr) != NULL) {
        line[strcspn(line, "\r\n")] = '\0'; 

        int colIndex = 0;
        char *token = strtok(line, ",");
        
        while (colIndex < cols && token != NULL) {
            unsigned long value = strtoul(token, NULL, 10);  
            matrix[rowIndex][colIndex] = (unsigned int)value;  

            token = strtok(NULL, ",");
            colIndex++;
        }
        rowIndex++;
    }
}

void print_matrix(int rows, int cols, unsigned int** matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%u ", matrix[i][j]);
        }
        printf("\n");
    }
}

double RowMean(unsigned int** matrix, int row, int cols){
    unsigned long long sum = 0;
    for (int c = 0; c < cols; c++){
        sum += matrix[row][c];
    }
    return sum / (double)cols;
}

unsigned int RowMax(unsigned int** matrix, int row, int cols) {
    unsigned int maxVal = matrix[row][0];  
    for (int c = 1; c < cols; c++) {
        if (matrix[row][c] > maxVal) {
            maxVal = matrix[row][c];
        }
    }
    return maxVal;
}

unsigned int RowMin(unsigned int** matrix, int row, int cols) {
    unsigned int minVal = matrix[row][0];
    for (int c = 1; c < cols; c++) {
        if (matrix[row][c] < minVal) {
            minVal = matrix[row][c];
        }
    }
    return minVal;
}

double ColMean(unsigned int** matrix, int col, int rows) {
    unsigned long long sum = 0;
    for (int r = 0; r < rows; r++) {
        sum += matrix[r][col];
    }
    return sum / (double)rows;
}

unsigned int ColMax(unsigned int** matrix, int col, int rows) {
    unsigned int maxVal = matrix[0][col];
    for (int r = 1; r < rows; r++) {
        if (matrix[r][col] > maxVal) {
            maxVal = matrix[r][col];
        }
    }
    return maxVal;
}

unsigned int ColMin(unsigned int** matrix, int col, int rows) {
    unsigned int minVal = matrix[0][col];
    for (int r = 1; r < rows; r++) {
        if (matrix[r][col] < minVal) {
            minVal = matrix[r][col];
        }
    }
    return minVal;
}

void formatOutput(char *file , char *plane, double mean, unsigned int max, unsigned int min){
    printf("%s %s %.2f %u %u\n", file, plane, mean, max, min);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <filename.csv> <r|c> <index>\n", argv[0]);
        return -1;
    }
    FILE *file = fopen(argv[1], "r"); 
    if (file == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    char dir = argv[2][0];   
    unsigned int plane = strtoul(argv[3], NULL, 10);

    struct Dimensions d1;
    sizecsv(file, &d1);
    unsigned int** my_matrix = create_matrix(d1.rows, d1.cols);
    rewind(file);
    populate_matrix_from_csv(file, my_matrix, d1.rows, d1.cols);

    if (dir == 'r') {
        if (plane >= d1.rows) {
            printf("error in input format at line %u\n", plane);
            free_matrix(my_matrix, d1.rows);
            fclose(file);
            return -1;
        } else {
            unsigned int maxVal  = RowMax(my_matrix, plane, d1.cols);
            unsigned int minVal  = RowMin(my_matrix, plane, d1.cols);
            double meanVal = RowMean(my_matrix, plane, d1.cols);
            formatOutput(argv[1], "row", meanVal, maxVal, minVal);
        }
    } 
    else if (dir == 'c') {
        if (plane >= d1.cols) {
            printf("error in input format at line %u\n", plane);
            free_matrix(my_matrix, d1.rows);
            fclose(file);
            return -1;
        } else {
            unsigned int maxVal  = ColMax(my_matrix, plane, d1.rows);
            unsigned int minVal  = ColMin(my_matrix, plane, d1.rows);
            double meanVal = ColMean(my_matrix, plane, d1.rows);
            formatOutput(argv[1], "column", meanVal, maxVal, minVal);
        }
    } 
    else {
        free_matrix(my_matrix, d1.rows);
        fclose(file);
        fprintf(stderr, "error in input format at line 1\n");
        return -1;
    }
    fclose(file);
    free_matrix(my_matrix, d1.rows);
    return 0;
}
