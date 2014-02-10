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

	terrain = std::shared_ptr<Terrain>( new Terrain() );
	terrain->init( 0.1f, 16 );

#ifdef DRAW_TEST_MONKEY
	std::shared_ptr<Mesh> msh( new Mesh( "../models/monkey.obj", "../textures/testChecker.DDS" ) );
	msh->init( *textureManager );
	renderables.push_back( msh );
#endif

	return true;
}

void Renderer::initContext() {
	context.meshTextureUniformId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "texture" );
	context.meshMVPId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "mvp" );

	context.terrainMVPId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "mvp" );
	context.terrainMinMaxId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "minMax" );

    context.fontTextureId = shaderManager->getParam( ShaderManager::ShaderType::FONT_SHADER, "texture" );
    context.fontColorId = shaderManager->getParam( ShaderManager::ShaderType::FONT_SHADER, "fontColor" );

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
	
	renderTerrain();
	renderMeshes();
	renderTexts();

    glfwSwapBuffers(window);

    fpsCounter->onFrame();
}

void Renderer::renderMeshes() {
	shaderManager->useProgram( ShaderManager::ShaderType::MODEL_SHADER );

	for (PRenderable& renderable : renderables) {
		renderable->render( context );
	}
}

void Renderer::renderTerrain() {
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glDisable( GL_BLEND );

	shaderManager->useProgram( ShaderManager::ShaderType::TERRAIN_SHADER );

	const glm::mat4 mvp = camera->getProjection() * camera->getView();
	glUniformMatrix4fv( context.terrainMVPId, 1, GL_FALSE, &mvp[0][0] );
	
	terrain->render( context );
}

void Renderer::renderTexts() {
	shaderManager->useProgram( ShaderManager::ShaderType::FONT_SHADER );
	glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glEnable( GL_BLEND );
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //TODO: extract GUI to separate class 
    static std::shared_ptr<Label> fpsLabel( new Label(assetManager->getDefaultFont(), "DUMMY", 20, context.windowHeight - 50, glm::vec3(0,1,0)) );

    char buff[20];
    sprintf_s(buff, "FPS: %.2f", fpsCounter->getFps());

    fpsLabel->setText(buff);
    fpsLabel->render( context );
}

void Renderer::shutdown() {
	for ( PRenderable& mesh : renderables ) {
		mesh->shutdown();
	}

	shaderManager->shutdown();
	assetManager->shutdown();
}
