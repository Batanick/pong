typedef unsigned int GLuint;

class ShaderManager final {
public:
	bool init();
	GLuint getProgramId();

private:
	GLuint programID;
};