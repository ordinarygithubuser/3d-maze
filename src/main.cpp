#include <iostream>
#include <stdexcept>
#include <vector>
#include <scg3.h>

#include "maze.h"
#include "cell.h"

using namespace scg;

void useCustomizedViewer();

void createMazeScene(ViewerSP viewer, CameraSP camera, GroupSP& scene, LightSP& light);

int main() {
	useCustomizedViewer();
	return 0;
}

void useCustomizedViewer() {
	// create viewer and renderer
	auto viewer = Viewer::create();
	auto renderer = StandardRenderer::create();
	viewer->init(renderer)->createWindow("3D Maze", 1024, 768);

	// Light
	auto light = Light::create();
	auto lightPosition = LightPosition::create(light);
	light->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
	     ->setPosition(glm::vec4(0.f, 2.f, 1.f, 1.f))
	     ->init();
	light->addChild(lightPosition);



	// create camera
	auto camera = MazeCamera::create(light);
	renderer->setCamera(camera);

	// create scene
	GroupSP scene;
	createMazeScene(viewer, camera, scene, light);
	renderer->setScene(scene);

	// start animations, enter main loop
	viewer->startAnimations()->startMainLoop();
}

void createMazeScene(ViewerSP viewer, CameraSP camera, GroupSP& scene, LightSP& light) {
	ShaderCoreFactory shaderFactory("resources/shaders");
#ifdef SCG_CPP11_INITIALIZER_LISTS
	auto shaderPhong = shaderFactory.createShaderFromSourceFiles({
		ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
		ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
		ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
		ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
	});

	// Gouraud shader
	auto shaderGouraud = shaderFactory.createShaderFromSourceFiles({
		ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER),
		ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER)
	});

	// Phong shader with texture mapping
	auto shaderBumpTex =
			shaderFactory.createShaderFromSourceFiles({
				ShaderFile("bump_vert.glsl", GL_VERTEX_SHADER),
				ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER),
				ShaderFile("bump_frag.glsl", GL_FRAGMENT_SHADER)
	});

	// Skybox shader
	auto shaderSkybox = shaderFactory.createShaderFromSourceFiles( {
		ShaderFile("skybox_vert.glsl", GL_VERTEX_SHADER),
		ShaderFile("skybox_frag.glsl", GL_FRAGMENT_SHADER)
	});
#else
	std::vector<ShaderFile> shaderFiles;
	shaderFiles.push_back(ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER));
	auto shaderGouraud = shaderFactory.createShaderFromSourceFiles(shaderFiles);

	// Phong shader with texture mapping
	shaderFiles.clear();
	shaderFiles.push_back(ShaderFile("bump_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("bump_frag.glsl", GL_FRAGMENT_SHADER));
	auto shaderBumpTex = shaderFactory.createShaderFromSourceFiles(shaderFiles);

	shaderFiles.clear();
	shaderFiles.push_back(ShaderFile("skybox_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("skybox_frag.glsl", GL_FRAGMENT_SHADER));
	auto shaderSkybox = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif
	// camera controllers
	camera->translate(glm::vec3(0.f, 2.f, 1.f))->dolly(0.f);
#ifdef SCG_CPP11_INITIALIZER_LISTS
	viewer->addControllers({
		FirstPersonController::create(camera),
		MouseController::create(camera)
	});
#else
	viewer->addController(FirstPersonController::create(camera))
		  ->addController(MouseController::create(camera));
#endif
	maze::Maze maze1 = maze::generateMaze();

	GeometryCoreFactory geometryFactory;
	TextureCoreFactory textureFactory("resources/textures");
	std::vector<std::string> skyboxFiles;
	skyboxFiles.push_back("right.png");
	skyboxFiles.push_back("left.png");
	skyboxFiles.push_back("up.png");
	skyboxFiles.push_back("down.png");
	skyboxFiles.push_back("back.png");
	skyboxFiles.push_back("front.png");
	auto texSkybox = textureFactory.createCubeMapFromFiles(skyboxFiles);

	auto matWhite = MaterialCore::create();
	matWhite->setAmbientAndDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
			->setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.f))
			->setShininess(5.f)
			->init();

	auto skyboxTrans = Transformation::create();
	auto skyboxShape = Shape::create();
	auto skyboxCore = geometryFactory.createCuboid(glm::vec3(250.f, 250.f, 250.f));

	skyboxShape->addCore(shaderSkybox);
	skyboxShape->addCore(matWhite);
	skyboxShape->addCore(texSkybox);
	skyboxShape->addCore(skyboxCore);
	skyboxTrans->addChild(skyboxShape);

	auto mazeScene = Group::create();
	mazeScene->addCore(shaderPhong);
	mazeScene->addChild(camera)->addChild(light);
	light->addChild(skyboxTrans);
	maze::printMaze(maze1);
	for (int i = 0; i < maze::MAZE_SIZE; i++) {
		for (int j = 0; j < maze::MAZE_SIZE; j++) {
			light->addChild(cell::createCell(j, i, maze1, shaderBumpTex));
		}
	}

//	light->addChild(cell::createFloor(shaderBumpTex));
	scene = mazeScene;
}
