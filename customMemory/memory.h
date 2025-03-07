#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>

// Struct definitions
typedef struct {
    int A;
    int B;
    int *firstElement;
} tuple;

typedef struct {
    tuple *data;
    int size;
    int capacity;
} ArrayList;

typedef struct {
    int max_size;
    int *memory; // representing each cell
    int count;   // current count
    ArrayList *free_tuples;
    ArrayList *allocated_tuples;
} resource;

// Function declarations
void is_seen();
void printArrayList(ArrayList *list, const char *listName);
void printResource(resource *res);
ArrayList* createArrayList();
void resizeArrayList(ArrayList *list);
void addElement(ArrayList *list, tuple value);
void removeElement(ArrayList *list, int index);
void freeArrayList(ArrayList *list);
int *customMalloc(resource *r1,int size);
resource* newAllocator(int max_size);

#endif // MEMORY_H
