#include <stdio.h>
#include "filters.h"
// #include "image.h" //? not required cuz already included in filters.h

void grayscale(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }

    //! we go row by row, similar to how the image is stored in the memory
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            int gray = (PIXEL_AT(img, x, y).blue * 0.114 + PIXEL_AT(img, x, y).green * 0.587 + PIXEL_AT(img, x, y).red * 0.299); //? better to the eyes
            // int gray = (PIXEL_AT(img, x, y).blue + PIXEL_AT(img, x, y).green + PIXEL_AT(img, x, y).red) / 3;
    
            PIXEL_AT(img, x, y).red = gray;
            PIXEL_AT(img, x, y).green = gray;
            PIXEL_AT(img, x, y).blue = gray;
        }
    }
}

void invert(Image *img)
{
    if(img == NULL || img->pixel == NULL)
    {
        return;
    }

    for (int y = 0; y < img->height; y++)
    {
        for(int x = 0; x < img->width; x++)
        {
            PIXEL_AT(img, x, y).red = 255 - PIXEL_AT(img, x, y).red;
            PIXEL_AT(img, x, y).green = 255 - PIXEL_AT(img, x, y).green;
            PIXEL_AT(img, x, y).blue = 255 - PIXEL_AT(img, x, y).blue;
        }  
    }
}

void sepia(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // R' = 0.393R + 0.769G + 0.189B
    // G' = 0.349R + 0.686G + 0.168B
    // B' = 0.272R + 0.534G + 0.131B
    
    for (int y = 0; y < img->height; y++)
    {
        for(int x = 0; x < img->width; x++)
        {
            // original pixel colors (uint8_t)
            float R = PIXEL_AT(img, x, y).red;
            float G = PIXEL_AT(img, x, y).green;
            float B = PIXEL_AT(img, x, y).blue;

            // new pixel colors casted to int obviously
            int sepia_R = (int)(0.393*R + 0.769*G + 0.189*B);
            int sepia_G = (int)(0.349*R + 0.686*G + 0.168*B);
            int sepia_B = (int)(0.272*R + 0.534*G + 0.131*B);

            // to prevent overflow of uint8_t
            PIXEL_AT(img, x, y).red = (sepia_R > 255) ? 255 : sepia_R;
            PIXEL_AT(img, x, y).green = (sepia_G > 255) ? 255 : sepia_G;
            PIXEL_AT(img, x, y).blue = (sepia_B > 255) ? 255 : sepia_B;
        }  
    }
}

void box_blur(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // TODO: implement box blur filter
}

void gaussian_blur(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // TODO: implement gaussian blur filter
}

void sobel_edge(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // TODO: implement sobel edge filter
}

void flip_horizontal(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // TODO: implement flip horizontal filter
}

void flip_vertical(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    // TODO: implement flip vertical filter
}