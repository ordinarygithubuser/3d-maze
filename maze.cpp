#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

// this sucks, but is the most convenient way for now
const int ROWS = 4;
const int COLS = 4;

struct Maze {
	int array[ROWS][COLS];
};

// header
Maze generateMaze();
void printMaze(Maze maze);
int getColDiff(int dir);
int getRowDiff(int dir);
int opposite(int dir);
char* getName(int dir);
void shuffle(int *dirs, int size);

const int NORTH = 1;
const int EAST = 2;
const int SOUTH = 3;
const int WEST = 4;

int DIRECTIONS[4] = { NORTH, EAST, SOUTH, WEST};

int main () {
	// Seed for random generator
	srand(time(NULL));

	Maze maze = generateMaze();
	printMaze(maze);
}

Maze generateMaze () {
	Maze maze = {{{0}}};
	int remaining = COLS * ROWS - 1;
	int row = rand() % ROWS;
	int col = rand() % COLS;

	while (remaining > 0) {
		// Random direction (drunk walk!)
		int * dirs = DIRECTIONS;
		shuffle(dirs, 4);

		for (int i = 0; i < 4; i++) {
			int dir = dirs[i];
			int nRow = row + getRowDiff(dir);
			int nCol = col + getColDiff(dir);

			// Is this node inside of the maze?
			if (nRow > -1 && nRow < ROWS && nCol > -1 && nCol < COLS) {
				// Only mark this, if we were not here
				if (maze.array[nRow][nCol] == 0) {
					maze.array[row][col] = dir;
					maze.array[nRow][nCol] = opposite(dir);
					remaining -= 1;
				}
				row = nRow;
				col = nCol;
				break;
			}
		}
	}
	return maze;
}

void printMaze (Maze maze) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			printf("p(%i, %i) = %s\r", i, j, getName(maze.array[i][j]));
		}
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
		case NORTH: return 1;
		case SOUTH: return -1;
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

void shuffle(int *array, int n) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);

    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; i--) {
            size_t j = (unsigned int) (drand48()*(i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
