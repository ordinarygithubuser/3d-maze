/**
 * \file main.cpp
 * \brief A simple scg3 example application.
 *
 * Requires C++11 and OpenGL 3.2 (or later versions).
 *
 * \author Volker Ahlers\n
 *         volker.ahlers@hs-hannover.de
 */

/*
 * Copyright 2014 Volker Ahlers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <stdexcept>
#include <vector>
#include <scg3.h>

#include "maze.h"

using namespace scg;

/**
 * \brief Configuration parameters.
 */
struct SCGConfiguration {
	static const int viewerType = 1;  // 0: simple, 1: customized
	// for customized viewer:
	static const int sceneType = 1;   // 0: teapot, 1: table
};

/**
 * \brief Minimal application using a simple viewer with default renderer, shaders,
 *   camera, and light to create a teapot scene.
 */
void useSimpleViewer();

/**
 * \brief Typical application using a customized viewer to create a teapot or table scene.
 */
void useCustomizedViewer();

/**
 * \brief Create a scene consisting of a teapot, a camera, and a light.
 */
void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP& scene);

/**
 * \brief Create a scene consisting of a floor, a table, a teapot, a camera, and a light.
 */
void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP& scene);

/**
 * \brief The main function.
 */
int main() {

	int result = 0;

	printf("%i", maze::COLS);
	maze::Maze maze1 = maze::generateMaze();
	maze::printMaze(maze1);

	try {
		if (SCGConfiguration::viewerType == 0) {
			useSimpleViewer();
		} else {
			useCustomizedViewer();
		}
	} catch (const std::exception& exc) {
		std::cerr << std::endl << "Exception: " << exc.what() << std::endl;
		result = 1;
	}
	return result;
}

// Minimal application using a simple viewer.
void useSimpleViewer() {

	// create viewer with default renderer, camera, and light
	auto viewer = Viewer::create();
	CameraSP camera;
	GroupSP scene;
	LightSP light;
	viewer->initSimpleRenderer(camera, scene, light);

	// define red material
	auto matRed = MaterialCore::create();
	matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))->setSpecular(
			glm::vec4(1.f, 1.f, 1.f, 1.f))->setShininess(20.f)->init();

	// add teapot shape to scene graph
	GeometryCoreFactory geometryFactory;
	auto teapot = Shape::create();
	teapot->addCore(matRed)->addCore(geometryFactory.createTeapot(1.f));
	light->addChild(teapot);

	// move camera backwards, enter main loop
	camera->translate(glm::vec3(0.f, 0.f, 1.f))->dolly(-1.f);
	viewer->startMainLoop();
}

// Typical application using a customized viewer.
void useCustomizedViewer() {

	// create viewer and renderer
	auto viewer = Viewer::create();
	auto renderer = StandardRenderer::create();
	viewer->init(renderer)->createWindow("s c g 3   E x a m p l e", 1024, 768);

	// create camera
	auto camera = PerspectiveCamera::create();
	renderer->setCamera(camera);

	// create scene
	GroupSP scene;
	switch (SCGConfiguration::sceneType) {
	case 0:
		createTeapotScene(viewer, camera, scene);
		break;
	case 1:
		createTableScene(viewer, camera, scene);
		break;
	default:
		throw std::runtime_error(
				"Invalid value of SCGConfiguration::sceneType [main()]");
	}
	renderer->setScene(scene);

	// start animations, enter main loop
	viewer->startAnimations()->startMainLoop();
}

void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP& scene) {

	ShaderCoreFactory shaderFactory("../scg3/shaders;../../scg3/shaders");

#ifdef SCG_CPP11_INITIALIZER_LISTS
	// Gouraud shader
	auto shaderGouraud = shaderFactory.createShaderFromSourceFiles( {
			ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER),
			ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER) });
#else
	std::vector<ShaderFile> shaderFiles;
	shaderFiles.push_back(ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER));
	auto shaderGouraud = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif

	// camera controllers
	camera->translate(glm::vec3(0.f, 0.f, 1.f))->dolly(-1.f);
#ifdef SCG_CPP11_INITIALIZER_LISTS
	viewer->addControllers( { KeyboardController::create(camera),
			MouseController::create(camera) });
#else
	viewer->addController(KeyboardController::create(camera))
	->addController(MouseController::create(camera));
#endif

	// white point light at position (10,10,10)
	auto light = Light::create();
	light->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setPosition(
			glm::vec4(10.f, 10.f, 10.f, 1.f))->init();

	// red material
	auto matRed = MaterialCore::create();
	matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))->setSpecular(
			glm::vec4(1.f, 1.f, 1.f, 1.f))->setShininess(20.f)->init();

	// teapot shape
	GeometryCoreFactory geometryFactory;
	auto teapotCore = geometryFactory.createTeapot(1.f);
	auto teapot = Shape::create();
	teapot->addCore(matRed)->addCore(teapotCore);

	// teapot transformation
	auto teapotTrans = Transformation::create();
	teapotTrans->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));

	// create scene graph
	scene = Group::create();
	scene->addCore(shaderGouraud);
	scene->addChild(camera)->addChild(light);
	light->addChild(teapotTrans);
	teapotTrans->addChild(teapot);
}

GroupSP createWalls(float floorX, float floorZ, ShaderCoreSP shaderPhongTex,
		MaterialCoreSP matWhite, bool withDoor) {
	float wallThickness = 0.2f;
	float wallHeight = 5.0f;

	TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures;resources/textures");
	auto texBrick = textureFactory.create2DTextureFromFile("brick_texture.png",
			GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);

	auto wallGroup = Group::create();
	wallGroup->addCore(shaderPhongTex)->addCore(matWhite)->addCore(texBrick);

	GeometryCoreFactory geometryFactory;
	auto bigWallCore = geometryFactory.createCuboid(
			glm::vec3(floorX, wallHeight, wallThickness));
	auto smallWallCore = geometryFactory.createCuboid(
			glm::vec3(floorZ, wallHeight, wallThickness));
	if (!withDoor) {
		ShapeSP walls[4];
		TransformationSP wallsTrans[4];
		for (int i = 0; i < 4; ++i) {
			if (i < 2) {
				walls[i] = Shape::create(bigWallCore);
			} else {
				walls[i] = Shape::create(smallWallCore);
			}

			wallsTrans[i] = Transformation::create();
			wallGroup->addChild(wallsTrans[i]);
			wallsTrans[i]->addChild(walls[i]);
		}
		wallsTrans[0]->translate(glm::vec3(0.0f, 2.0f, 5.0f));
		wallsTrans[1]->translate(glm::vec3(0.0f, 2.0f, -5.0f));
		wallsTrans[2]->translate(glm::vec3(10.0f, 2.0f, 0.0f))->rotate(90.0f,
				glm::vec3(0.0f, 1.0f, 0.0f));
		wallsTrans[3]->translate(glm::vec3(-10.0f, 2.0f, 0.0f))->rotate(90.0f,
				glm::vec3(0.0f, 1.0f, 0.0f));
	} else {
		float doorLength = 2.0f;
		float doorHeight = 3.0f;
		float doorThickness = 0.1f;
		auto doorSideCore = geometryFactory.createCuboid(glm::vec3((floorX - doorLength) / 2.0f, wallHeight, wallThickness));
		auto doorTopCore = geometryFactory.createCuboid(glm::vec3(doorLength, wallHeight - doorHeight, wallThickness));
		ShapeSP walls[6];
		TransformationSP wallsTrans[6];
		for (int i = 0; i < 6; i++) {
			if (i == 0) {
				walls[i] = Shape::create(bigWallCore);
			} else if (i < 3) {
				walls[i] = Shape::create(doorSideCore);
			} else if (i == 3) {
				walls[i] = Shape::create(doorTopCore);
			} else {
				walls[i] = Shape::create(smallWallCore);
			}
			wallsTrans[i] = Transformation::create();
			wallGroup->addChild(wallsTrans[i]);
			wallsTrans[i]->addChild(walls[i]);
		}
		wallsTrans[0]->translate(glm::vec3(0.0f, 2.0f, 5.0f));
		wallsTrans[1]->translate(glm::vec3(-5.5f, 2.0f, -5.0f));
		wallsTrans[2]->translate(glm::vec3(5.5f, 2.0f, -5.0f));
		wallsTrans[3]->translate(glm::vec3(0.0f, 3.5f, -5.0f));
		wallsTrans[4]->translate(glm::vec3(10.0f, 2.0f, 0.0f))->rotate(90.0f,
						glm::vec3(0.0f, 1.0f, 0.0f));
		wallsTrans[5]->translate(glm::vec3(-10.0f, 2.0f, 0.0f))->rotate(90.0f,
						glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return wallGroup;
}

void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP& scene) {
	ShaderCoreFactory shaderFactory("../scg3/shaders;../../scg3/shaders;../scg3/scg3/shaders");

	float floorX = 20.0f;
	float floorY = 0.05f;
	float floorZ = 10.0f;

#ifdef SCG_CPP11_INITIALIZER_LISTS
	// Phong shader
	auto shaderPhong = shaderFactory.createShaderFromSourceFiles(
			{ ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER), ShaderFile(
					"phong_frag.glsl", GL_FRAGMENT_SHADER), ShaderFile(
					"blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
					ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER) });

	// Phong shader with texture mapping
	auto shaderPhongTex =
			shaderFactory.createShaderFromSourceFiles(
					{ ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
							ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
							ShaderFile("blinn_phong_lighting.glsl",
									GL_FRAGMENT_SHADER), ShaderFile(
									"texture2d_modulate.glsl",
									GL_FRAGMENT_SHADER) });
#else
	// Phong shader
	std::vector<ShaderFile> shaderFiles;
	shaderFiles.push_back(ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER));
	auto shaderPhong = shaderFactory.createShaderFromSourceFiles(shaderFiles);

	// Phong shader with texture mapping
	shaderFiles.clear();
	shaderFiles.push_back(ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER));
	auto shaderPhongTex = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif

	// camera controllers
	camera->translate(glm::vec3(0.f, 0.5f, 1.f))->dolly(-1.f);
#ifdef SCG_CPP11_INITIALIZER_LISTS
	viewer->addControllers( { KeyboardController::create(camera),
			MouseController::create(camera) });
#else
	viewer->addController(KeyboardController::create(camera))
	->addController(MouseController::create(camera));
#endif

	// lights
	auto light = Light::create();
	light->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setPosition(
			glm::vec4(0.f, 0.9f, 0.f, 1.f))->init();

	// materials
	auto matRed = MaterialCore::create();
	matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))->setSpecular(
			glm::vec4(1.f, 1.f, 1.f, 1.f))->setShininess(20.f)->init();

	auto matYellow = MaterialCore::create();
	matYellow->setAmbientAndDiffuse(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))->init();

	auto matGreen = MaterialCore::create();
	matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.f))->init();

	auto matWhite = MaterialCore::create();
	matWhite->setAmbientAndDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))->setSpecular(
			glm::vec4(0.5f, 0.5f, 0.5f, 1.f))->setShininess(20.f)->init();

	// textures
	TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures;resources/textures");
	auto texWood = textureFactory.create2DTextureFromFile("wood_256.png",
			GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	// set texture matrix
//  texWood->scale2D(glm::vec2(4.f, 4.f));

	auto texBrick = textureFactory.create2DTextureFromFile("brick_texture.png",
			GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	auto texFloor = textureFactory.create2DTextureFromFile("parquee.png",
			GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	// floor shape and transformation
	GeometryCoreFactory geometryFactory;
	auto floorCore = geometryFactory.createCuboid(
			glm::vec3(floorX, floorY, floorZ));
	auto floor = Shape::create();
	floor->addCore(shaderPhongTex)->addCore(matWhite)->addCore(texFloor)->addCore(
			floorCore);
	auto floorTrans = Transformation::create();
	floorTrans->translate(glm::vec3(0.f, -0.5f, 0.f));

	auto walls = createWalls(floorX, floorZ, shaderPhongTex, matWhite, true);

	auto ceilingCore = geometryFactory.createCuboid(
			glm::vec3(floorX, floorY, floorZ));
	auto ceiling = Shape::create();
	ceiling->addCore(matRed)->addCore(ceilingCore);
	auto ceilingTrans = Transformation::create();
	ceilingTrans->translate(glm::vec3(0.0f, 4.5f, 0.0f));

	// teapot shape and transformation
	auto teapotCore = geometryFactory.createTeapot(0.35f);
	auto teapot = Shape::create();
	teapot->addCore(matRed)->addCore(teapotCore);
	auto teapotTrans = Transformation::create();
	teapotTrans->translate(glm::vec3(0.f, 0.9f, 0.f))->rotate(-90.f,
			glm::vec3(1.f, 0.f, 0.f));

	// table group and transformation
	auto table = Group::create();
	table->addCore(shaderPhongTex)->addCore(matWhite)->addCore(texWood);
	auto tableTrans = Transformation::create();
	tableTrans->rotate(30.f, glm::vec3(0.f, 1.f, 0.f));

	auto tableTop = Shape::create(
			geometryFactory.createCuboid(glm::vec3(1.5f, 0.05f, 1.f)));
	auto tableTopTrans = Transformation::create();
	tableTopTrans->translate(glm::vec3(0.f, 0.5f, 0.f));
	table->addChild(tableTopTrans);
	tableTopTrans->addChild(tableTop);

	auto tableLegCore = geometryFactory.createCuboid(
			glm::vec3(0.1f, 1.f, 0.1f));
	ShapeSP tableLeg[4];
	TransformationSP tableLegTrans[4];
	for (int i = 0; i < 4; ++i) {
		tableLeg[i] = Shape::create(tableLegCore);
		tableLegTrans[i] = Transformation::create();
		table->addChild(tableLegTrans[i]);
		tableLegTrans[i]->addChild(tableLeg[i]);
	}
	tableLegTrans[0]->translate(glm::vec3(0.6f, 0.f, 0.35f));
	tableLegTrans[1]->translate(glm::vec3(0.6f, 0.f, -0.35f));
	tableLegTrans[2]->translate(glm::vec3(-0.6f, 0.f, -0.35f));
	tableLegTrans[3]->translate(glm::vec3(-0.6f, 0.f, 0.35f));

	//lambda function for animation
	auto animation = TransformAnimation::create();
	float angularVel = 50.0f;
	glm::vec3 axis(0.0f, 0.0f, 1.0f);
	animation->setUpdateFunc(
			[angularVel, axis](TransformAnimation* animation,
                        double currTime, double diffTime, double totalTime) {
		animation->rotate(angularVel * static_cast<GLfloat>(diffTime), axis);
	});

	// create scene graph
	scene = Group::create();
	scene->addCore(shaderPhong);
	scene->addChild(camera)->addChild(light);
	light->addChild(floorTrans)
			->addChild(tableTrans)
			->addChild(walls)
			->addChild(ceilingTrans);
	floorTrans->addChild(floor);
	ceilingTrans->addChild(ceiling);
	tableTrans->addChild(table)->addChild(teapotTrans);
	teapotTrans->addChild(animation);
	animation->addChild(teapot);
	viewer->addAnimation(animation);
}
