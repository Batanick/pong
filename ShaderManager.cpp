#include "ShaderManager.h"

#include <vector>
#include <set>

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>

class ShaderDesc final {
public:
	ShaderDesc( ShaderManager::ShaderType shaderType, char * vertexShader, char * fragmentShader, std::set<ShaderParam> params):
		vertexShaderFName(vertexShader), fragmentShaderFName(fragmentShader), shaderType(shaderType), params(params) { 
	}

	const std::string vertexShaderFName;
	const std::string fragmentShaderFName;
	 ShaderManager::ShaderType shaderType;	
	const std::set<ShaderParam> params;
};

// shaders list here
std::vector<ShaderDesc> buildShaders() {
	std::vector<ShaderDesc> shaders;

	//MESH_SHADER
	std::set<std::string> meshShaderParams;
	meshShaderParams.insert( "mvp" );
	meshShaderParams.insert( "texture" );

	shaders.push_back( ShaderDesc( 
		ShaderManager::ShaderType::MODEL_SHADER, 
		"MeshVertexShader.vertexshader", 
		"MeshFragmentShader.fragmentshader", 
		meshShaderParams ) );

	//TERRAIN_SHADER
	std::set<std::string> terrainShaderParams;
	terrainShaderParams.insert( "mvp" );
	terrainShaderParams.insert( "minMax" );

	shaders.push_back( ShaderDesc(
		ShaderManager::ShaderType::TERRAIN_SHADER, 
		"TerrainVertexShader.vertexshader", 
		"TerrainFragmentShader.fragmentshader", 
		terrainShaderParams ) );

	//FONT_SHADER
	std::set<std::string> fontShaderParams;
	fontShaderParams.insert( "texture" );

	shaders.push_back( ShaderDesc(
		ShaderManager::ShaderType::FONT_SHADER, 
		"FontVertexShader.vertexshader", 
		"FontFragmentShader.fragmentshader", 
		fontShaderParams ) );

	return shaders;
}

static const std::string SHADER_PATH = "../shaders/";
static const std::vector<ShaderDesc> shaders = buildShaders();

void loadShaderSource( GLuint shaderId, char const * source );
void printLog( GLuint obj );

bool ShaderManager::init() {
	for ( ShaderDesc desc : shaders ) {
		if ( !loadShader(desc) ){
			return false;
		}
	}

	return true;
}

bool ShaderManager::loadShader( const ShaderDesc &shaderDesc ){
	if ( typeToProgramId.find( shaderDesc.shaderType ) != typeToProgramId.end() ){
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

	typeToProgramId.insert( std::make_pair(shaderDesc.shaderType, programID) ) ;
	ParamsHolder params;
	for ( auto param : shaderDesc.params ) {
		GLuint paramId = glGetUniformLocation( programID, param.c_str() );
		if ( paramId == GL_INVALID_VALUE ) {
			LOG( "Incorrect shader parameter name: [%s]", param );
			continue;
		}

		params.insert ( std::make_pair(param, paramId) );
	}
	typeToParams.insert( std::make_pair(shaderDesc.shaderType, params) );

	//don't actually destroyed until program
	glDeleteShader( fragmentShaderId );
	glDeleteShader( vertextShaderId );

	return true;
}

void ShaderManager::shutdown() {
	for ( auto& entry :  typeToProgramId ) {
		glDeleteProgram( entry.second );
	}
}

void loadShaderSource( GLuint shaderId, char const * source) {
	glShaderSource(shaderId, 1, &source , NULL);
	glCompileShader(shaderId);

	printLog(shaderId);
}

void ShaderManager::useProgram( const ShaderManager::ShaderType shaderType ) {
	auto result = typeToProgramId.find( shaderType );
	if ( result != typeToProgramId.end() ) {
		glUseProgram( result->second );
	} else {
		LOG( "Unable to find program: [%d]", shaderType );
		return;
	}
}

GLuint ShaderManager::getParam( const ShaderType shaderType, const ShaderParam param ) {
	const auto paramsHolder = typeToParams.find( shaderType );
	if ( paramsHolder == typeToParams.end() ) {
		LOG( "Can't find params for shaderType: [%d]", shaderType );
		return GL_INVALID_VALUE;
	}

	auto paramId = paramsHolder->second.find( param );
	if ( paramId == paramsHolder->second.end() ) {
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

