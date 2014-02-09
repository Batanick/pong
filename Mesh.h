#include "RenderCommon.h"

#include <vector>
#include <glm.hpp>

class AssetManager;

class Mesh final : public Renderable {
public:
	Mesh( const std::string modelPath, const std::string texturePath ): 
		modelPath( modelPath ), 
		texturePath(texturePath ) {
		indicesSize = 0;
	}

    void init( AssetManager &manager );
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

private:
	const std::string modelPath;
	const std::string texturePath;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint uvsBuffer;
	GLuint textureId;

	int indicesSize;

    void Mesh::loadTexture( AssetManager &manager, std::vector<glm::vec2> &uvs );
};