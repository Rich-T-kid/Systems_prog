#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void greet(){
    printf("Hello world -> from matrix.c\n");
}
// all of these functions assume matrixes are square (MNIST is)
int** add_matrix(unsigned char **matrix_1 , unsigned char **matrix_2, int rows, int cols);
int** multiply_matrix(unsigned char **matrix_1 , unsigned char **matrix_2, int rows, int cols); 
int** apply_functon_matrix(unsigned char **matrix , int rows, int cols,unsigned char (*operation)(unsigned char** matrix) ); // example function here is the sigmoid function (0-1) // or Relu --> Max(0,a)
//Returns the resulting matrix to the caller it will be of size R x C 
void print_matrix(unsigned char **matrix ,int rows, int cols){
    for (int i = 0;i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%4d",matrix[i][j]); // right aligned with 4 spaces
        }
        printf("\n");
    }
    printf("size: %d\n",rows * cols);
}

void generate_matrix(int rows, int cols,unsigned char  **empty_matrix){
    //allocate rows
    empty_matrix = malloc(rows * sizeof(unsigned char *));
    // Allocate memory for each column in each row
  for (int i = 0; i < rows; i++) {
    empty_matrix[i] = (unsigned char *)malloc(cols * sizeof(int));
    if (empty_matrix[i] == NULL) {
      printf("Memory allocation failed for column %d.\n", i);
      // Free previously allocated memory
      for (int j = 0; j < i; j++) {
        free(empty_matrix[j]);
      }
      free(empty_matrix);
      return;
    }
  }
}

double sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}
unsigned char relU(unsigned char x) {
   if (x <= 0 ){
    return 0;
   }
   return x;
}