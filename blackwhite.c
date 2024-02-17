/*
* C program to convert image to pure black and white. 
* Note that this is only for bmp files. I might try a png version in the future
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

struct BitmapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BitmapInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct RgbPixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
#pragma pack(pop)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_bmp_file>\n", argv[0]);
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        perror("Error opening input file");
        return 1;
    }

    struct BitmapFileHeader fileHeader;
    struct BitmapInfoHeader infoHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, infile);
    fread(&infoHeader, sizeof(infoHeader), 1, infile);

    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Input file is not in BMP format\n");
        fclose(infile);
        return 1;
    }

    if (infoHeader.biBitCount != 24) {
        fprintf(stderr, "Input file is not a 24-bit BMP\n");
        fclose(infile);
        return 1;
    }

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    int padding = (4 - (width * sizeof(struct RgbPixel)) % 4) % 4;

    FILE *outfile = fopen("out.bmp", "wb");
    if (!outfile){
        perror("Error creating output file");
        fclose(infile);
        return 1;
    }
    fwrite(&fileHeader, sizeof(fileHeader), 1, outfile);
    fwrite(&infoHeader, sizeof(infoHeader), 1, outfile);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            struct RgbPixel pixel;
            fread(&pixel, sizeof(pixel), 1, infile);
            uint8_t grayscale = (uint8_t)((pixel.red + pixel.green + pixel.blue) / 3);
            pixel.red = grayscale;
            pixel.green = grayscale;
            pixel.blue = grayscale;
            fwrite(&pixel, sizeof(pixel), 1, outfile);
        }
        fseek(infile, padding, SEEK_CUR);
        for (int p = 0; p < padding; p++) {
            fputc(0, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);

    printf("Completion of task. Output saved as out.bmp\n");

    return 0;
}