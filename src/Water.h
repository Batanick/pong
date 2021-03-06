#include <GL/glew.h>
#include "RenderCommon.h"

static const float WATER_SIZE = 10000.0f;
static const glm::vec3 COLOR(0.25f, 0.5f, 0.5f);

class Water final : public Renderable {

public:
    Water() {
        verticesId = -1;
        reflectionTexId = -1;
        mainColorId = -1;
        timeId = -1;
        projectionId = -1;
        viewId = -1;
        cameraPosId = -1;
        lightDirId = -1;
        cameraDirId = -1;
    }

    virtual void init(const GLuint shaderId) override;

    virtual void render(const RenderContext &context) override;

    virtual void shutdown() override;

    virtual ShaderType getType() override;

private:
    GLuint verticesId;

    GLint projectionId;
    GLint viewId;
    GLint cameraPosId;

    GLint reflectionTexId;
    GLint mainColorId;
    GLint lightDirId;
    GLint cameraDirId;
    GLint timeId;

};