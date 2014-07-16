#include "RenderCommon.h"

#include <vector>

class GuiElement : public Renderable {
public:
  GuiElement() {
    vertexBuffer = -1;
    uvBuffer = -1;
    textureId = -1;
    textureParamId = -1;

    loaded = false;
  }

  virtual void render(const RenderContext &context) final override;
  virtual void shutdown() final override;
  virtual void init(const GLuint shaderId) final override;

protected:
  void setVertices(const std::vector<glm::vec2> &uvs, const std::vector<glm::vec2> &vertices);

private:

  GLuint textureId;
  GLuint textureParamId;

  GLuint vertexBuffer;
  GLuint uvBuffer;

  std::vector<glm::vec2> uvs;
  std::vector<glm::vec2> vertices;
  
  bool loaded;

  void reinit();

  virtual void onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId){};
  virtual void onBeforeRender(const RenderContext &context){};
};