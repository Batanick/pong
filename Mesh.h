#include "RenderCommon.h"

#include <vector>
#include <glm.hpp>

class Mesh final : public Renderable {
public:
	Mesh( const std::string modelPath, const std::string texturePath ): 
		modelPath( modelPath ), 
		texturePath(texturePath ) {
		indicesSize = 0;
	}

	Mesh( ):
		modelPath( "../models/monkey.obj" ), 
		texturePath( "../models/testTexture.DDS" ) {
		indicesSize = 0;
	}

	~Mesh(){}

	void init();
	virtual void render( const RenderContext &context );
	void shutdown();

private:
	const std::string modelPath;
	const std::string texturePath;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint uvsBuffer;
	GLuint textureId;

	int indicesSize;

	void Mesh::loadTexture( std::vector<glm::vec2> &uvs );
};