#ifndef H_FONT
#define H_FONT

#include <map>

#include "RenderCommon.h"
#include "fontLoader.h"

class Font final {

public:
    Font(TextureInfo info) :
            info(info),
            distance(1) {
    }

    void init(std::vector<Glyph> symbols);

    const TextureInfo &getTextureInfo() const;

    Glyph getGlyph(int code) const;

    int getDistance() const;

private:
    const TextureInfo info;
    std::map<int, const Glyph> symbols;
    const int distance;
};

#endif