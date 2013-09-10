#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const GLfloat triangle[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void Mesh::load( GLuint shaderProgramId ) {
	for each (GLfloat val in triangle) {
		vertices.push_back(val);
	}
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	modelMatrix = glGetUniformLocation(shaderProgramId, "mvp");
}

void Mesh::render( glm::mat4 mvp ) {
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
 
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

glm::mat4 Mesh::getModelTrans() {
	return model;
}

