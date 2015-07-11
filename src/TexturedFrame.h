#include "GuiElement.h"

class TexturedFrame final : public GuiElement {

public:
    TexturedFrame(float x, float y, float width, float height, const GLuint textureId) :
            x(x), y(y),
            width(width),
            height(height),
            textureId(textureId) {
        colorId = -1;
    }

private:
    const float x, y;
    const float width, height;
    const GLuint textureId;

    GLuint colorId;

    virtual void onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId) override;

    virtual void onBeforeRender(const RenderContext &context) override;
};