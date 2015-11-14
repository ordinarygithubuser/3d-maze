#include <scg3.h>
#include "maze.h"
#include "cell.h"

using namespace scg;

GroupSP createCell(int col, int row, maze::Maze m) {
	GeometryCoreFactory geometryFactory;
	auto cellGroup = Group::create();
	auto matYellow = MaterialCore::create();
	matYellow->setAmbientAndDiffuse(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))
			 ->init();
	auto matGreen = MaterialCore::create();
	matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.f))
			->init();
	float halfWidth = CELL_WIDTH / 2;
	// floor
	auto floorTrans = Transformation::create();
	auto floorShape = Shape::create();
	auto floorCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, 0.5f, CELL_WIDTH));

	floorShape->addCore(matYellow);
	floorShape->addCore(floorCore);

	floorTrans->addChild(floorShape);
	floorTrans->translate(glm::vec3(col * CELL_WIDTH, 0, row * CELL_WIDTH));

	//walls
	auto wallShape = Shape::create(floorCore);
	wallShape->addCore(matGreen);
	TransformationSP wallTrans[2];
	for (int i = 0; i < 2; i++) {
		wallTrans[i] = Transformation::create();
		wallTrans[i]->addChild(wallShape);
		cellGroup->addChild(wallTrans[i]);
	}
	wallTrans[0]->translate(glm::vec3(col * CELL_WIDTH - halfWidth, 0, row * CELL_WIDTH))
				->rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	wallTrans[1]->translate(glm::vec3(col * CELL_WIDTH + halfWidth, 0, row * CELL_WIDTH))
				->rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//	wallTrans[2]->translate(glm::vec3(col * CELL_WIDTH, 0, row * CELL_WIDTH - halfWidth))
//				->rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f))
//				->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//	wallTrans[3]->translate(glm::vec3(col * CELL_WIDTH + CELL_WIDTH / 2, 0, row * CELL_WIDTH))
//				->rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f))
//				->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	cellGroup->addChild(floorTrans);
	return cellGroup;
}
