#include <map>
#include <vector>
#include <set>

typedef unsigned int GLuint;
typedef std::string ShaderParam;

class ShaderDesc;


enum class ShaderType : unsigned int {
    NONE, MODEL_SHADER, TERRAIN_SHADER, BUSH_SHADER, FONT_SHADER, TEST_SHADER
};

class ShaderManager final {

public:
	
	bool init();
	void shutdown();

	void useProgram( const ShaderType shaderType );
    GLuint getProgramId( const ShaderType shaderType );
	
private:
    typedef void (*ShaderInitializer) ();
    struct ShaderDesc {
        ShaderDesc( ShaderType shaderType, char * vertexShader, char * fragmentShader, ShaderInitializer initilizer):
            vertexShaderFName(vertexShader), 
            fragmentShaderFName(fragmentShader), 
            shaderType(shaderType), 
            initilizer(initilizer) { 
    	}
    
	    const std::string vertexShaderFName;
	    const std::string fragmentShaderFName;
        ShaderType shaderType;	
        ShaderInitializer initilizer;
    };


    struct ShaderParams {
        ShaderParams( ShaderType type, ShaderInitializer initilizer, GLuint id ):
            type(type),
            initilizer(initilizer),
            programId (id) {
        }

        GLuint programId;
        ShaderType type;
        ShaderInitializer initilizer;
    };

    static std::vector<ShaderDesc> buildShaders();
    std::map<ShaderType, const ShaderParams> shaders;
	bool loadShader( const ShaderDesc &desc );
};