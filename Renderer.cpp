#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Camera.h"

#include <gtc\matrix_transform.hpp>

#include "logging.h"

#define LOAD_TEST_MONKEY

bool Renderer::init(){
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

	textureManager = std::shared_ptr<TextureManager>( new TextureManager() );
	shaderManager = std::shared_ptr<ShaderManager>( new ShaderManager() );
	VERIFY (shaderManager->init(), "Unable to initialise renderer", return false );

	camera = std::shared_ptr<Camera>( new Camera() );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

	terrain = std::shared_ptr<Terrain>( new Terrain() );
	terrain->init( 0.1f, 256 );

	context.meshTextureUniformId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "texture" );
	context.meshMVPId = shaderManager->getParam( ShaderManager::ShaderType::MODEL_SHADER, "mvp" );

	context.terrainMVPId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "mvp" );
	context.terrainMinMaxId = shaderManager->getParam( ShaderManager::ShaderType::TERRAIN_SHADER, "minMax" );

#ifdef LOAD_TEST_MONKEY
	PMesh msh =  PMesh( new Mesh() );
	msh->init( *textureManager );
	meshes.push_back( msh );
#endif

	return true;
}

void Renderer::render( double timeDelta ){
	camera->onBeforeRender( window, timeDelta );

	context.timeDelta = timeDelta;
	const glm::mat4 view = camera->getView();
	const glm::mat4 projection = camera->getProjection();
	context.pv = projection * view;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderTerrain();
	renderMeshes();

    glfwSwapBuffers(window);
}

void Renderer::renderMeshes() {
	shaderManager->useProgram( ShaderManager::ShaderType::MODEL_SHADER );

	static int frameNum = 0;
	for (PMesh& mesh : meshes) {
		const glm::mat4 mvp = context.pv* glm::rotate(glm::mat4(1), ((float) (frameNum++)), glm::vec3(0,1,0)); 
		glUniformMatrix4fv( context.meshMVPId, 1, GL_FALSE, &mvp[0][0] );

		mesh->render( context );
	}
}

void Renderer::renderTerrain() {
	shaderManager->useProgram( ShaderManager::ShaderType::TERRAIN_SHADER );

	const glm::mat4 mvp = camera->getProjection() * camera->getView();
	glUniformMatrix4fv( context.terrainMVPId, 1, GL_FALSE, &mvp[0][0] );
	
	terrain->render( context );
}

void Renderer::shutdown() {
	for ( PMesh& mesh : meshes ) {
		mesh->shutdown();
	}

	shaderManager->shutdown();
}