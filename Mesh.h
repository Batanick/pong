#include "RenderCommon.h"

#include <vector>
#include <glm.hpp>

class TextureManager;

class Mesh final : public Renderable {
public:
	Mesh( const std::string modelPath, const std::string texturePath ): 
		modelPath( modelPath ), 
		texturePath(texturePath ) {
		indicesSize = 0;
	}

	void init( TextureManager &textureManager );
	virtual void render( const RenderContext &context );
	virtual void shutdown();

private:
	const std::string modelPath;
	const std::string texturePath;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint uvsBuffer;
	GLuint textureId;

	int indicesSize;

	void Mesh::loadTexture( TextureManager &textureManager, std::vector<glm::vec2> &uvs );
};