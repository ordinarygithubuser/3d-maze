#ifndef cell
#define cell

#include <scg3.h>
#include "maze.h"

/*
 * Width of a cell.
 */
const float CELL_WIDTH = 10.0f;
/*
 * Height (or Depth) of the cell.
 */
const float CELL_HEIGHT = 10.0f;

/*
 * Method creates a complete cell of the maze, the location is given by tow and column in the maze-matrix.
 * A cell consists of a floor and a maximum of 3 walls.
 * The cell is returned as a GroupSP to insert the it into the graph.
 */
scg::GroupSP createCell(int col, int row, maze::Maze m, scg::ShaderCoreSP textureShader);

#endif
