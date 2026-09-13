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