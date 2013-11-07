#include <map>

typedef unsigned int GLuint;
typedef std::string ShaderParam;
typedef std::map<ShaderParam, GLuint> ParamsHolder;

class ShaderDesc;

class ShaderManager final {

public:
	enum class ShaderType {
		MODEL_SHADER, TERRAIN_SHADER, FONT_SHADER
	};
	
	bool init();
	void shutdown();

	void useProgram( const ShaderType shaderType );
	GLuint getParam( const ShaderType shaderType, const ShaderParam param );
	
private:
	std::map<ShaderType, GLuint> typeToProgramId;
	std::map<ShaderType, ParamsHolder> typeToParams;
	bool loadShader( const ShaderDesc &desc );
};