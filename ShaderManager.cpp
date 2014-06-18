#include "ShaderManager.h"

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

void initTerrainShader() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void initTestShader() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void initBushShader() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}


// shaders list here
std::vector<ShaderManager::ShaderDesc> ShaderManager::buildShaders() {
  std::vector<ShaderDesc> shaders;

  // ============== TERRAIN_SHADER ============== 
  shaders.push_back(ShaderDesc(
    ShaderType::TERRAIN_SHADER,
    "Terrain.vs",
    "Terrain.fs",
    &initTerrainShader));

  // ============== BUSH_SHADER ============== 
  shaders.push_back(ShaderDesc(
    ShaderType::BUSH_SHADER,
    "Bush.vs",
    "Bush.fs",
    &initBushShader));

  // ============== FONT_SHADER ============== 
  shaders.push_back(ShaderDesc(
    ShaderType::FONT_SHADER,
    "Font.vs",
    "Font.fs",
    &initTextShader));

  // ============== TEST_SHADER ============== 
  shaders.push_back(ShaderDesc(
    ShaderType::TEST_SHADER,
    "Test.vs",
    "Test.fs",
    initTestShader));

  return shaders;
}

static const std::string SHADER_PATH = "../shaders/";

void loadShaderSource(GLuint shaderId, char const * source);
void printLog(GLuint obj);

bool ShaderManager::init() {
  std::vector<ShaderManager::ShaderDesc> shaderDescs = buildShaders();
  for (ShaderDesc desc : shaderDescs) {
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
