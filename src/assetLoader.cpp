#include "assetLoader.h"

#include "assimp/Importer.hpp"

#include <GL/glew.h>


#include "logging.h"

static const int DDS_HEADER_SIZE_BYTES = 124;
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

bool loadDDS(const std::string fileName, unsigned int &w, unsigned int &h) {
    FILE *fp = fopen(fileName.c_str(), "rb");
    VERIFY(fp, "Unable to open texture file", return false;);

    char filecode[4];
    fread(filecode, 1, 4, fp);
    VERIFY(strncmp(filecode, "DDS ", 4) == 0, "Incorrect texture file", return false;)

    unsigned char header[DDS_HEADER_SIZE_BYTES];
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int *) &(header[8]);
    unsigned int width = *(unsigned int *) &(header[12]);
    unsigned int linearSize = *(unsigned int *) &(header[16]);
    unsigned int mipMapCount = *(unsigned int *) &(header[24]);
    unsigned int fourCC = *(unsigned int *) &(header[80]);

    w = width;
    h = height;

    unsigned char *buffer;
    unsigned int bufsize;
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = new unsigned char[bufsize];
    fread(buffer, 1, bufsize, fp);
    fclose(fp);

    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            delete[] buffer;
            return false;
    }

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }

    free(buffer);

    return true;
}