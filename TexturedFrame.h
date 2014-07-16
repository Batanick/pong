#include "RenderCommon.h"

class TexturedFrame final : public Renderable {

public:
  TexturedFrame(int x, int y, const GLuint textureId) :
    x(x), y(y), 
    textureId(textureId) {
  }

  virtual void render(const RenderContext &context) override;
  virtual void shutdown() override;
  virtual void init(const GLuint shaderId) override;

private:
  const int x, y;
  const GLuint textureId;


};