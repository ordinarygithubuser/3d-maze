#include <iostream>
#include <stdexcept>
#include <vector>
#include <scg3.h>

#include "maze.h"
#include "cell.h"

using namespace scg;

void useCustomizedViewer();

void createMazeScene(ViewerSP viewer, CameraSP camera, GroupSP& scene);

int main() {
	useCustomizedViewer();
	return 0;
}

void useCustomizedViewer() {
	// create viewer and renderer
	auto viewer = Viewer::create();
	auto renderer = StandardRenderer::create();
	viewer->init(renderer)->createWindow("3D Maze", 1024, 768);

	// create camera
	auto camera = PerspectiveCamera::create();
	renderer->setCamera(camera);

	// create scene
	GroupSP scene;
	createMazeScene(viewer, camera, scene);
	renderer->setScene(scene);

	// start animations, enter main loop
	viewer->startAnimations()->startMainLoop();
}

void createMazeScene(ViewerSP viewer, CameraSP camera, GroupSP& scene) {
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
	auto shaderPhongTex =
			shaderFactory.createShaderFromSourceFiles({
				ShaderFile("bump_vert.glsl", GL_VERTEX_SHADER),
//				ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
//				ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
//				ShaderFile("blinn_phong_lighting.glsl",	GL_FRAGMENT_SHADER),
				ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER),
				ShaderFile("bump_frag.glsl", GL_FRAGMENT_SHADER)
	});
#else
	std::vector<ShaderFile> shaderFiles;
	shaderFiles.push_back(ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER));
	auto shaderGouraud = shaderFactory.createShaderFromSourceFiles(shaderFiles);

	// Phong shader with texture mapping
	shaderFiles.clear();
	shaderFiles.push_back(ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER));
	shaderFiles.push_back(ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER));
	shaderFiles.push_back(ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER));
	auto shaderPhongTex = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif
	// camera controllers
	camera->translate(glm::vec3(0.f, 2.f, 1.f))->dolly(0.f);
#ifdef SCG_CPP11_INITIALIZER_LISTS
	viewer->addControllers({
		KeyboardController::create(camera),
		MouseController::create(camera)
	});
#else
	viewer->addController(KeyboardController::create(camera))
		  ->addController(MouseController::create(camera));
#endif
	maze::Maze maze1 = maze::generateMaze();

	auto light = Light::create();
	light->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
	       ->setPosition(glm::vec4(0.f, 2.f, 1.f, 1.f))
	       ->init();

	auto mazeScene = Group::create();
	mazeScene->addCore(shaderPhong);
	mazeScene->addChild(camera)->addChild(light);
	for (int i = 0; i < maze::MAZE_SIZE; i++) {
		for (int j = 0; j < maze::MAZE_SIZE; j++) {
			light->addChild(cell::createCell(i, j, maze1, shaderPhongTex));
		}
	}
	light->addChild(cell::createFloor());
	scene = mazeScene;
}
