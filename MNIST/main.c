#include <stdio.h>
#include <stdlib.h>
#include "matrix.c"

#define HEIGHT 28
#define WIDTH 28

// Function to parse the image file and store it in a dynamically allocated array
void parseImage(const char *filename, unsigned char **pixels) {
    FILE *file = fopen(filename, "rb"); // Open in binary mode
    if (!file) {
        perror("Unable to open file");
        exit(1);
    }

    // Read all pixel values (784 bytes) into the array
    size_t bytesRead = fread(*pixels, sizeof(unsigned char), WIDTH * HEIGHT, file);
    if (bytesRead != WIDTH * HEIGHT) {
        printf("bytes read: %ld\n", bytesRead);
        perror("Error reading file");
        exit(1);
    }

    fclose(file);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%3d ", (*pixels)[i * WIDTH + j]);  // Access pixel using flat index
        }
        printf("\n");
    }
    printf("Size : %d\n",HEIGHT * WIDTH);
}

int main() {
    // Dynamically allocate memory for the pixel array
    unsigned char *pixels = (unsigned char*)malloc(HEIGHT * WIDTH * sizeof(unsigned char));
    if (!pixels) {
        perror("Unable to allocate memory");
        exit(1);
    }

    // Parse the image and store it in the allocated memory
    parseImage("archive/train-images.idx3-ubyte", &pixels);

    // Don't forget to free the memory after you're done using it
    free(pixels);

    return 0;
}
