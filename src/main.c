#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char* argv[]) 
{
    Image* file = image_read("examples/test.bmp");
    
    if (file==NULL)
    {
        printf("Could not read image file!\n");
        free(file);
        return 1;
    }

    printf("Yay!\n");
    printf("%d %d \n", file->height, file->width);
    return 0;
}