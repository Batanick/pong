#include "Game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>  
#include <time.h> 

#include <thread>
#include <chrono>

#include "Renderer.h"

#include "logging.h"

static int const SECOND = 1;

Game::Game() {
  running = false;
};

bool Game::init() {
  srand((unsigned int)time(NULL));

  if (!glfwInit()) {
    return false;
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1280, 720, "C++ Playground", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  renderer = std::shared_ptr<Renderer>(new Renderer(window));
  VERIFY(renderer->init(), "Unable to initialize renderer", return false);

  lastTick = glfwGetTime();

  //center cursor on the screen
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  glfwSetCursorPos(window, windowW / 2, windowH / 2);

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
  double lastTickTime = glfwGetTime();
  double currentTime = 0;
  double timeDelta = 0;

  glFinish();
  while (running) {
    currentTime = glfwGetTime();
    timeDelta = currentTime - lastTickTime;
    lastTickTime = currentTime;

    renderer->render(timeDelta);

    glfwSwapBuffers(window);
    glfwPollEvents();

    running = running & (!glfwWindowShouldClose(window));
  }

  onShutdown();
}

void Game::onShutdown() {
  renderer->shutdown();
}

bool Game::isRunning() {
  return running;
}