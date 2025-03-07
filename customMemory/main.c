#include <stdio.h>
#include "memory.h"

int main() {
    resource *t = newAllocator(5);
    printf("-> 0");
    int *err = customMalloc(t,3);
    printf("-> 1");
    if (err == NULL){
      printf("-> 2");
      printf("Program didnt have enough space\n");
      return 0;
    }
    printf("-> 3");
    printResource(t);
    printf("Hello, world!\n");
    return 0;
}
