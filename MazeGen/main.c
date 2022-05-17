#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "generateur.h"

int main()
{
	srand(time(NULL));
	Maze* maze = NULL;
	Vec2 size = { 11, 11 };
	maze = genMaze(maze, size, DIFF_HARD);
	printMaze(maze);
	solveMaze(maze);
	printMaze(maze);
}