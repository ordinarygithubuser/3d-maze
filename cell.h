#ifndef cell
#define cell

#include <scg3.h>
#include "maze"

const float CELL_WIDTH = 10f;
const float CELL_HEIGHT = 10f;

scg::GroupSP createCell(int col, int row, Maze maze);

#endif
