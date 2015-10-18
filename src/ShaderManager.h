#include <map>
#include <vector>
#include <set>

#include "RenderCommon.h"

typedef std::string ShaderParam;

class ShaderManager final {

public:

    bool init();

    void shutdown();

    void useProgram(const ShaderType shaderType);

    GLuint getProgramId(const ShaderType shaderType);

    typedef void(*ShaderInitializer)();

    struct ShaderDesc {
        ShaderDesc(ShaderType shaderType, std::string vertexShader, std::string fragmentShader,
                   ShaderInitializer initilizer) :
                vertexShaderFName(vertexShader),
                fragmentShaderFName(fragmentShader),
                shaderType(shaderType),
                initilizer(initilizer) {
        }

        std::string vertexShaderFName;
        std::string fragmentShaderFName;
        ShaderType shaderType;
        ShaderInitializer initilizer;
    };

private:

    struct ShaderParams {
        ShaderParams(ShaderType type, ShaderInitializer initilizer, GLuint id) :
                type(type),
                initilizer(initilizer),
                programId(id) {
        }

        GLuint programId;
        ShaderType type;
        ShaderInitializer initilizer;
    };

    std::map<ShaderType, const ShaderParams> shaders;

    bool loadShader(const ShaderDesc &desc);
};