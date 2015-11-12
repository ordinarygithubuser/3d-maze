#ifndef maze
#define maze

#import <array>

// this sucks, but is the most convenient way for now
const int ROWS = 4;
const int COLS = 4;

const int NORTH = 1;
const int EAST = 2;
const int SOUTH = 3;
const int WEST = 4;

const std::array<int, 4> MAZE_DIRECTIONS = { NORTH, EAST, SOUTH, WEST};

struct Maze {
	int array[ROWS][COLS];
};

// header
Maze generateMaze();
bla(Maze maze);
void printMaze(Maze maze);
int getColDiff(int dir);
int getRowDiff(int dir);
int opposite(int dir);
char* getName(int dir);
std::array<int, 4> shuffle();

#endif
