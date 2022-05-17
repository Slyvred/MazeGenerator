#pragma once

#define ERROR -1
#define SUCCESS 0

#define X_MAX 999
#define Y_MAX 999

typedef enum DIR
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UNDEFINED
}Dir;

typedef enum DIFFICULTY
{
	DIFF_EASY = 1,
	DIFF_MEDIUM = 2,
	DIFF_HARD = 3,
}Difficulty;

typedef struct vec2 { int x, y; }Vec2;

typedef struct maze {
	Vec2 entry, exit, size;
	int** data;
	Difficulty level;
}Maze;

Maze* genMaze(Maze* maze, Vec2 size, Difficulty diff);
void printMaze(Maze* maze);
int solveMaze(Maze* maze);