#include <vector>
#include <glm.hpp>

typedef unsigned int GLuint;

class Mesh final {

public:
	Mesh( GLuint modelMatrix ): modelMatrix( modelMatrix ) {
		model = glm::mat4(1);
	}
	~Mesh(){}

	void init();
	void render( glm::mat4 mvp );
	void shutdown();
	glm::mat4 getModelTrans();

private:
	const GLuint modelMatrix;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;

	GLuint vertexBuffer;
	GLuint indexBuffer;

	glm::mat4 model;

};