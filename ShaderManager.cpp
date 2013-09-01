#include "ShaderManager.h"

#include <vector>

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>

void loadShader( GLuint shaderId, char const * source);
void printLog(GLuint obj);

bool ShaderManager::init() {
	//TODO: Vertex shader
	
	//Fragment shader
	std::string fragmentShaderContent;
	VERIFY( loadContent("shaders/FragmentShader.fragmentshader", fragmentShaderContent) , 
		"Unable to load fragment shader program", return false);

	
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = fragmentShaderContent.c_str();
	loadShader( fragmentShaderId, fragmentShaderContent.c_str());

	//Program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, fragmentShaderId);
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

