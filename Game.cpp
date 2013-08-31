#include "Game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool Game::init() {
	if (!glfwInit()) {
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }

	/* Make the window's context current */
    glfwMakeContextCurrent(window);

	return true;
}

void Game::start() {
	running = true;
	runMainLoop();
}

void Game::shutdown() {
	running = false;
    glfwTerminate();
}

void Game::runMainLoop() {
	/* Loop until the user closes the window */
    while (running)
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

		running = running & (!glfwWindowShouldClose(window));
    }
}