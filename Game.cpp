#include "Game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>  
#include <time.h> 

#include "Renderer.h"

#include "logging.h"

static int const SECOND = 1;

class Game::Timer {
public:
	Timer() {
		lastTickTime = 0;
		fpsCounter = 0;
		currentFps = 0;
		timeCounter = 0;
	}

	double onTick() {
		const double currentTime = glfwGetTime();
		fpsCounter++;
		
		const double timeDelta = currentTime - lastTickTime;
		timeCounter += timeDelta;
		if ( timeCounter > SECOND ) {
			currentFps = (fpsCounter) / timeCounter;

			fpsCounter = 0;
			timeCounter = 0;

			LOG ( "FPS: %f", currentFps );
		}

		lastTickTime = currentTime;
		return timeDelta;
	}

	double getFps() {
		return currentFps;
	}

private :
	double lastTickTime;

	double currentFps;
	int fpsCounter;
	double timeCounter;
};

Game::Game() {
	running = false;
	timer = std::shared_ptr<Timer> ( new Timer() );
};

bool Game::init() {
	srand( (unsigned int)time(NULL) );

	if (!glfwInit()) {
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "C++ Playground", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }

	/* Make the window's context current */
    glfwMakeContextCurrent(window);

	renderer = std::shared_ptr<Renderer>(new Renderer(window));
	VERIFY (renderer->init(), "Unable to initialize renderer", return false);

	lastTick = glfwGetTime();

	//center cursor on the screen
	int windowW, windowH;
	glfwGetWindowSize( window, &windowW, &windowH );
	glfwSetCursorPos(window, windowW/2 , windowH/2);

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
	while (running) {
		const double deltaTime = timer->onTick();

		renderer->render( deltaTime );
	
        glfwPollEvents();

		running = running & (!glfwWindowShouldClose(window));
    }

	onShutdown();
}

void Game::onShutdown() {
	renderer->shutdown();
}