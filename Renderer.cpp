#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>

#include "ShaderManager.h"
#include "AssetManager.h"

#include "Terrain.h"
#include "Bush.h"
#include "Tree.h"
#include "Label.h"
#include "Clouds.h"
#include "Sun.h"

#include "Camera.h"
#include "FpsCounter.h"
#include "TexturedFrame.h"

#include "logging.h"
#include "commonMath.h"

#define SHOW_FPS

#define DRAW_TERRAIN
//#define DRAW_GRASS
//#define DRAW_TREES
#define DRAW_SKYBOX
#define SECOND_CAMERA

bool Renderer::init() {
  VERIFY(glewInit() == GLEW_OK, "Unable to initialize glew", return false);

  assetManager = std::shared_ptr<AssetManager>(new AssetManager());
  VERIFY(assetManager->init(), "Unable to initialise asset manager", return false);

  shaderManager = std::shared_ptr<ShaderManager>(new ShaderManager());
  VERIFY(shaderManager->init(), "Unable to initialise shader manager", return false);

  fpsCounter = std::shared_ptr<FpsCounter>(new FpsCounter());

  camera = std::shared_ptr<Camera>(new Camera());
  glClearColor(0.0f, 0.635f, 0.91f, 0.0f);

  glfwSwapInterval(1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  initContext();
  initScene();

  return true;
}

void Renderer::initScene() {
#ifdef DRAW_TERRAIN
  std::shared_ptr<Terrain> terrain = std::shared_ptr<Terrain>(new Terrain());
  add(ShaderType::TERRAIN_SHADER, terrain);
#endif

#ifdef DRAW_SKYBOX
  std::shared_ptr<Clouds> clouds = std::shared_ptr<Clouds>(new Clouds());
  add(ShaderType::CLOUDS_SHADER, clouds);

  std::shared_ptr<TexturedMesh> sun = std::shared_ptr<TexturedMesh>(new Sun());
  add(ShaderType::TEX_MESH_SHADER, sun);
#endif

#ifdef SHOW_FPS
  this->fpsLabel = std::shared_ptr<Label>(new Label(assetManager->getDefaultFont(), 20, context.windowHeight - 50, glm::vec3(0, 1, 0)));
  add(ShaderType::FONT_SHADER, fpsLabel, RenderableType::GUI);

  this->cameraCoordsLabel = std::shared_ptr<Label>(new Label(assetManager->getDefaultFont(), 20, context.windowHeight - 80, glm::vec3(0, 1, 0)));
  add(ShaderType::FONT_SHADER, cameraCoordsLabel, RenderableType::GUI);
#endif

#ifdef SECOND_CAMERA
  std::shared_ptr<TexturedFrame> secondCamera = std::shared_ptr<TexturedFrame>(new TexturedFrame(0.5f, -1.0f, 0.5f, 0.5f, assetManager->getDefaultFont()->getTextureInfo() ));
  add(ShaderType::FONT_SHADER, secondCamera);
#endif

}

void Renderer::initContext() {
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  context.windowHeight = windowH;
  context.windowWidth = windowW;
  context.guiView = convertToViewMatrix(2 / (float)context.windowWidth, 2 / (float)context.windowHeight, -1, -1);
  context.time = 0;

  context.lightDir = glm::normalize(glm::vec3(1, 1, -1));
}

void Renderer::render(double timeDelta) {
  camera->onBeforeRender(window, timeDelta);

  context.timeDelta = timeDelta;
  context.time += timeDelta;
  context.cameraPos = camera->getPosition();

  const glm::mat4 view = camera->getView();
  const glm::mat4 projection = camera->getProjection();
  context.pv = projection * view;
  context.projection = projection;
  context.view = view;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderAll();

  fpsCounter->onFrame();
  char buff[128];
  sprintf_s(buff, "FPS: %.2f", fpsCounter->getFps());
  fpsLabel->setText(context, buff);

  const glm::vec3 pos = camera->getPosition();
  sprintf_s(buff, "%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
  cameraCoordsLabel->setText(context, buff);
}

void Renderer::add(ShaderType shaderType, PRenderable renderable, Renderer::RenderableType type) {
  const GLuint shaderId = shaderManager->getProgramId(shaderType);
  renderable->init(shaderId);

  renderables[type].insert(std::make_pair(shaderType, renderable));
}

void Renderer::renderAll() {
  ShaderType currentType = ShaderType::NONE;

  for (RenderableHolder holder : renderables) {
    // expecting that elements ordered by key
    for (auto iterator = holder.begin(); iterator != holder.end(); iterator++) {
      if (iterator->first != currentType) {
        currentType = iterator->first;
        shaderManager->useProgram(currentType);
      }

      iterator->second->render(context);
    }
  }
}

void Renderer::shutdown() {
  for (RenderableHolder holder : renderables)
    for (auto iterator = holder.begin(); iterator != holder.end(); iterator++) {
      iterator->second->shutdown();
    }

  shaderManager->shutdown();
  assetManager->shutdown();
}
