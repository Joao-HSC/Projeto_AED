#include <stdio.h>
#include <stdlib.h>
#include "tiles.h"
#include "dfs.h"

/*
* Stack for storing Coordinates
*/
struct Stack {
    struct Coordinates* array;
    int capacity;
    int top;
};

struct Stack* newStack(int capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (struct Coordinates*)malloc(capacity * sizeof(struct Coordinates));
    return stack;
}

// Adds a new element to the stack if there is enough space
void push(struct Stack* stack, struct Coordinates value) {
    if (stack->top < stack->capacity - 1) {
        stack->array[++stack->top] = value;
    }
}

// Checks if the stack is empty or not
int is_empty(struct Stack* stack) {
    return stack->top == -1;
}

struct Coordinates pop(struct Stack* stack) {
    if (!is_empty(stack)) {
        return stack->array[stack->top--];
    } 
    
    else {
        exit(1);
    }
}

