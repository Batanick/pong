#ifndef H_RENDER_COMMON
#define H_RENDER_COMMON
#include "glm.hpp"
#include <memory>

static const float RENDER_PI = 3.14159265358979323846264338327950288f;
static const float RENDER_FOV = 45.0f;

static const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
static const glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

typedef unsigned int GLuint;

enum class ShaderType : unsigned int {
  NONE, MODEL_SHADER, TEX_MESH_SHADER, TERRAIN_SHADER, BUSH_SHADER, CLOUDS_SHADER, GUI_SHADER, TEST_SHADER
};

struct RenderContext {
  double time;
  double timeDelta;

  int windowHeight;
  int windowWidth;

  glm::mat4 pv;
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec3 cameraPos;
  glm::vec3 cameraUp;
  glm::vec3 cameraDir;

  glm::mat3 guiView;

  glm::vec3 lightDir;

  GLuint reflectionTexId;
};

struct Glyph {
  Glyph() {
  }
  Glyph(int key, int x, int y, int width, int height) :
    key(key),
    x(x), y(y),
    width(width),
    height(height){
  }

  int key;
  int x, y;
  int width, height;
};

struct TextureInfo {
  TextureInfo(){
  }

  TextureInfo(GLuint textureId, unsigned int width, unsigned int height) :
    textureId(textureId),
    height(height),
    width(width){
  }

  GLuint textureId;
  unsigned int height;
  unsigned int width;
};

class Renderable {
public:
  virtual void render(const RenderContext &context) = 0;
  virtual void shutdown() = 0;
  virtual void init(const GLuint shaderId) = 0;
  virtual ShaderType getType() = 0;
};

typedef std::shared_ptr<Renderable> PRenderable;

class CommonRenderer {
public:
  enum RenderableType : unsigned char {
    Common = 0, PostRender, GUI
  };

  virtual void renderAll() = 0;
  virtual void render(const RenderableType &type, const RenderContext &context) = 0;
  virtual void add(PRenderable renderable, RenderableType type = RenderableType::Common) = 0;
};

class RenderHandler {
public:
  virtual void onInit(){};
  virtual void onContextInit(RenderContext &context){};
  virtual void onInitScene(){};
  virtual void onBeforeRender(const RenderContext &context){};
  virtual void onShutdown(){};
};

#endif