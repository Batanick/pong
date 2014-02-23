#include "RenderCommon.h"
#include <vector>

class Bush final : public Renderable {

public:
    Bush():
        heightSegments(16),
        height(1.0f),
        width(0.05f), 
        pos(glm::vec3(1,0.2,1)) {
    }

    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

    void init();

private:
    struct Leaf {
        GLuint vertexBuffer;
	    GLuint indexBuffer;
        unsigned int indexSize;
    };
    glm::vec3 pos;

    std::vector<const Leaf> leafs;

    char const heightSegments;
    float const height;
    float const width;

    Leaf createLeaf( glm::vec3 pos, float maxRotationAngle, float localYaw );
};