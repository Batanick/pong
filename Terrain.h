#include "RenderCommon.h"
#include <vector>
#include <memory>

#include "TerrainConstants.h"

class Terrain final : public Renderable {

public:
    Terrain() {
        position = glm::vec3();
		refreshPos = 0;

		mvpId = -1;
		textureId = -1;
	}

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
	virtual bool refresh( const RenderContext &context ) override;
    
    glm::vec3 getRandomPos();
    float getHeight( float x, float y );

private :
    struct IndexBuffer {
        GLuint id;
        int length;
    };

    struct Patch {
		Patch() {
			x = -1;
			y = -1;
		}

        GLuint id;
        int lod;
		std::vector<glm::vec3> vertices;
		int x, y;
    };

	void initTexture( const GLuint &shaderId );
	void initVertices( const GLuint &shaderId );
	void initIndices( const GLuint &shaderId );

    std::vector<Patch> patches;
    std::vector<IndexBuffer> indexBuffers;
    std::vector<int> indexToLod;

    glm::vec3 position;
    
    GLuint mvpId;
	GLuint textureParamId;
	GLuint heightId;

	GLuint textureId;

	unsigned int refreshPos;

	void generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod );
	
    void rebuildTerrain( const float &dx, const float &dz);
    void reinitPatch( Patch &patch, const int x, const int y, int lod );
};