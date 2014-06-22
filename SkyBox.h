#include "RenderCommon.h"

static const float SIZE = 1000.0f;

class SkyBox final : public Renderable  {
public:
  SkyBox() {
    verticesId = -1;
    indicesId = -1;
    cameraPosId = -1;
  }

  virtual void init(const GLuint shaderId) override;
  virtual void render(const RenderContext &context) override;
  virtual void shutdown() override;

private:
  GLuint verticesId;
  GLuint indicesId;
  GLuint cameraPosId;
  GLuint mvpId;
};