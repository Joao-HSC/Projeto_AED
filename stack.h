#ifndef stack_h_
#define stack_h_
#include "tiles.h"

typedef struct Stack {
    Coordinates* array;
    int capacity;
    int top;
} Stack;

typedef struct Stack_N {
    struct Coordinates_plus* array;
    int capacity;
    int top;
}Stack_N;

Stack *newStack();
struct Coordinates pop(Stack *stack);
void push(Stack *stack, struct Coordinates value);
int is_empty(Stack *stack);

#endif
