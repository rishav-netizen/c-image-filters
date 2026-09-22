#include <stdio.h>
#include "filters.h"
// #include "image.h" //? not required cuz already included in filters.h

void grayscale(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
    int total = img->height * img->width;

    for (int i = 0; i < total; i++)
    {
        int gray = (img->pixel[i].blue * 0.114 + img->pixel[i].green * 0.587 + img->pixel[i].red * 0.299); //? better to the eyes
        // int gray = (img->pixel[i].blue + img->pixel[i].green + img->pixel[i].red) / 3;

        img->pixel[i].red = gray;
        img->pixel[i].green = gray;
        img->pixel[i].blue = gray;
    }
}

void invert(Image *img)
{
    if(img == NULL || img->pixel == NULL)
    {
        return;
    }
    int total = img->height * img->width;

    for(int i = 0; i < total; i++)
    {
        img->pixel[i].red = 255 - img->pixel[i].red;
        img->pixel[i].green = 255 - img->pixel[i].green;
        img->pixel[i].blue = 255 - img->pixel[i].blue;
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

    int total = img->height * img->width;

    for (int i = 0; i < total; i++)
    {
        // original pixel colors (uint8_t)
        float R = img->pixel[i].red;
        float G = img->pixel[i].green;
        float B = img->pixel[i].blue;

        // new pixel colors casted to int obviously
        int sepia_R = (int)(0.393*R + 0.769*G + 0.189*B);
        int sepia_G = (int)(0.349*R + 0.686*G + 0.168*B);
        int sepia_B = (int)(0.272*R + 0.534*G + 0.131*B);

        // to prevent overflow of uint8_t
        img->pixel[i].red = (sepia_R > 255) ? 255 : sepia_R;
        img->pixel[i].green = (sepia_G > 255) ? 255 : sepia_G;
        img->pixel[i].blue = (sepia_B > 255) ? 255 : sepia_B;

    }
}