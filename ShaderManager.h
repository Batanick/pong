typedef unsigned int GLuint;

class ShaderManager final {

public:
	bool init();
	void shutdown();

	GLuint getProgramId();
	GLuint getMVPId();
	GLuint getTextureUniformId();

private:
	GLuint programID;
	GLuint mvpId;
	GLuint textureUniformId;
};