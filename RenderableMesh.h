#ifndef H_RENDERABLE
#define H_RENDERABLE

#include <vector>

#include "glm.hpp"

#include "RenderCommon.h"

class RenderableMesh : public Renderable {
public:

    void init();
    
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

protected: 
    virtual void initMesh( 
        std::vector<const glm::vec3> &vertices, 
        std::vector<const unsigned int> &indices,
        std::vector<const glm::vec2> &uvs) = 0;
    
    virtual void initTexture( GLuint textureId ) = 0;

private:

    GLuint vertexBuffer;
	GLuint indexBuffer;

	GLuint uvsBuffer;
	GLuint textureId;

	int indicesSize;

};

#endif