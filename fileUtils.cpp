#include "fileUtils.h"

bool loadContent(const char * path, std::string &result) {
  std::ifstream stream(path);
  if (!stream.is_open()){
    return false;
  }

  result = "";
  std::string line = "";
  while (std::getline(stream, line)){
    result += line + "\n";
  }

  return true;
}