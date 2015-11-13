#include <scg3.h>
#include "maze.h"
#include "cell.h"

using namespace scg;

GroupSP createCell(int col, int row, maze::Maze m) {
	GeometryCoreFactory geometryFactory;
	auto cellCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, 0.5f, CELL_WIDTH));
	auto cellGroup = Group::create();
	auto cellShape = Shape::create();
	auto matRed = MaterialCore::create();
	auto cellTrans = Transformation::create();

	matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f));
	matRed->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f));
	matRed->setShininess(20.f);
	matRed->init();

	cellShape->addCore(matRed);
	cellShape->addCore(cellCore);

	cellTrans->addChild(cellShape);
	cellTrans->translate(glm::vec3(col * CELL_WIDTH, 0, row * CELL_WIDTH));

	cellGroup->addChild(cellTrans);
	return cellGroup;
}
