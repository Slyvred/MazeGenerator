#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdbool.h>
#include "generateur.h"

#define STACKOVERFLOW -1

typedef struct {
	Vec2* array;
	int size;
	int eltsCount;
} Stack;


void NewStack(Stack** stack, int initialStackSize);
bool isStackFull(Stack* stack);
bool isStackEmpty(Stack* stack);
int push(Stack* stack, Vec2 value);
int pull(Stack* stack, Vec2* value);
int peek(Stack* stack, Vec2* value);