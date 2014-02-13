#include "ShaderManager.h"

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void initTextShader() {
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glEnable( GL_BLEND );
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void initTerrainShader() {
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glDisable( GL_BLEND );
}


// shaders list here
std::vector<ShaderManager::ShaderDesc> ShaderManager::buildShaders() {
	std::vector<ShaderDesc> shaders;

	// ============== MESH_SHADER ================
	std::set<std::string> meshShaderParams;
	meshShaderParams.insert( "mvp" );
	meshShaderParams.insert( "texture" );

	shaders.push_back( ShaderDesc( 
		ShaderType::MODEL_SHADER, 
		"MeshVertexShader.vertexshader", 
		"MeshFragmentShader.fragmentshader", 
        meshShaderParams, 0 ) );

	// ============== TERRAIN_SHADER ============== 
	std::set<std::string> terrainShaderParams;
	terrainShaderParams.insert( "mvp" );
	terrainShaderParams.insert( "minMax" );

	shaders.push_back( ShaderDesc(
		ShaderType::TERRAIN_SHADER, 
		"TerrainVertexShader.vertexshader", 
		"TerrainFragmentShader.fragmentshader", 
		terrainShaderParams, &initTerrainShader ) );

	// ============== FONT_SHADER ============== 
	std::set<std::string> fontShaderParams;
	fontShaderParams.insert( "texture" );
    fontShaderParams.insert( "fontColor" );

	shaders.push_back( ShaderDesc(
		ShaderType::FONT_SHADER, 
		"FontVertexShader.vertexshader", 
		"FontFragmentShader.fragmentshader", 
		fontShaderParams,
        &initTextShader) );

	return shaders;
}

static const std::string SHADER_PATH = "../shaders/";

void loadShaderSource( GLuint shaderId, char const * source );
void printLog( GLuint obj );

bool ShaderManager::init() {
    std::vector<ShaderManager::ShaderDesc> shaderDescs = buildShaders();
    for ( ShaderDesc desc : shaderDescs ) {
		if ( !loadShader(desc) ){
			return false;
		}
	}

	return true;
}

bool ShaderManager::loadShader( const ShaderDesc &shaderDesc ){
	if ( shaders.find( shaderDesc.shaderType ) != shaders.end() ){
		LOG ( "Attempt to load more that one shader programm of the same type" );
		return false;
	}

	//Vertex shader
	std::string vertexShaderContent;
	VERIFY( loadContent( ( SHADER_PATH + shaderDesc.vertexShaderFName).c_str(), vertexShaderContent ), 
		"Unable to load fragment vertex program", return false);

	const GLuint vertextShaderId = glCreateShader(GL_VERTEX_SHADER);
	loadShaderSource(vertextShaderId, vertexShaderContent.c_str());

	//Fragment shader
	std::string fragmentShaderContent;
	VERIFY( loadContent( (SHADER_PATH + shaderDesc.fragmentShaderFName).c_str(), fragmentShaderContent) , 
		"Unable to load fragment shader program", return false);

	const GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	loadShaderSource( fragmentShaderId, fragmentShaderContent.c_str());

	//Program
	const GLuint programID = glCreateProgram();
	glAttachShader(programID, fragmentShaderId);
	glAttachShader(programID, vertextShaderId);
	glLinkProgram(programID);

	printLog(programID);

	ParamsHolder params;
	for ( auto param : shaderDesc.params ) {
		GLuint paramId = glGetUniformLocation( programID, param.c_str() );
		if ( paramId == GL_INVALID_VALUE ) {
			LOG( "Incorrect shader parameter name: [%s]", param );
			continue;
		}

		params.insert ( std::make_pair(param, paramId) );
	}
    printLog(programID);

    ShaderManager::ShaderParams shaderParams(shaderDesc.shaderType, params, shaderDesc.initilizer, programID);
	shaders.insert( std::make_pair(shaderDesc.shaderType, shaderParams) );

	//don't actually destroyed until program
	glDeleteShader( fragmentShaderId );
	glDeleteShader( vertextShaderId );

	return true;
}

void ShaderManager::shutdown() {
	for ( auto& entry : shaders ) {
        glDeleteProgram( entry.second.programId );
	}
}

void loadShaderSource( GLuint shaderId, char const * source) {
	glShaderSource(shaderId, 1, &source , NULL);
	glCompileShader(shaderId);

	printLog(shaderId);
}

void ShaderManager::useProgram( const ShaderType shaderType ) {
	auto result = shaders.find( shaderType );
	if ( result != shaders.end() ) {
        if (result->second.initilizer) {
            result->second.initilizer();
        }

        glUseProgram( result->second.programId );
	} else {
		LOG( "Unable to find program: [%d]", shaderType );
		return;
	}
}

GLuint ShaderManager::getParam( const ShaderType shaderType, const ShaderParam param ) {
	const auto paramsHolder = shaders.find( shaderType );
	if ( paramsHolder == shaders.end() ) {
		LOG( "Can't find params for shaderType: [%d]", shaderType );
		return GL_INVALID_VALUE;
	}

    auto paramId = paramsHolder->second.params.find( param );
    if ( paramId == paramsHolder->second.params.end() ) {
		LOG( "Can't find param id for [%s] in shader type [%s] params", shaderType, param );
		return GL_INVALID_VALUE;
	}

	return paramId->second;
}

void printLog(GLuint obj) {
	int infologLength = 0;
	int maxLength = 1024;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	char infoLog[1024];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
		LOG( "%s\n",infoLog );
}
