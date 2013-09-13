#include <vector>
#include <glm.hpp>

#include "RenderCommon.h"

typedef unsigned int GLuint;

class Mesh final {

public:
	Mesh( const std::string modelPath, const std::string texturePath ): 
		modelPath( modelPath ), 
		texturePath(texturePath ) {
	}

	Mesh( ):
		modelPath( "../models/monkey.obj" ), 
		texturePath( "../models/testTexture.DDS" ) {
	}

	~Mesh(){}

	void init();
	void render( const RenderContext context );
	void shutdown();

private:
	const std::string modelPath;
	const std::string texturePath;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;
	std::vector<glm::vec2> uvs;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint uvsBuffer;
	GLuint textureId;

	void loadTexture();
};