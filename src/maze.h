#ifndef maze
#define maze

#include <array>

const int MAZE_SIZE = 8;

const int NORTH = 1;
const int EAST = 2;
const int SOUTH = 3;
const int WEST = 4;

const std::array<int, 4> MAZE_DIRECTIONS = { NORTH, EAST, SOUTH, WEST};

/*
 * Representation of the maze.
 */
struct Maze {
	int array[MAZE_SIZE][MAZE_SIZE];
};

/*
 * Generates the maze via an implementation of a "drunk walk"-algorithm.
 * The resulting maze has the dimensions of MAZE_SIZE * MAZE_SIZE.
 */
Maze generateMaze();
/*
 * Prints each cell of the maze with the direction resulting from the "drunk walk".
 */
void printMaze(Maze maze);

#endif
