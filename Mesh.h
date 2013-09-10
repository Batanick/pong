#include <vector>
#include <glm.hpp>

typedef unsigned int GLuint;

class Mesh final {

public:
	Mesh() {
		model = glm::mat4(1);
		model[3][3] = 1;		
	}
	~Mesh(){}

	void load( GLuint shaderProgramId );
	void render( glm::mat4 mvp );
	glm::mat4 getModelTrans();

private:
	std::vector<float> vertices;
	std::vector<short> indices;

	GLuint vertexBuffer;
	GLuint modelMatrix;

	glm::mat4 model;
};