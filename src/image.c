#include <stdio.h>
#include <stdlib.h>
#include "image.h"

Image* image_read(const char* filename)
{
    // opening file in read binary mode
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("File could not be opened!\n");
        return NULL;
    }

    BMPHeader header;
    fread(&header, sizeof(BMPHeader), 1, file);

    // (0x4D42 is hex for ASCII 'B' = 0x42 and 'M' = 0x4D in little-endian byte order)
    if(header.signature != 0x4D42)
    {
        printf("Error: The file is not BMP!\n");
        fclose(file);
        return NULL;
    }

    BMPInfoHeader info_header;
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);
    if(info_header.compression != 0 || info_header.bit_count != 24)
    {
        printf("Error: Only uncompressed BMP images are allowed!\n");
        fclose(file);
        return NULL;
    }


    Image *image = (Image*)malloc(sizeof(Image));
    image->header = header;
    image->info_header = info_header;
    image->width = info_header.width;
    image->height = abs(info_header.height); //absolute cuz it can be negative aswell

    // malloc return the base address of pixels from where the image starts, and they're continuos
    // this is basically pixel array, but the image is still in form of matrix
    image->pixel = (Pixel *)malloc(image->height * image->width * sizeof(Pixel));

    if (image->pixel == NULL)
    {
        printf("Not enough memory for pixel!\n");
        return NULL;
    }
    
    fseek(file, header.offset, SEEK_SET); // SEEK_SET = 0 (beginning)

    //? we need this cuz each row in BMP is multiple of 4bytes, cuz earlier it was easier for systems to read it
    int padding = (4 - (image->width * sizeof(Pixel)) % 4) % 4;

    for(int i = 0; i < image->height; i++)
    {
        // because the rows are read reverse, from bottom up 
        int row = image->height - i - 1;

        // fread(WHERE_TO_SAVE, SIZE_OF_ONE_ITEM, HOW_MANY_ITEMS, WHICH_FILE);
        fread(&image->pixel[row * image->width], sizeof(Pixel), image->width, file);

        // after each row is read, go ahead to consider the padding then move on to next row
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    return image;
}

void image_free(Image* img)
{
    if (img != NULL)
    {
        free(img->pixel);
        free(img);
    }
}

int image_write(const char* outfile, const Image* img)
{
    if (outfile == NULL || img == NULL || img->pixel == NULL)
    {
        return 0;
    }

    FILE *output = fopen(outfile, "wb");
    if (output == NULL)
    {
        return 0;
    }

    // copy meta data as it is of the source image file
    fwrite(&img->header, sizeof(BMPHeader), 1, output);
    fwrite(&img->info_header, sizeof(BMPInfoHeader), 1, output);

    int padding = (4 - (img->width * sizeof(Pixel)) % 4) % 4;

    for (int i = 0, h = img->height, w = img->width; i < h; i++)
    {
        int row = h - i - 1;

        fwrite(&img->pixel[row * w], sizeof(Pixel), w, output);

        uint8_t pad_bytes[3] = {0, 0, 0};

        fwrite(pad_bytes, sizeof(uint8_t), padding, output); // sizeof(uint8_t) = 1
    }

    fclose(output);
    return 1;
}