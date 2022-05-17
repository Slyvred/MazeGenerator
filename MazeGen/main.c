#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "generateur.h"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("\n[+] Usage: {height} {width} {difficulty}\n");
		printf("[+] Difficulty: 1 = easy, 2 = medium, 3 = hard\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));
	Maze* maze = NULL;
	Vec2 size = { atoi(argv[1]), atoi(argv[2])};
	maze = genMaze(maze, size, atoi(argv[3]));
	printMaze(maze);
	solveMaze(maze);
	printMaze(maze);
}