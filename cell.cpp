#include <scg3.h>
#include "maze.h"
#include "cell.h"

using namespace scg;

GroupSP createCell(int col, int row, maze::Maze m) {

	GeometryCoreFactory geometryFactory;
	auto cellCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, 0.5f, CELL_WIDTH));
	auto c = Group::create();
	auto matRed = MaterialCore::create();
	matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))->setSpecular(
				glm::vec4(1.f, 1.f, 1.f, 1.f))->setShininess(20.f)->init();
	auto cellShape = Shape::create(cellCore);
	cellShape->addCore(matRed);
	auto cellTrans = Transformation::create();
	cellTrans->addChild(cellShape);
	cellTrans->translate(glm::vec3(col * CELL_WIDTH, 0, row * CELL_WIDTH));

	c->addChild(cellTrans);
	return c;
}
