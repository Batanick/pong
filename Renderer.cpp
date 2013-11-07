#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Camera.h"
#include "Label.h"
#include "Font.h"

#include "logging.h"

//#define DRAW_TEST_MONKEY

bool Renderer::init() {
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

	textureManager = std::shared_ptr<TextureManager>( new TextureManager() );
	shaderManager = std::shared_ptr<ShaderManager>( new ShaderManager() );
	VERIFY ( shaderManager->init(), "Unable to initialise renderer", return false );

	initContext();

	camera = std::shared_ptr<Camera>( new Camera() );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

	terrain = std::shared_ptr<Terrain>( new Terrain() );
	terrain->init( 0.1f, 256 );

#ifdef DRAW_TEST_MONKEY
	std::shared_ptr<Mesh> msh( new Mesh( "../models/monkey.obj", "../textures/testChecker.DDS" ) );
	msh->init( *textureManager );
	renderables.push_back( msh );
#endif

	GLuint fontTexture = textureManager->loadTexture( "../textures/defaultFont.DDS_0.dds" );
	defaultFont = std::shared_ptr<Font> (new Font( fontTexture ));

	return true;
}

void Renderer::initContext() {
	context.meshTextureUniformId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "texture" );
	context.meshMVPId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "mvp" );

	context.terrainMVPId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "mvp" );
	context.terrainMinMaxId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "minMax" );

	context.fontTextureId = shaderManager->getParam( ShaderManager::ShaderType::FONT_SHADER, "texture" );
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
}

void Renderer::renderMeshes() {
	shaderManager->useProgram( ShaderManager::ShaderType::MODEL_SHADER );

	for (PRenderable& renderable : renderables) {
		renderable->render( context );
	}
}

void Renderer::renderTerrain() {
	shaderManager->useProgram( ShaderManager::ShaderType::TERRAIN_SHADER );

	const glm::mat4 mvp = camera->getProjection() * camera->getView();
	glUniformMatrix4fv( context.terrainMVPId, 1, GL_FALSE, &mvp[0][0] );
	
	terrain->render( context );
}

void Renderer::renderTexts() {
	glDisable( GL_CULL_FACE );

	shaderManager->useProgram( ShaderManager::ShaderType::FONT_SHADER );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, defaultFont->getTextureId() );
	glUniform1i( context.meshTextureUniformId, 0 );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glRectf(-0.75f,0.75f, 0.75f, -0.75f);

	glEnable( GL_CULL_FACE );
}

void Renderer::shutdown() {
	for ( PRenderable& mesh : renderables ) {
		mesh->shutdown();
	}

	shaderManager->shutdown();
	textureManager->shutdown();
}