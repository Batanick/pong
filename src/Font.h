#ifndef H_FONT
#define H_FONT

#include <map>

#include "RenderCommon.h"
#include "fontLoader.h"

class Font final {

public:
    Font(TextureInfo info, unsigned int rowHeight) :
            info(info),
            distance(1),
            rowHeight(rowHeight + 3) {
    }

    void init(std::vector<Glyph> symbols);

    const TextureInfo &getTextureInfo() const;

    Glyph getGlyph(int code) const;

    int getDistance() const;

    const int getRowHeight() const {
        return rowHeight;
    }

private:
    const TextureInfo info;
    std::map<int, const Glyph> symbols;
    const int distance;
    const int rowHeight;
};

#endif