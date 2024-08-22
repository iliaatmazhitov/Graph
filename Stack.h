//
// Created by Ilya Atmazhitov on 21.08.2024.
//

#ifndef INC_2_LABORATORY5_STACK_H
#define INC_2_LABORATORY5_STACK_H

#include <stdbool.h>

typedef struct Stack {
    char* arr;
    int top;
    int size;
} Stack;

Stack* createStack();
void deleteStack(Stack* stack);
bool isEmpty(Stack* stack);
void push(Stack* stack, char item);
void pop(Stack* stack);
char top(Stack* stack);

#endif //INC_2_LABORATORY5_STACK_H

