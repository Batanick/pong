#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Camera.h"
#include "Label.h"
#include "FpsCounter.h"

#include "logging.h"

//#define DRAW_TEST_MONKEY

bool Renderer::init() {
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

    assetManager = std::shared_ptr<AssetManager>( new AssetManager() );
    VERIFY ( assetManager->init(), "Unable to initialise asset manager", return false );

	shaderManager = std::shared_ptr<ShaderManager>( new ShaderManager() );
	VERIFY ( shaderManager->init(), "Unable to initialise shader manager", return false );

    fpsCounter = std::shared_ptr<FpsCounter>( new FpsCounter() );

	initContext();

	camera = std::shared_ptr<Camera>( new Camera() );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

    std::shared_ptr<Terrain> terrain = std::shared_ptr<Terrain>( new Terrain() );
	terrain->init( 0.1f, 16 );
    add( ShaderType::TERRAIN_SHADER ,terrain );

#ifdef DRAW_TEST_MONKEY
	std::shared_ptr<Mesh> msh( new Mesh( "../models/monkey.obj", "../textures/testChecker.DDS" ) );
    msh->init( *assetManager );
	renderables.push_back( msh );
#endif

    static std::shared_ptr<Label> fpsLabel( new Label(assetManager->getDefaultFont(), "DUMMY", 20, context.windowHeight - 50, glm::vec3(0,1,0)) );
    this->fpsLabel = fpsLabel;
    add( ShaderType::FONT_SHADER, fpsLabel );

	return true;
}

void Renderer::initContext() {
	context.meshTextureUniformId = shaderManager->getParam( ShaderType::MODEL_SHADER, "texture" );
	context.meshMVPId = shaderManager->getParam( ShaderType::MODEL_SHADER, "mvp" );

	context.terrainMVPId = shaderManager->getParam( ShaderType::TERRAIN_SHADER, "mvp" );
	context.terrainMinMaxId = shaderManager->getParam( ShaderType::TERRAIN_SHADER, "minMax" );

    context.fontTextureId = shaderManager->getParam( ShaderType::FONT_SHADER, "texture" );
    context.fontColorId = shaderManager->getParam( ShaderType::FONT_SHADER, "fontColor" );

    int windowW, windowH;
	glfwGetWindowSize( window, &windowW, &windowH );
    context.windowHeight = windowH;
    context.windowWidth = windowW;
}

void Renderer::render( double timeDelta ) {
	camera->onBeforeRender( window, timeDelta );

	context.timeDelta = timeDelta;
	const glm::mat4 view = camera->getView();
	const glm::mat4 projection = camera->getProjection();
	context.pv = projection * view;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderAll();

    glfwSwapBuffers(window);

    fpsCounter->onFrame();
    char buff[20];
    sprintf_s(buff, "FPS: %.2f", fpsCounter->getFps());
    fpsLabel->setText(buff);
}

void Renderer::add( ShaderType type, PRenderable renderable ) {
    renderables.insert( std::make_pair(type, renderable) );
}

void Renderer::renderAll() {
    // expecting that elements ordered by key
    ShaderType currentType = ShaderType::NONE;
    for ( auto iterator = renderables.begin(); iterator != renderables.end();iterator++ ) {
        if ( iterator->first != currentType ) {
            currentType = iterator->first;
            shaderManager->useProgram(currentType);
        }

        iterator->second->render( context );
    }
}

void Renderer::shutdown() {
	for ( auto iterator = renderables.begin(); iterator != renderables.end();iterator++ ) {
        iterator->second->shutdown();
	}

	shaderManager->shutdown();
	assetManager->shutdown();
}
