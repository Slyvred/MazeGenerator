#include "stack.h"
#include <stdbool.h>

#define STACKOVERFLOW -1

// création d'une pile
// stack : nom de la pile
// initialStackSize : taille de la pile en nombre de valeurs empilables
void NewStack(Stack** stack, int initialStackSize)
{
	// allocation de la structure pile
	*stack = (Stack*)malloc(sizeof(Stack));
	if (*stack != NULL)
	{
		// allocation du tableau de données
		(*stack)->array = (Vec2*)malloc(sizeof(Vec2) * initialStackSize);
		if (((*stack)->array) != NULL)
		{
			// initialisation des paramètres de la pile
			(*stack)->size = initialStackSize;
			(*stack)->eltsCount = 0;
		}
		else
		{
			// la pile n'a pas pu être créée, stack vaut NULL
			free(*stack);
			*stack = NULL;
		}
	}
}

// teste si la piles est pleine
bool isStackFull(Stack* stack) {
	return(stack->eltsCount >= stack->size);
}

// teste si la pile est vide
bool isStackEmpty(Stack* stack) {
	return(stack->eltsCount == 0);
}

// pousse une valeur sur la pile
int push(Stack* stack, Vec2 value)
{
	if (!isStackFull(stack))
	{
		stack->array[stack->eltsCount] = value;
		stack->eltsCount++;
		return(0);
	}
	else return(STACKOVERFLOW);
}

// récupère la valeur au sommet de la pile
int pull(Stack* stack, Vec2* value)
{
	if (!isStackEmpty(stack))
	{
		// COMPLETER LE CODE ICI ************************
		*value = stack->array[stack->eltsCount - 1];
		stack->eltsCount--; // On décrémente le nombre d'éléments de 1: notre dernier élément peut être écrasé

		return(EXIT_SUCCESS);
	}
	return(EXIT_FAILURE);
}

int peek(Stack* stack, Vec2* value)
{
	if (!isStackEmpty(stack))
	{
		// COMPLETER LE CODE ICI ************************
		*value = stack->array[stack->eltsCount - 1];
		return(EXIT_SUCCESS);
	}
	return(EXIT_FAILURE);
}