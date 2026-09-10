#ifndef IMAGE_H // compiler checks "if not defined" IMAGE_H
#define IMAGE_H // then defines the header, also the name IMAGE_H is convention, i can use anything

#include <stdint.h>

// any BMP is just stream of binary bytes, in three sections
// 14bytes file header
// 40bytes info header
// pixel data (rest of the file)


// BMP file header meta data of 14 bytes has 5fields
// we use packed attribute to save from padding
typedef struct BMPHeader
{   
    uint16_t signature;     // 2bytes = 16bits to store file signature ('B' and 'M')
    uint32_t size;          // 4bytes = 32bits to store the .bmp file's size
    uint16_t reserved1;     // 2bytes to store the reserved data (mostly zero)
    uint16_t reserved2;     // 2bytes to store the reserved data (mostly zero)
    uint32_t offset;        // 4bytes to store position of where image pixel starts

} __attribute__((packed)) BMPHeader; 


// info header metadata of 40 bytes has 11 fields
typedef struct BMPInfoHeader
{
    uint32_t header_size;       // 4 bytes: size of this header (always 40)
    int32_t  width;             // 4 bytes: image width in pixels
    int32_t  height;            // 4 bytes: image height (positive = bottom-up, negative = top-down)

    uint16_t planes;            // 2 bytes: number of color planes (must be 1)
    uint16_t bit_count;         // 2 bytes: bits per pixel (24 for 24-bit RGB)

    uint32_t compression;       // 4 bytes: compression type (0 = uncompressed BI_RGB)
    uint32_t image_size;        // 4 bytes: image data size in bytes (including row padding)

    int32_t  x_pixels_per_m;    // 4 bytes: horizontal resolution (pixels per meter)
    int32_t  y_pixels_per_m;    // 4 bytes: vertical resolution (pixels per meter)
    uint32_t colors_used;       // 4 bytes: number of colors in palette (0 for 24-bit)
    uint32_t colors_important;  // 4 bytes: number of important colors (0 = all)

} __attribute__((packed)) BMPInfoHeader;

// to hold pixel data, each pixel is 3bytes, holding color value of int BGR order not RGB
typedef struct Pixel
{
    //each color takes one byte to store
    uint8_t blue; 
    uint8_t green;
    uint8_t red;

} __attribute__((packed)) Pixel;

// this image struct bundles the whole BMP file together to use anytime in RAM
typedef struct Image
{
    int width;
    int height;
    BMPHeader header;
    BMPInfoHeader info_header;
    Pixel* pixel;
} Image;

// function prototypes for the definitions to be written in image.c
Image* image_read(const char *filename); //read BMP file and return the Image struct pointer
void image_free(Image* img); // free that image after use

#endif