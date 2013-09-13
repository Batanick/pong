#include <vector>
#include <glm.hpp>

#include "RenderCommon.h"

typedef unsigned int GLuint;

class Mesh final {

public:
	Mesh() {
		model = glm::mat4(1);
	}
	~Mesh(){}

	void init();
	void render( const RenderContext context );
	void shutdown();
	glm::mat4 getModelTrans();

private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;

	GLuint vertexBuffer;
	GLuint indexBuffer;

	glm::mat4 model;

};