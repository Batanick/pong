#include <vector>
#include <glm.hpp>

typedef unsigned int GLuint;
struct RenderContext;

class Mesh final {
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
	void render( const RenderContext context );
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