#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RenderCommon.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Camera.h"

#include "logging.h"

void Renderer::render( const RenderContext &context ){
	camera->onBeforeRender( context );
	const glm::mat4 view = camera->getView();
	const glm::mat4 projection = camera->getProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (PMesh& mesh : meshes) {
		const glm::mat4 mvp = projection * view * mesh->getModelTrans();
		mesh->render( mvp );
	}

    glfwSwapBuffers(window);
}

bool Renderer::init(){
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

	shaderManager = std::shared_ptr<ShaderManager> (new ShaderManager());
	VERIFY (shaderManager->init(), "Unable to initialise renderer", return false );

	camera = std::shared_ptr<Camera> (new Camera());
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	PMesh msh =  PMesh( new Mesh( shaderManager->getMVPId()) );
	msh->init();

	meshes.push_back( msh );

	return true;
}

void Renderer::shutdown() {
	for ( PMesh& mesh : meshes ) {
		mesh->shutdown();
	}

	shaderManager->shutdown();
}