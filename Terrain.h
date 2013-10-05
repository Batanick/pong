#include "RenderCommon.h"

class Terrain final : public Renderable {

public:
	Terrain():
		vertexBuffer(-1),
		indexBuffer(-1),
		indicesSize(-1) {
	}

	void init( const float tileSize, const int widthTiles, const int heightTiles );
	virtual void render( const RenderContext &context );

private :
	GLuint vertexBuffer;
	GLuint indexBuffer;
	int indicesSize;
};