#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "maze.h"

Maze generateMaze () {
	srand(time(NULL));
	Maze m = {{{0}}};
	int remaining = MAZE_SIZE * MAZE_SIZE - 1;
	int row = rand() % MAZE_SIZE;
	int col = rand() % MAZE_SIZE;

	while (remaining > 0) {
		// Random direction (drunk walk!)
		std::array<int, 4> dirs = shuffle();

		for (int i = 0; i < 4; i++) {
			int dir = dirs[i];
			int nRow = row + getRowDiff(dir);
			int nCol = col + getColDiff(dir);

			// Is this node inside of the maze?
			if (nRow > -1 && nRow < MAZE_SIZE && nCol > -1 && nCol < MAZE_SIZE) {
				// Only mark this, if we were not here
				if (m.array[nRow][nCol] == 0) {
					if (m.array[row][col] == 0) {
						m.array[row][col] = dir;
					}
					m.array[nRow][nCol] = opposite(dir);
					remaining -= 1;
				}
				row = nRow;
				col = nCol;
				break;
			}
		}
	}
	return m;
}

void printMaze (Maze m) {
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			printf("%s | ", getName(m.array[i][j]));
		}
		printf("\n");
	}
}

int getColDiff (int dir) {
	switch (dir) {
		case EAST: return 1;
		case WEST: return -1;
		default: return 0;
	}
}

int getRowDiff (int dir) {
	switch (dir) {
		case NORTH: return -1;
		case SOUTH: return 1;
		default: return 0;
	}
}

int opposite (int dir) {
	switch (dir) {
		case EAST: return WEST;
		case WEST: return EAST;
		case NORTH: return SOUTH;
		case SOUTH: return NORTH;
		default: return 0;
	}
}

char * getName (int dir) {
	switch (dir) {
		case EAST: return (char *)"East";
		case WEST: return (char *)"West";
		case NORTH: return (char *)"North";
		case SOUTH: return (char *)"South";
		default: return (char *)"Unknown";
	}
}

std::array<int, 4> shuffle() {
	std::array<int, 4> dirs_cpy = MAZE_DIRECTIONS;
    std::random_shuffle(std::begin(dirs_cpy), std::end(dirs_cpy));
    return dirs_cpy;
}
