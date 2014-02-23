#include "RenderCommon.h"
#include <vector>

class Bush final : public Renderable {

public:
    Bush(glm::vec3 pos, int leafsCount, float height, float width):
        pos(pos),
        leafsCount(leafsCount),
        height(height), 
        width(width){
    }

    Bush( glm::vec3 pos, int leafsCount ):
        pos(pos),
        leafsCount(leafsCount), 
        height(0.5f), 
        width(0.05f){
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

    int const leafsCount;

    float const height;
    float const width;

    Leaf createLeaf( glm::vec3 pos, float height, float maxRotationAngle, float localYaw );
};