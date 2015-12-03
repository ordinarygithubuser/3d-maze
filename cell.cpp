#include <scg3.h>
#include "maze.h"
#include "cell.h"

using namespace scg;

struct WallRenderHelper {
	int wallCount;
	bool north = false;
	bool south = false;
	bool west = false;
	bool east = false;
};

GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory);

GroupSP createCell(int col, int row, maze::Maze m) {
	GeometryCoreFactory geometryFactory;
	auto cellGroup = Group::create();

	// kamera startet auf 0,0. 0,1 ist das feld rechts. 1,0 ist das feld hinter der kamera.
	auto wallGroup = createWalls(col, row, m, geometryFactory);

	cellGroup->addChild(wallGroup);
	return cellGroup;
}

GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory) {
	auto wallGroup = Group::create();

	auto matGreen = MaterialCore::create();
	matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.0f))
			->init();
	float halfWidth = CELL_WIDTH / 2;

	auto wallCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, halfWidth, 0.5f));

	WallRenderHelper helper;

	if (row == 0 && col == 0) {
		helper.wallCount = 4;
		helper.east = true;
		helper.west = true;
		helper.north = true;
		helper.south = true;
	} else if (row == 0 && col == maze::MAZE_SIZE - 1) {
		helper.wallCount = 3;
		helper.north = true;
		helper.south = true;
		helper.east = true;
	} else if (col == 0) {
		helper.wallCount = 3;
		helper.west = true;
		helper.south = true;
		helper.east = true;
	} else {
		helper.wallCount = 2;
		helper.south = true;
		helper.east = true;
	}
	int mazeDir = m.array[row][col];
	if (mazeDir == 1) {
		if (helper.north) {
			helper.wallCount = helper.wallCount - 1;
		}
		helper.north = false;
	} else if (mazeDir == 2) {
		if (helper.east) {
			helper.wallCount = helper.wallCount - 1;
		}
		helper.east = false;
	} else if (mazeDir == 3) {
		if (helper.south) {
			helper.wallCount = helper.wallCount - 1;
		}
		helper.south = false;
	} else {
		if (helper.west) {
			helper.wallCount = helper.wallCount - 1;
		}
		helper.west = false;
	}
	ShapeSP wallShapes[helper.wallCount];
	TransformationSP wallTrans[helper.wallCount];
	for (int i = 0; i < helper.wallCount; i++) {
		wallShapes[i] = Shape::create();
		wallShapes[i]->addCore(matGreen);
		wallShapes[i]->addCore(wallCore);
		wallTrans[i] = Transformation::create();
		wallTrans[i]->addChild(wallShapes[i]);
		wallGroup->addChild(wallTrans[i]);
	}
	int currentIndex = 0;
	if (helper.north) {
		wallTrans[currentIndex]->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH - halfWidth));
		currentIndex++;
	}
	if (helper.west) {
		wallTrans[currentIndex]->translate(glm::vec3(col * CELL_WIDTH - halfWidth, halfWidth / 2, row * CELL_WIDTH))
							   ->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		currentIndex++;
	}
	if (helper.south) {
		wallTrans[currentIndex]->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH + halfWidth));
		currentIndex++;
	}
	if (helper.east) {
		wallTrans[currentIndex]->translate(glm::vec3(col * CELL_WIDTH + halfWidth, halfWidth / 2, row * CELL_WIDTH))
					->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	return wallGroup;
}

TransformationSP createFloor() {
	GeometryCoreFactory geometryFactory;
	auto matYellow = MaterialCore::create();
	matYellow->setAmbientAndDiffuse(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))
				 ->init();

	auto floorTrans = Transformation::create();
	auto floorShape = Shape::create();
	auto floorCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH * MAZE_SIZE, 0.5f, CELL_WIDTH * MAZE_SIZE));

	floorShape->addCore(matYellow);
	floorShape->addCore(floorCore);

	// just done once, don't create temp var
	auto bla = (MAZE_SIZE * CELL_WIDTH / 2) - (CELL_WIDTH / 2);
	floorTrans->addChild(floorShape);
	floorTrans->translate(glm::vec3(bla, 0, bla));
	return floorTrans;
}
