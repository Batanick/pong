#include <memory>

#include "GuiElement.h"
#include "Font.h"
#include <glm/glm.hpp>

class Label final : public GuiElement {

public:
    Label(std::shared_ptr<Font> font, int x, int y, glm::vec3 color) :
            font(font),
            color(color),
            x(x), y(y) {
    }

    void setText(const RenderContext &context, std::string text);

private:
    const int x, y;
    const std::shared_ptr<Font> font;
    std::string str;
    const glm::vec3 color;

    GLuint fontColorId;

    virtual void onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId) override;

    virtual void onBeforeRender(const RenderContext &context) override;
};