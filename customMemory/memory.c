#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
void is_seen(){
    printf("hello from memory.c\n");
}
void printArrayList(ArrayList *list, const char *listName) {
    if (list == NULL) {
        printf("%s is NULL\n", listName);
        return;
    }
    printf("%s (size: %d, capacity: %d): [", listName, list->size, list->capacity);
    for (int i = 0; i < list->size; i++) {
        printf("A:%d, B:%d, Ptr %p", list->data[i].A,list->data[i].B,list->data->firstElement);
        if (i < list->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
void printResource(resource *res) {
    if (res == NULL) {
        printf("Resource is NULL\n");
        return;
    }

    printf("Resource Details:\n");
    printf("Max Size: %d\n", res->max_size);
    printf("Current Count: %d\n", res->count);

    // Print memory array
    printf("Memory: [");
    for (int i = 0; i < res->max_size; i++) {
        printf("%d", res->memory[i]);
        if (i < res->max_size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // Print ArrayLists
    printArrayList(res->free_tuples, "Free Tuples");
    printArrayList(res->allocated_tuples, "Allocated Tuples");
}


// Initialize a new ArrayList
ArrayList* createArrayList() {
    ArrayList *list = (ArrayList*) malloc(sizeof(ArrayList));
    list->size = 0;
    list->capacity = 10;
    list->data = (tuple*) malloc(list->capacity * sizeof(tuple));
    return list;
}

// Resize the array (double capacity)
void resizeArrayList(ArrayList *list) {
    list->capacity *= 2;
    list->data = (tuple*) realloc(list->data, list->capacity * sizeof(int));
}

// Add an element to the ArrayList
void addElement(ArrayList *list, tuple value) {
    if (list->size == list->capacity) {
        resizeArrayList(list);
    }
    list->data[list->size++] = value;
}
void removeElement(ArrayList *list, int index) {
    if (index < 0 || index >= list->size) {
        return;
    }
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
}
// Get an element by index

// Clear the ArrayList
// Free memory allocated for the ArrayList
void freeArrayList(ArrayList *list) {
    free(list->data);
    free(list);
}


// pointer to start of memory
// [0|0|0|0|0|0|0|0|0|0|0|0]
//    T T T T T
// represent taken as [1:5] and free as [0] ,[6:11]
/*
1. mabey have two arrays. one allocates memory as an api but internally also mark which spaces are free 
    at init array of size max_size is allocated. all elements are set to 0 and a global counter is set and it begins at 1
    each time malloc is called increment the counter by 1 and then attempt to allocate the number of cells asked for
    mark this in the secondary memory (set to -1) and return a pointer that points to an array of the sized in bytes asked for by the caller
    method returns the pointer 
    free:
        take in the pointer returned by malloc. go to pointer and keep traversing until you encounter a value this isnt the same as the starting element
        at the same time traverse through the internal array and de-allocate(mark as 0 again)

2. store taken memory as a pair of tuples. (1,5) this represent what cells they are occupyin.
    this has advantages because we can compute the size of allocated space easily ((b-a)+1)
*/
resource *newAllocator(int max_size){
    resource r1;
    r1.max_size = max_size;
    r1.memory = (int *)calloc(max_size, sizeof(int));
    r1.count = 1;
    //set each element to 0 to start off with
    // following should be a dynamicly sized list of tuples need to implement
    r1.free_tuples = createArrayList();
    
    tuple start;
    start.A = 0;
    start.B= max_size-1;
    start.firstElement= r1.memory;
    addElement(r1.free_tuples,start);
    
    r1.allocated_tuples = createArrayList();
    printResource(&r1);
    
};

// find first open position, insert there and mark it by setting internal array to current count , update tuple to not include just taken memory
// increment count for next malloc call and return pointer to the start
int *customMalloc(resource *r1, int size){
    // exit early otherwise we dont have enough space
    printf("2");
    for (int i = 0;i<r1->free_tuples->size;i++){
        tuple *t1 =  &r1->free_tuples->data[i];
        printf("2%d",i);
        if ((t1->B - t1->A) + 1 >= size){
            int newStart = t1->A;
            int iterations = 0;
            for (int j = t1->A;iterations< size;iterations++){
                r1->memory[j] = r1->count;
            }
            t1->A += size;
            if (t1->A > t1->B) {
                removeElement(r1->free_tuples, i);
            }
            r1->count++;
            return &r1->memory[newStart];
        }else{
            continue;
        }
    }
    return NULL;
}