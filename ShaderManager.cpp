#include "ShaderManager.h"

#include "fileUtils.h"
#include "logging.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void initTextShader();
void initMeshShader();
void initSkyMesh();
void initDumb();

static const std::string SHADER_PATH = "../shaders/";

void loadShaderSource(GLuint shaderId, char const * source);
void printLog(GLuint obj);

static std::vector<ShaderManager::ShaderDesc> SHADERS = {
  ShaderManager::ShaderDesc(ShaderType::TERRAIN_SHADER, "Terrain.vs", "Terrain.fs", &initMeshShader),
  ShaderManager::ShaderDesc(ShaderType::BUSH_SHADER, "Bush.vs", "Bush.fs", &initMeshShader),
  ShaderManager::ShaderDesc(ShaderType::TEX_MESH_SHADER, "TexturedMesh.vs", "TexturedMesh.fs", &initMeshShader),
  ShaderManager::ShaderDesc(ShaderType::GUI_SHADER, "GuiElement.vs", "GuiElement.fs", &initTextShader),
  ShaderManager::ShaderDesc(ShaderType::CLOUDS_SHADER, "Clouds.vs", "Clouds.fs", &initSkyMesh),
  ShaderManager::ShaderDesc(ShaderType::WATER_SHADER, "Water.vs", "Water.fs", &initMeshShader),
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
  LOG("Loading shader: %s", shaderDesc.vertexShaderFName.c_str());
  if (!loadContent((SHADER_PATH + shaderDesc.vertexShaderFName).c_str(), vertexShaderContent)) {
    LOG("Unable to load: [%s]", shaderDesc.vertexShaderFName.c_str());
    return false;
  }

  const GLuint vertextShaderId = glCreateShader(GL_VERTEX_SHADER);
  loadShaderSource(vertextShaderId, vertexShaderContent.c_str());

  //Fragment shader
  std::string fragmentShaderContent;
  LOG("Loading shader: %s", shaderDesc.fragmentShaderFName.c_str());
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
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initMeshShader() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initSkyMesh() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initDumb() {

}
