#include "RenderCommon.h"
#include <vector>

class Terrain final : public Renderable {

public:
	Terrain():
		vertexBuffer(-1),
		indexBuffer(-1),
		indicesSize(-1),
		maxHeight(-1),
		minHeight(-1) {
	}

	void init( const float tileSize, const int tiles );
	virtual void render( const RenderContext &context );
	virtual void shutdown();

private :
	GLuint vertexBuffer;
	GLuint indexBuffer;
	int indicesSize;

	float minHeight;
	float maxHeight;

	void generateIndicies( const int res, std::vector<unsigned int> &indices );
	void generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices );
};