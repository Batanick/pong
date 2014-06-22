#include "ShaderManager.h"

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void initTextShader();
void initMeshShader();
void initSkyMesh();

static const std::string SHADER_PATH = "../shaders/";

void loadShaderSource(GLuint shaderId, char const * source);
void printLog(GLuint obj);

static std::vector<ShaderManager::ShaderDesc> SHADERS = {
  ShaderManager::ShaderDesc(ShaderType::TERRAIN_SHADER, "Terrain.vs", "Terrain.fs", &initMeshShader),
  ShaderManager::ShaderDesc(ShaderType::BUSH_SHADER, "Bush.vs", "Bush.fs", &initMeshShader),
  ShaderManager::ShaderDesc(ShaderType::FONT_SHADER, "Font.vs", "Font.fs", &initTextShader),
  ShaderManager::ShaderDesc(ShaderType::SKYBOX_SHADER, "SkyBox.vs", "SkyBox.fs", &initSkyMesh),
  ShaderManager::ShaderDesc(ShaderType::TEST_SHADER, "Test.vs", "Test.fs", &initMeshShader)
};

bool ShaderManager::init() {
  for (const ShaderDesc &desc : SHADERS) {
    if (!loadShader(desc)){
      return false;
    }
  }

  return true;
}

bool ShaderManager::loadShader(const ShaderDesc &shaderDesc){
  if (shaders.find(shaderDesc.shaderType) != shaders.end()){
    LOG("Attempt to load more that one shader programm of the same type");
    return false;
  }

  //Vertex shader
  std::string vertexShaderContent;
  if (!loadContent((SHADER_PATH + shaderDesc.vertexShaderFName).c_str(), vertexShaderContent)) {
    LOG("Unable to load: [%s]", shaderDesc.vertexShaderFName.c_str());
    return false;
  }

  const GLuint vertextShaderId = glCreateShader(GL_VERTEX_SHADER);
  loadShaderSource(vertextShaderId, vertexShaderContent.c_str());

  //Fragment shader
  std::string fragmentShaderContent;
  if (!loadContent((SHADER_PATH + shaderDesc.fragmentShaderFName).c_str(), fragmentShaderContent)) {
    LOG("Unable to load: [%s]", shaderDesc.fragmentShaderFName.c_str());
    return false;
  }

  const GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  loadShaderSource(fragmentShaderId, fragmentShaderContent.c_str());

  //Program
  const GLuint programID = glCreateProgram();
  glAttachShader(programID, fragmentShaderId);
  glAttachShader(programID, vertextShaderId);
  glLinkProgram(programID);

  printLog(programID);

  ShaderManager::ShaderParams shaderParams(shaderDesc.shaderType, shaderDesc.initilizer, programID);
  shaders.insert(std::make_pair(shaderDesc.shaderType, shaderParams));

  //don't actually destroyed until program
  glDeleteShader(fragmentShaderId);
  glDeleteShader(vertextShaderId);

  return true;
}

void ShaderManager::shutdown() {
  for (auto& entry : shaders) {
    glDeleteProgram(entry.second.programId);
  }
}

void loadShaderSource(GLuint shaderId, char const * source) {
  glShaderSource(shaderId, 1, &source, NULL);
  glCompileShader(shaderId);

  printLog(shaderId);
}

GLuint ShaderManager::getProgramId(const ShaderType shaderType) {
  auto result = shaders.find(shaderType);
  if (result == shaders.end()) {
    LOG("Can't find info for shader: [%d]", shaderType);
    return GL_INVALID_VALUE;
  }

  return result->second.programId;
}

void ShaderManager::useProgram(const ShaderType shaderType) {
  auto result = shaders.find(shaderType);
  if (result != shaders.end()) {
    if (result->second.initilizer) {
      result->second.initilizer();
    }

    glUseProgram(result->second.programId);
  }
  else {
    LOG("Unable to find program: [%d]", shaderType);
    return;
  }
}

void printLog(GLuint obj) {
  int infologLength = 0;
  int maxLength = 1024;

  if (glIsShader(obj))
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);
  else
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

  char infoLog[128 * 1024];

  if (glIsShader(obj))
    glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
  else
    glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

  if (infologLength > 0)
    LOG("%s\n", infoLog);
}

void initTextShader() {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glEnable(GL_BLEND);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initMeshShader() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glDisable(GL_BLEND);
}

void initSkyMesh() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glDisable(GL_BLEND);
}
