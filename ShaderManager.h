typedef unsigned int GLuint;

class ShaderManager final {

public:
	bool init();
	void shutdown();

	GLuint getProgramId();
	GLuint getMVPId();

private:
	GLuint programID;
	GLuint mvpId;
};