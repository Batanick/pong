#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <algorithm>

#include "ShaderManager.h"
#include "AssetManager.h"

#include "ReflectionHolder.h"

#include "Terrain.h"
#include "Water.h"
#include "Label.h"
#include "Clouds.h"
#include "Sun.h"

#include "Tree.h"

#include "Camera.h"
#include "FpsCounter.h"

#include "commonMath.h"
#include "renderUtils.h"
#include "noise.h"

//#include "noise.h"

#define SHOW_FPS

#define DRAW_TERRAIN
#define DRAW_SKYBOX

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    addons.push_back(std::shared_ptr<RenderHandler>(new ReflectionHolder(*this)));

    for (PRenderHandler addon : addons) {
        addon->onInit();
    }

    initContext();
    initScene();

    return true;
}

void Renderer::initScene() {
    const float distance{1.0f};
    for (int i = 0; i < 25; i++) {
        add(std::shared_ptr<Tree>(new Tree(glm::vec3((i / 5) * distance, 0.0f, (i % 5) * distance))));
    }


#ifdef DRAW_TERRAIN
  add(std::shared_ptr<Terrain>(new Terrain()));
  add(std::shared_ptr<Water>(new Water()), CommonRenderer::PostRender);
#endif

#ifdef DRAW_SKYBOX
  add(std::shared_ptr<Clouds>(new Clouds()));
  add(std::shared_ptr<TexturedMesh>(new Sun()));
#endif

#ifdef SHOW_FPS
    fpsLabel = std::shared_ptr<Label>(
            new Label(assetManager->getDefaultFont(), 20, context.windowHeight - 50, glm::vec3(0, 1, 0)));
    add(fpsLabel, RenderableType::GUI);

    cameraCoordsLabel = std::shared_ptr<Label>(
            new Label(assetManager->getDefaultFont(), 20, context.windowHeight - 80, glm::vec3(0, 1, 0)));
    add(cameraCoordsLabel, RenderableType::GUI);

    statsLabel = std::shared_ptr<Label>(
            new Label(assetManager->getDefaultFont(), 20, context.windowHeight - 110, glm::vec3(0, 1, 0)));
    add(statsLabel, RenderableType::GUI);
#endif

    for (PRenderHandler addon : addons) {
        addon->onInitScene();
    }
}

void Renderer::initContext() {
    int windowW, windowH;
    glfwGetWindowSize(window, &windowW, &windowH);
    context.windowHeight = windowH;
    context.windowWidth = windowW;
    context.guiView = convertToViewMatrix(2 / (float) context.windowWidth, 2 / (float) context.windowHeight, -1, -1);
    context.time = 0;

    context.lightDir = glm::normalize(glm::vec3(1, 1, -1));
    for (PRenderHandler addon : addons) {
        addon->onContextInit(context);
    }
}

void Renderer::render(double timeDelta) {
    camera->onBeforeRender(window, timeDelta);
    context.timeDelta = timeDelta;
    context.time += timeDelta;

    setUpCamera(context, camera->getPosition(), camera->getUp(), camera->getDirection());

    for (PRenderHandler addon : addons) {
        addon->onBeforeRender(context);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderAll();

    fpsCounter->onFrame();
    char buff[128];
    sprintf(buff, "FPS: %.2f", fpsCounter->getFps());
    fpsLabel->setText(context, buff);

    const glm::vec3 pos = camera->getPosition();
    sprintf(buff, "%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
    cameraCoordsLabel->setText(context, buff);

    sprintf(buff, "Calls:%d", getHeightCalls());
    statsLabel->setText(context, buff);
}

void Renderer::add(const PRenderable renderable, Renderer::RenderableType type) {
    const GLuint shaderId = shaderManager->getProgramId(renderable->getType());
    renderable->init(shaderId);

    renderables[type].insert(std::make_pair(renderable->getType(), renderable));
}

void Renderer::render(const RenderableType &type, const RenderContext &context) {
    const RenderableHolder &holder = renderables[type];

    ShaderType currentType = ShaderType::NONE;
    for (auto &iterator : holder) {
        if (iterator.first != currentType) {
            currentType = iterator.first;
            shaderManager->useProgram(currentType);
        }

        iterator.second->render(context);
    }
}

void Renderer::renderAll() {
    ShaderType currentType = ShaderType::NONE;

    for (RenderableHolder &holder : renderables) {
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

    for (PRenderHandler addon : addons) {
        addon->onShutdown();
    }

    shaderManager->shutdown();
    assetManager->shutdown();
}