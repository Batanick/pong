#include "RenderCommon.h"
#include <vector>
#include <memory>

#include "TerrainConstants.h"
#include "Patches.h"

class Terrain final : public Renderable {

public:
    Terrain(): 
		patches( new Patches() ) {
        position = glm::vec3();

		mvpId = -1;
		textureId = -1; 
		heightId = -1;
	}

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
private :
    struct IndexBuffer {
        GLuint id;
        int length;
    };

	void initTexture( const GLuint &shaderId );
	void initVertices( const GLuint &shaderId );
	void initIndices( const GLuint &shaderId );

    std::vector<IndexBuffer> indexBuffers;

    glm::vec3 position;
    
    GLuint mvpId;
	GLuint textureParamId;
	GLuint heightId;

	GLuint textureId;

	std::unique_ptr<Patches> patches;

	void generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod );
	
    void rebuildTerrain( const float &dx, const float &dz);
};