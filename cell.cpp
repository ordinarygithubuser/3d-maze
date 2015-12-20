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

GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory, TextureCoreFactory textureFactory, ShaderCoreSP textureShader);
void createWall (TransformationSP trans, MaterialCoreSP mat, GeometryCoreSP geo, GroupSP group, BumpMapCoreSP texture, ShaderCoreSP textureShader);

GroupSP createCell(int col, int row, maze::Maze m, ShaderCoreSP textureShader) {
	GeometryCoreFactory geometryFactory;
	TextureCoreFactory textureFactory("resources/textures");

	auto cellGroup = Group::create();

	// kamera startet auf 0,0. 0,1 ist das feld rechts. 1,0 ist das feld hinter der kamera.
	auto wallGroup = createWalls(col, row, m, geometryFactory, textureFactory, textureShader);

	cellGroup->addChild(wallGroup);
	return cellGroup;
}

GroupSP createWalls(int col, int row, maze::Maze m, GeometryCoreFactory geometryFactory, TextureCoreFactory textureFactory, ShaderCoreSP textureShader) {
	auto wallGroup = Group::create();
	auto matGreen = MaterialCore::create();
	auto texBrick = textureFactory.create2DTextureFromFile("brick_texture.png",
				GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	auto texBrickBump = textureFactory.createBumpMapFromFiles("brick_texture.png", "brick_normal.png",
				GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	auto matWhite = MaterialCore::create();
	matWhite->setAmbientAndDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))->setSpecular(
				glm::vec4(0.5f, 0.5f, 0.5f, 1.f))->setShininess(5.f)->init();

	float halfWidth = CELL_WIDTH / 2;
	auto wallCore = geometryFactory.createCuboid(glm::vec3(CELL_WIDTH, halfWidth, 0.5f));

	int dir = m.array[row][col];
	int top = -1;
	int right = -1;
	int left = -1;
	int bottom = -1;

	WallRenderHelper helper;

	matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.0f))->init();
	//matRed->setAmbientAndDiffuse(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->init();
	//matBlue->setAmbientAndDiffuse(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->init();

	helper.east = true;
	helper.west = true;
	helper.north = true;
	helper.south = true;

	// assign direction of surrounding cells to variables top, left, ...
	if (col + 1 < maze::MAZE_SIZE) right = m.array[row][col+1];
	if (row + 1 < maze::MAZE_SIZE) top = m.array[row+1][col];
	if (col - 1 > -1) left = m.array[row][col-1];
	if (row - 1 > -1) bottom = m.array[row-1][col];

	// If a surrounding cell points to this cell, we remove the wall.
	if (bottom == maze::NORTH) 	helper.south = false;
	if (left == maze::EAST) 	helper.west = false;
	if (right == maze::WEST) 	helper.east = false;
	if (top == maze::SOUTH) 	helper.north = false;
	
	// Now check if we point to a cell and remove the wall UNLESS it's already removed.
	if (dir == maze::NORTH && helper.north) {
		helper.north = false;
	} else if (dir == maze::WEST && helper.west) {
		helper.west = false;
	} else if (dir == maze::SOUTH && helper.south) {
		helper.south = false;
	} else if (dir == maze::EAST && helper.east) {
		helper.east = false;
	}

	// Now create the walls and apply transformations.
	if (helper.north) {
		auto trans = Transformation::create();
		createWall(trans, matWhite, wallCore, wallGroup, texBrickBump, textureShader);
		trans->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH + halfWidth));
	}
	if (helper.west) {
		auto trans = Transformation::create();
		createWall(trans, matWhite, wallCore, wallGroup, texBrickBump, textureShader);
		trans->translate(glm::vec3(col * CELL_WIDTH - halfWidth, halfWidth / 2, row * CELL_WIDTH))
			 ->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (helper.south) {
		auto trans = Transformation::create();
		createWall(trans, matWhite, wallCore, wallGroup, texBrickBump, textureShader);
		trans->translate(glm::vec3(col * CELL_WIDTH, halfWidth / 2, row * CELL_WIDTH - halfWidth));
	}
	if (helper.east) {
		auto trans = Transformation::create();
		createWall(trans, matWhite, wallCore, wallGroup, texBrickBump, textureShader);
		trans->translate(glm::vec3(col * CELL_WIDTH + halfWidth, halfWidth / 2, row * CELL_WIDTH))
			 ->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	return wallGroup;
}

void createWall (TransformationSP trans, MaterialCoreSP mat, GeometryCoreSP geo, GroupSP group, BumpMapCoreSP texture, ShaderCoreSP textureShader) {
	auto shape = Shape::create();
	shape->addCore(textureShader);
	shape->addCore(mat);
	shape->addCore(texture);
	shape->addCore(geo);
	trans->addChild(shape);
	group->addChild(trans);
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

	auto size = (MAZE_SIZE * CELL_WIDTH / 2) - (CELL_WIDTH / 2);
	floorTrans->addChild(floorShape);
	floorTrans->translate(glm::vec3(size, 0, size));
	return floorTrans;
}
