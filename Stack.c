//
// Created by Ilya Atmazhitov on 21.08.2024.
//

#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->size = 1;
    stack->top = 0;
    stack->arr = (char*)malloc(stack->size * sizeof(char));
    return stack;
}

void deleteStack(Stack* stack) {
    free(stack->arr);
    free(stack);
}

bool isEmpty(Stack* stack) {
    return stack->top == 0;
}

void push(Stack* stack, char item) {
    if (stack->top == stack->size) {
        stack->size *= 2;
        stack->arr = (char*)realloc(stack->arr, stack->size * sizeof(char));
    }
    stack->arr[stack->top++] = item;
}

void pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Popped element: %c\n", stack->arr[--stack->top]);
}

char top(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return '\0';
    }
    return stack->arr[stack->top - 1];
}