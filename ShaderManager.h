#include <map>
#include <vector>
#include <set>

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
    typedef void (*ShaderInitializer) ();
    struct ShaderDesc {
        ShaderDesc( ShaderManager::ShaderType shaderType, char * vertexShader, char * fragmentShader, std::set<ShaderParam> params, ShaderInitializer initilizer):
            vertexShaderFName(vertexShader), 
            fragmentShaderFName(fragmentShader), 
            shaderType(shaderType), 
            params(params), 
            initilizer(initilizer) { 
    	}
    
	    const std::string vertexShaderFName;
	    const std::string fragmentShaderFName;
        ShaderManager::ShaderType shaderType;	
	    const std::set<ShaderParam> params;
        ShaderInitializer initilizer;
    };


    struct ShaderParams {
        ShaderParams( ShaderType type, ParamsHolder &holder, ShaderInitializer initilizer, GLuint id ):
            type(type),
            params(holder),
            initilizer(initilizer),
            programId (id) {
        }

        GLuint programId;
        ShaderType type;
        ParamsHolder params;
        ShaderInitializer initilizer;
    };

    static std::vector<ShaderDesc> buildShaders();
    std::map<ShaderType, const ShaderParams> shaders;
	bool loadShader( const ShaderDesc &desc );
};