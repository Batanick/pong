#include "RenderCommon.h"

static const float SIZE = 10000.0f;
static const int SKY_LEVELS_COUNT = 16;
static const float HEIGHT = 500;
static const float HEIGHT_STEP = 10;

class Clouds final : public Renderable {
public:
    Clouds() {
        verticesId = 0;
        cameraPosId = 0;
        pvId = 0;
        timeId = 0;
    }

    virtual void init(const GLuint shaderId) override;

    virtual void render(const RenderContext &context) override;

    virtual void shutdown() override;

    virtual ShaderType getType() final

    override;

private:
    GLuint verticesId;
    GLuint cameraPosId;
    GLuint pvId;
    GLuint timeId;

    GLuint textureId;
};