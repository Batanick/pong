#include "GuiElement.h"

class TexturedFrame final : public GuiElement {

public:
  TexturedFrame(float x, float y, float width, float height, const TextureInfo &textureInfo) :
    x(x), y(y),
    width(width),
    height(height),
    textureInfo(textureInfo) {
    colorId = -1;
  }

private:
  const float x, y;
  const float width, height;
  const TextureInfo textureInfo;

  GLuint colorId;

  virtual void onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId) override;
};