#include "RenderCommon.h"

class Terrain final : public Renderable {

public:
	Terrain():
		tileSize(1.0f),
		widthTiles(32),
		heightTiles(32),
		vertexBuffer(-1),
		indexBuffer(-1),
		indicesSize(-1) {
	}

	void init();
	virtual void render( const RenderContext &context );

private :
	const float tileSize;
	const int widthTiles;
	const int heightTiles;()

	GLuint vertexBuffer;
	GLuint indexBuffer;
	int indicesSize;

};