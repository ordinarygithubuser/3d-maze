#ifndef cell
#define cell

#include <scg3.h>
#include "maze.h"

const float CELL_WIDTH = 10.0f;
const float CELL_HEIGHT = 10.0f;

scg::GroupSP createCell(int col, int row, maze::Maze m, scg::ShaderCoreSP textureShader);
scg::TransformationSP createFloor(scg::ShaderCoreSP shader);

#endif
