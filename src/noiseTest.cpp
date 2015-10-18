#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cfloat>

#include "noise.h"

typedef struct                       /**** BMP file header structure ****/
{
    unsigned int bfSize;
    /* Size of file */
    unsigned short bfReserved1;
    /* Reserved */
    unsigned short bfReserved2;
    /* ... */
    unsigned int bfOffBits;        /* Offset to bitmap data */
} BITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int biSize;
    /* Size of info header */
    int biWidth;
    /* Width of image */
    int biHeight;
    /* Height of image */
    unsigned short biPlanes;
    /* Number of color planes */
    unsigned short biBitCount;
    /* Number of bits per pixel */
    unsigned int biCompression;
    /* Type of compression to use */
    unsigned int biSizeImage;
    /* Size of image data */
    int biXPelsPerMeter;
    /* X pixels per meter */
    int biYPelsPerMeter;
    /* Y pixels per meter */
    unsigned int biClrUsed;
    /* Number of colors used */
    unsigned int biClrImportant;   /* Number of important colors */
} BITMAPINFOHEADER;

#ifdef NOISE_TEST
int main(int argc, char *argv[]) {\
    return generateFile(argc, argv);
}
#endif

int generateFile(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Not enough params, required: PATH WIDTH HEIGHT\n");
        return -1;
    }

    const int width = atoi(argv[2]);
    const int height = atoi(argv[3]);
    const float size = static_cast<float>(atof(argv[4]));

    printf("Generating noise: %s:%ux%u, real width: %f\n", argv[1], width, height, size);

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

/* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType = 0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 640 * 480 * 3;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    FILE *file = fopen(argv[1], "wb");
    if (!file) {
        printf("Could not write file\n");
        return -1;
    }

/*Write headers*/
    fwrite(&bfType, 1, sizeof(bfType), file);
    fwrite(&bfh, 1, sizeof(bfh), file);
    fwrite(&bih, 1, sizeof(bih), file);

/*Write bitmap*/
    const float step = size / width;
    const float maxAmplitude = sqrt(PERSISTENCE);

    float maxHeight = FLT_MIN;
    float minHeight = FLT_MAX;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < width; y++) {
            const float posX = (x - width / 2) * step;
            const float posY = (y - height / 2) * step;

            const float height = noise(posX, posY) / maxAmplitude + AMPLITUDE;

            maxHeight = fmaxf(maxHeight, height);
            minHeight = fminf(minHeight, height);

            // 0.2989, 0.5870, 0.1140
            unsigned char r = 0 ;
            unsigned char g = 0;
            unsigned char b = 255 * height;
            fwrite(&b, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&r, 1, 1, file);
        }
    }

    printf("Min height: %f\n", minHeight);
    printf("Max height: %f\n", maxHeight);

    fclose(file);

    printf("Done!!!\n");
    return 0;
}
