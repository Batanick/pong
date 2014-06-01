#include "Font.h"

void Font::init(std::vector<const Glyph> symbols) {
  for (Glyph gl : symbols) {
    this->symbols.insert(std::make_pair(gl.key, gl));
  }
}

const TextureInfo & Font::getTextureInfo() const {
  return info;
}

Glyph Font::getGlyph(int code) const {
  auto result = symbols.find(code);

  if (result == symbols.end())
    return Glyph();

  return result->second;
}

int Font::getDistance() const {
  return distance;
}