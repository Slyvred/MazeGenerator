#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "generateur.h"
#include "stack.h"

int rangedRand(int range_min, int range_max)
{
	int u = (int)((double)rand() / ((double)RAND_MAX + 1) * ((double)range_max - (double)range_min)) + range_min;
	return(u);
}

int genOuterWalls(Maze* maze)
{
	if (!maze) return ERROR;

	for (int y = 0; y < maze->size.y; y++)
	{
		for (int x = 0; x < maze->size.x; x++)
		{
			// Mur = -1
			maze->data[x][y] = -1;
		}
	}

	return SUCCESS;
}

int genInnerWalls(Maze* maze)
{
	if (!maze) return ERROR;

	for (int y = 1; y < maze->size.y - 1; y++)
	{
		for (int x = 1; x < maze->size.x - 1; x++)
		{
			maze->data[x][y] = (x % 2 == 1 && y % 2 == 1) ? 0 : -1; // Alterne entre vide et mur
		}
	}
	return SUCCESS;
}

bool isFinished(Maze* maze) // Vérifie que toutes les cases ont la même valeur
{
	for (int y = 1; y < maze->size.y - 1; y += 2)
	{
		for (int x = 1; x < maze->size.x - 1; x += 2)
		{
			if (maze->data[x][y] != maze->data[1][1])
				return false;
		}
	}
	return true;
}

// Algo de Kruskal
int createPath(Maze* maze, Difficulty diff)
{
	if (!maze) return ERROR;

	// On assigne des valeurs aux endroits non remplis
	int nb = 0;
	for (int y = 0; y < maze->size.y; y++)
	{
		for (int x = 0; x < maze->size.x; x++)
		{
			if (maze->data[x][y] == 0)
			{
				nb++;
				maze->data[x][y] = nb;
			}
		}
	}

	// Entrée / Sortie
	maze->entry.x = 0, maze->entry.y = 1;
	maze->exit.x = maze->size.x - 1, maze->exit.y = maze->size.y - 2;

	// Prennent la valeur de leur voisin
	maze->data[maze->entry.x][maze->entry.y] = 1;
	maze->data[maze->exit.x][maze->exit.y] = nb;

	while(!isFinished(maze))
	{
		Vec2 pos;
		int val;

		pos.x = rangedRand(1, maze->size.x - 2);//rand() % (maze->size.x - 2) + 1;
		val = (pos.x % 2 == 0) ? 1 : 2;
		pos.y = ((rand() % ((maze->size.y - val) / 2))) * 2 + val;

		int cell1, cell2;

		if (maze->data[pos.x - 1][pos.y] == -1)
		{
			cell1 = maze->data[pos.x][pos.y - 1];
			cell2 = maze->data[pos.x][pos.y + 1];
		}
		else
		{
			cell1 = maze->data[pos.x - 1][pos.y];
			cell2 = maze->data[pos.x + 1][pos.y];
		}

		if (cell1 != cell2)
		{
			maze->data[pos.x][pos.y] = 0;

			for (int y = 1; y < maze->size.y - 1; y += 2)
			{
				for (int x = 1; x < maze->size.x - 1; x += 2)
				{
					if (maze->data[x][y] == cell2)
						maze->data[x][y] = cell1;
				}
			}
		}
	}

	maze->data[maze->entry.x][maze->entry.y] = maze->data[maze->entry.x + 1][maze->entry.y];
	maze->data[maze->exit.x][maze->exit.y] = maze->data[maze->exit.x - 1][maze->exit.y];

	// On sort avec les mêmes valeurs partout et des 0
	for (int y = 0; y < maze->size.y; y++)
	{
		for (int x = 0; x < maze->size.x; x++)
		{
			if (maze->data[x][y] == -1) // On met donc tout ce qui n'est pas un mur à 0
				maze->data[x][y] = 1;
			else
				maze->data[x][y] = 0;
		}
	}

	// Si on veut un labyrinthe plus complexe on enlève aléatoirement des murs
	// pour crééer plus de possibilitées
	if (diff != DIFF_EASY)
	{
		int iter = 0;
		switch (diff)
		{
		case DIFF_MEDIUM:
			iter = floor((float)((maze->size.x * maze->size.y) / 8));
			break;
		case DIFF_HARD:
			iter = floor((float)((maze->size.x * maze->size.y) / 6));
			break;
		default:
			break;
		}

		for (int i = 0; i < iter; i++)
		{
			Vec2 pos;
			int val;

			pos.x = rand() % (maze->size.x - 2) + 1;
			val = (pos.x % 2 == 0) ? 1 : 2;
			pos.y = ((rand() % ((maze->size.y - val) / 2))) * 2 + val;

			maze->data[pos.x][pos.y] = 0;
		}
	}

	return SUCCESS;
}

Maze* genMaze(Maze* maze, Vec2 size, Difficulty diff)
{
	if (!size.x || !size.y || size.x > X_MAX || size.y > Y_MAX) return NULL;
	if (size.x % 2 == 0) size.x++;
	if (size.y % 2 == 0) size.y++;

	maze = (Maze*)calloc(1, sizeof(Maze));

	if (!maze)
	{
		free(maze);
		return NULL;
	}

	maze->size = size;
	maze->level = diff;

	maze->data = (int*)calloc(size.x, sizeof(int*)); // Malloc des x
	for (int i = 0; i < size.x; i++)
		maze->data[i] = (int*)calloc(size.y, sizeof(int)); // Malloc des y

	if (maze->data == NULL) return NULL;
	if (genOuterWalls(maze) == ERROR) return NULL;
	if (genInnerWalls(maze) == ERROR) return NULL;
	if (createPath(maze, diff) == ERROR) return NULL;

	return maze;
}

void printMaze(Maze* maze)
{
	for (int y = 0; y < maze->size.y; y++)
	{
		for (int x = 0; x < maze->size.x; x++)
		{
			char chr = ' ';
			if (maze->data[x][y] == 1) chr = '@';
			else if (maze->data[x][y] == -2) chr = 'O';
			else if (maze->data[x][y] == -3) chr = '*';
			else chr = ' ';
			printf("%c ", chr);
			//printf("%4d ", maze->data[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}


unsigned int calcDist(Vec2 a, Vec2 b)
{
	if (a.x == b.x && a.y == b.y) return 0;
	// * 100 pour avoir plus de précision (en gros 2 chiffre après la virgule mais sans virgule)
	return (unsigned int)(100 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

bool checkPos(Maze* maze, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < maze->size.x && pos.y >= 0 && pos.y < maze->size.y);
}

bool canMove(Maze* maze, Vec2 pos, Dir dir)
{
	return (checkPos(maze, pos) && maze->data[pos.x][pos.y] != 1);
}

int getNeighbor(Maze* maze, Vec2 pos, Dir dir)
{
	switch (dir)
	{
	case DIR_UP:
		pos.y--;
		break;
	case DIR_DOWN:
		pos.y++;
		break;
	case DIR_LEFT:
		pos.x--;
		break;
	case DIR_RIGHT:
		pos.x++;
		break;
	default:
		break;
	}

	if (checkPos(maze, pos, dir))
		return (maze->data[pos.x][pos.y]);
	else return -1;
}

int countNeighbors(Maze* maze, Vec2 pos) // Compte les voisins aux 4 points cardinaux de pos
{
	if (!maze) return ERROR;

	int nbNeighbors = 0;

	for (int y = pos.y - 1; y <= pos.y + 1; y++)
	{
		Vec2 _pos = { pos.x, y };
		if (checkPos(maze, _pos))
			if (y != pos.y && maze->data[pos.x][y] != 1 && maze->data[pos.x][y] != -2)
				nbNeighbors++;
	}

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		Vec2 _pos = { x, pos.y };
		if (checkPos(maze, _pos))
			if (x != pos.x && maze->data[x][pos.y] != 1 && maze->data[x][pos.y] != -2)
				nbNeighbors++;
	}

	return nbNeighbors;
}

void move(Maze* map, Vec2* pos, Dir dir)
{
	Vec2 _pos = *pos;
	switch (dir)
	{
	case DIR_UP:
		_pos.y--;
		break;
	case DIR_DOWN:
		_pos.y++;
		break;
	case DIR_LEFT:
		_pos.x--;
		break;
	case DIR_RIGHT:
		_pos.x++;
		break;
	default:
		break;
	}
	if (canMove(map, _pos, dir))
		*pos = _pos;
}

int solveMaze(Maze* maze)
{
	if (!maze) return ERROR;

	Vec2 pos = maze->entry, oldPos = pos;
	Dir dir = DIR_RIGHT;

	Stack* visited;
	NewStack(&visited, maze->size.x * maze->size.y);


	maze->data[maze->entry.x][maze->entry.y] = -2;//calcDist(maze->entry, maze->exit);

	// Chaque cellule contient sa distance par rapport à la sortie
	for (int y = 1; y < maze->size.y - 1; y++)
	{
		for (int x = 1; x < maze->size.x - 1; x++)
		{
			if (maze->data[x][y] != 1)
				maze->data[x][y] = calcDist((Vec2) { x, y }, maze->exit);
		}
	}
	//printMaze(maze);

	while (pos.x != maze->exit.x || pos.y != maze->exit.y)
	//for (int i = 0; i < maze->size.x * maze->size.y; i++)
	{
		int neighbors[4];
		neighbors[DIR_UP] = getNeighbor(maze, pos, DIR_UP);
		neighbors[DIR_DOWN] = getNeighbor(maze, pos, DIR_DOWN);
		neighbors[DIR_LEFT] = getNeighbor(maze, pos, DIR_LEFT);
		neighbors[DIR_RIGHT] = getNeighbor(maze, pos, DIR_RIGHT);

		int smallest = INT_MAX;
		Dir newDir = DIR_UNDEFINED;
		for (int i = 0; i < 4; i++)
		{
			if (neighbors[i] <= smallest && neighbors[i] != 1 && neighbors[i] != -1 && neighbors[i] != -2)
			{
				smallest = neighbors[i];
				newDir = i;
			}
			else if (neighbors[i] != 1 && neighbors[i] != -1 && neighbors[i] != -2) // Si on retourne en arrière
			{
				smallest = neighbors[i];
				newDir = i;
			}
		}

		move(maze, &pos, newDir);
		if (pos.x != oldPos.x || pos.y != oldPos.y)
		{
			maze->data[pos.x][pos.y] = -2; // On marque la cellule comme visitée
			push(visited, pos); // On ajoute ses coordonnées dans la pile
			oldPos = pos;
			//printf("--> (%d, %d)\n", pos.x, pos.y);
			system("cls");
			printMaze(maze);
		}

		// Backtracking
		if (countNeighbors(maze, pos) == 0 && pos.x != maze->exit.x && pos.y != maze->exit.y)
		{
			pull(visited, &pos);
			oldPos = pos;
			//printf("%s\n", "No more neighbors: pulling from stack");
		}

		//if (pos.x == maze->exit.x && pos.y == maze->exit.y)
		//	break;
	}

	// On enlève les chemins et les distances
	for (int y = 1; y < maze->size.y - 1; y++)
	{
		for (int x = 1; x < maze->size.x - 1; x++)
		{
			if (maze->data[x][y] != 1)
				maze->data[x][y] = 0;
		}
	}

	// La pile contient le chemin le plus court
	while (!isStackEmpty(visited))
	{
		pull(visited, &pos);
		maze->data[pos.x][pos.y] = -2;
	}

	return SUCCESS;
}