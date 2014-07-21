#include "renderCommon.h"

#include <memory>

//#define SHOW_DEBUG_FRAME

class ReflectionHolder final : public RenderHandler {

public:
  ReflectionHolder(CommonRenderer &renderer) :
    renderer(renderer) {
    textureId = -1;
  }

  virtual void onInit() override;
  virtual void onContextInit(RenderContext &context) override;
  virtual void onBeforeRender(const RenderContext &context) override;
  virtual void onInitScene() override;
  virtual void onShutdown() override;

private:
  GLuint textureId;
  GLuint frameBuffer;
  GLuint depthBuffer;

  CommonRenderer &renderer;

};