#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Renderer::render(){
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}