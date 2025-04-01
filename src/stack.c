#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack* stack_create(size_t capacity) {
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    stack->items = malloc(sizeof(FILE*) * capacity);
    if (!stack->items) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->top = 0;
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack) {
        free(stack->items);
        free(stack);
    }
}

bool stack_push(Stack *stack, FILE *item) {
    if (stack->top >= stack->capacity) {
        return false; 
    }
    stack->items[stack->top++] = item;
    return true;
}

FILE* stack_pop(Stack *stack) {
    if (stack->top == 0) {
        return NULL; 
    }

    return stack->items[--stack->top];
}

FILE* stack_peek(Stack *stack) {
    if (stack->top == 0) {
        return NULL;
    }
    return stack->items[stack->top - 1];
}

bool stack_is_empty(Stack *stack) {
    return stack->top == 0;
}

bool stack_is_full(Stack *stack) {
    return stack->top == stack->capacity;
}