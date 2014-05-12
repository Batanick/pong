#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "AssetManager.h"
#include "Terrain.h"
#include "Bush.h"
#include "Tree.h"
#include "Camera.h"
#include "Label.h"
#include "FpsCounter.h"

#include "logging.h"

#define SHOW_FPS

#define DRAW_TERRAIN
//#define DRAW_GRASS
//#define DRAW_TREES

bool Renderer::init() {
	VERIFY (glewInit() == GLEW_OK, "Unable to initialize glew", return false);

    assetManager = std::shared_ptr<AssetManager>( new AssetManager() );
    VERIFY ( assetManager->init(), "Unable to initialise asset manager", return false );

	shaderManager = std::shared_ptr<ShaderManager>( new ShaderManager() );
	VERIFY ( shaderManager->init(), "Unable to initialise shader manager", return false );

    fpsCounter = std::shared_ptr<FpsCounter>( new FpsCounter() );

	camera = std::shared_ptr<Camera>( new Camera() );
	glClearColor(0.0f, 0.0f, 0.6f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); 

    initContext();
    initScene();

	return true;
}

void Renderer::initScene() {
#ifdef DRAW_TERRAIN
    std::shared_ptr<Terrain> terrain = std::shared_ptr<Terrain>( new Terrain() );
    add( ShaderType::TERRAIN_SHADER ,terrain );
#endif
      
#ifdef DRAW_GRASS
    static const int bushes = tiles * 4;
    for (int i = 0; i < bushes; i++) {
        std::shared_ptr<Bush> bush = std::shared_ptr<Bush>( new Bush( terrain->getRandomPos(), 64 ) ) ;
        add ( ShaderType::BUSH_SHADER, bush );
    }
#endif

#ifdef DRAW_TREES
    std::shared_ptr<Tree> tree = std::shared_ptr<Tree> ( new Tree() );
    add ( ShaderType::BUSH_SHADER, tree );
#endif

#ifdef SHOW_FPS
    this->fpsLabel = std::shared_ptr<Label> (new Label(assetManager->getDefaultFont(), "DUMMY", 20, context.windowHeight - 50, glm::vec3(0,1,0)) );
    add( ShaderType::FONT_SHADER, fpsLabel );

    this->cameraCoords = std::shared_ptr<Label> (new Label(assetManager->getDefaultFont(), "DUMMY", 20, context.windowHeight - 80, glm::vec3(0,1,0)) );
    add( ShaderType::FONT_SHADER, cameraCoords );
#endif
}

void Renderer::initContext() {
    int windowW, windowH;
	glfwGetWindowSize( window, &windowW, &windowH );
    context.windowHeight = windowH;
    context.windowWidth = windowW;
}

void Renderer::render( double timeDelta ) {
	camera->onBeforeRender( window, timeDelta );

	context.timeDelta = timeDelta;
    context.cameraPos = camera->getPosition();

	const glm::mat4 view = camera->getView();
	const glm::mat4 projection = camera->getProjection();
	context.pv = projection * view;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderAll();

    glfwSwapBuffers(window);

    fpsCounter->onFrame();
    char buff[128];
    sprintf_s(buff, "FPS: %.2f", fpsCounter->getFps());
    fpsLabel->setText(buff);

    const glm::vec3 pos = camera->getPosition();
    sprintf_s(buff, "%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
    cameraCoords->setText(buff);
}

void Renderer::add( ShaderType type, PRenderable renderable ) {
    const GLuint shaderId = shaderManager->getProgramId(type);
    renderable->init(shaderId);

    renderables.insert( std::make_pair(type, renderable) );
}

void Renderer::renderAll() {
    // expecting that elements ordered by key
    ShaderType currentType = ShaderType::NONE;
    for ( auto iterator = renderables.begin(); iterator != renderables.end();iterator++ ) {
        if ( iterator->first != currentType ) {
            currentType = iterator->first;
            shaderManager->useProgram(currentType);
        }

        iterator->second->render( context );
    }
}

void Renderer::shutdown() {
	for ( auto iterator = renderables.begin(); iterator != renderables.end();iterator++ ) {
        iterator->second->shutdown();
	}

	shaderManager->shutdown();
	assetManager->shutdown();
}
