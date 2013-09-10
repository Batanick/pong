#include "ShaderManager.h"

#include <vector>

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>

void loadShader( GLuint shaderId, char const * source);
void printLog(GLuint obj);

bool ShaderManager::init() {
	//TODO: Vertex shader
	std::string vertexShaderContent;
	VERIFY( loadContent("shaders/VertexShader.vertexshader", vertexShaderContent) , 
		"Unable to load fragment vertex program", return false);

	GLuint vertextShaderId = glCreateShader(GL_VERTEX_SHADER);
	loadShader(vertextShaderId, vertexShaderContent.c_str());
	
	//Fragment shader
	std::string fragmentShaderContent;
	VERIFY( loadContent("shaders/FragmentShader.fragmentshader", fragmentShaderContent) , 
		"Unable to load fragment shader program", return false);

	
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	loadShader( fragmentShaderId, fragmentShaderContent.c_str());

	//Program
	programID = glCreateProgram();
	glAttachShader(programID, fragmentShaderId);
	glAttachShader(programID, vertextShaderId);
	glLinkProgram(programID);

	printLog(programID);

	glUseProgram(programID);

	glDeleteShader(fragmentShaderId);
	return true;
}

void loadShader( GLuint shaderId, char const * source) {
	glShaderSource(shaderId, 1, &source , NULL);
	glCompileShader(shaderId);

	printLog(shaderId);
}

GLuint ShaderManager::getProgramId(){
	return programID;
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
		printf("%s\n",infoLog);
}

