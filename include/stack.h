#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack {
    FILE **items;
    size_t capacity;
    size_t top;
} Stack;

Stack* stack_create(size_t capacity);

void stack_destroy(Stack *stack);

bool stack_push(Stack *stack, FILE *item);

FILE* stack_pop(Stack *stack);

FILE* stack_peek(Stack *stack);

bool stack_is_empty(Stack *stack);

bool stack_is_full(Stack *stack);

#endif // STACK_H