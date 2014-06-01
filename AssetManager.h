#include <map>
#include <memory>
#include <string>

#include "RenderCommon.h"
#include "Font.h"

class AssetManager final {
public:
  GLuint loadTexture(std::string fileName);
  GLuint getTextureId(std::string fileName);

  bool init();
  void shutdown();
  std::shared_ptr<Font> getDefaultFont();

private:
  std::map<std::string, TextureInfo> textures;
  std::shared_ptr<Font> defaultFont;

  std::shared_ptr<Font> loadFont(std::string metaFileName);
};
