#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "Mesh.h"
#include "Camera.h"

#include "logging.h"

void Renderer::render( double timeDelta ){
	camera->onBeforeRender( window, timeDelta );

	const glm::mat4 view = camera->getView();
	const glm::mat4 projection = camera->getProjection();

	RenderContext context;
	context.timeDelta = timeDelta;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (PMesh& mesh : meshes) {
		const glm::mat4 mvp = projection * view * mesh->getModelTrans();
		glUniformMatrix4fv( shaderManager->getMVPId() , 1, GL_FALSE, &mvp[0][0] );

		mesh->render( context );
	}

    glfwSwapBuffers(window);
}

bool Renderer::init(){
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

	shaderManager = std::shared_ptr<ShaderManager> (new ShaderManager());
	VERIFY (shaderManager->init(), "Unable to initialise renderer", return false );

	camera = std::shared_ptr<Camera> (new Camera());
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	PMesh msh =  PMesh( new Mesh() );
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