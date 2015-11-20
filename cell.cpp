#include <scg3.h>
#include "maze.h"
#include "cell.h"

using namespace scg;

TransformationSP createFloor(int col, int row, GeometryCoreFactory geometryFactory);
GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory);

GroupSP createCell(int col, int row, maze::Maze m) {
	GeometryCoreFactory geometryFactory;
	auto cellGroup = Group::create();

	auto floorTrans = createFloor(col, row, geometryFactory);

	auto wallGroup = createWalls(col, row, m, geometryFactory);

	cellGroup->addChild(wallGroup);
	cellGroup->addChild(floorTrans);
	return cellGroup;
}

GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory) {
	auto wallGroup = Group::create();

	auto matGreen = MaterialCore::create();
	matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.0f))
			->init();
	float halfWidth = CELL_WIDTH / 2;

	auto wallCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, halfWidth, 0.5f));
	ShapeSP wallShapes[4];
	TransformationSP wallTrans[4];
	for (int i = 0; i < 4; i++) {
		wallShapes[i] = Shape::create();
		wallShapes[i]->addCore(matGreen);
		wallShapes[i]->addCore(wallCore);
		wallTrans[i] = Transformation::create();
		wallTrans[i]->addChild(wallShapes[i]);
		wallGroup->addChild(wallTrans[i]);
	}
	wallTrans[0]->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH - halfWidth));
	wallTrans[1]->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH + halfWidth));
	wallTrans[2]->translate(glm::vec3(col * CELL_WIDTH - halfWidth, halfWidth / 2, row * CELL_WIDTH))
				->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	wallTrans[3]->translate(glm::vec3(col * CELL_WIDTH + halfWidth, halfWidth / 2, row * CELL_WIDTH))
				->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	return wallGroup;
}

TransformationSP createFloor(int col, int row, GeometryCoreFactory geometryFactory) {
	auto matYellow = MaterialCore::create();
	matYellow->setAmbientAndDiffuse(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))
				 ->init();

	auto floorTrans = Transformation::create();
	auto floorShape = Shape::create();
	auto floorCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, 0.5f, CELL_WIDTH));

	floorShape->addCore(matYellow);
	floorShape->addCore(floorCore);

	floorTrans->addChild(floorShape);
	floorTrans->translate(glm::vec3(col * CELL_WIDTH, 0, row * CELL_WIDTH));

	return floorTrans;
}
