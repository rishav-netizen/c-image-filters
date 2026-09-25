#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "filters.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>

enum Filter
{
    GRAYSCALE,
    INVERT,
    SEPIA,
    BOXBLUR,
    GAUSSIANBLUR,
    SOBELEDGE,
    FLIPHORIZONTAL,
    FLIPVERTICAL,
    INVALID
};

enum Filter filter(const char* name);

int main(int argc, char* argv[]) 
{
    if ((argc != 5) || (strcmp(argv[1], "-f") != 0))
    {
        printf("Usage: ./c-image-filters -f <filter_name> <input.bmp> <output.bmp>\n");
        return 1;
    }
    
    char* filter_name = argv[2];
    char* infile = argv[3];
    char* outfile = argv[4];

    Image* img = image_read(infile);
    
    if (img==NULL)
    {
        printf("Could not read image file!\n");
        free(img);
        return 2;
    }

    switch (filter(filter_name))
    {
        case GRAYSCALE:
            grayscale(img);
            break;

        case INVERT:
            invert(img);
            break;

        case SEPIA:
            sepia(img);
            break;

        case BOXBLUR:
            box_blur(img);
            break;

        case GAUSSIANBLUR:
            gaussian_blur(img);
            break;

        case SOBELEDGE:
            sobel_edge(img);
            break;

        case FLIPHORIZONTAL:
            flip_horizontal(img);
            break;

        case FLIPVERTICAL:
            flip_vertical(img);
            break;

        case INVALID:
        default:
            printf("Invalid filter: %s\n", filter_name);
            image_free(img);
            return 1;
    }

    

    if (image_write(outfile, img)) 
    {
        printf("%s filter applied to %s and saved at %s\n", filter_name, infile, outfile);
    }
    else
    {
        printf("COULDNT WRITE\n");
    }

    image_free(img);
    return 0;
}

enum Filter filter(const char* name)
{
    if (name == NULL)
    {
        return INVALID;
    }

    if (strcasecmp("grayscale", name) == 0 || strcasecmp("gray", name) == 0)
    {
        return GRAYSCALE;
    }
    else if (strcasecmp("invert", name) == 0)
    {
        return INVERT;
    }
    else if (strcasecmp("sepia", name) == 0)
    {
        return SEPIA;
    }
    else if (strcasecmp("boxblur", name) == 0 || strcasecmp("box_blur", name) == 0 || strcasecmp("box-blur", name) == 0)
    {
        return BOXBLUR;
    }
    else if (strcasecmp("gaussianblur", name) == 0 || strcasecmp("gaussian_blur", name) == 0 || strcasecmp("gaussian-blur", name) == 0 || strcasecmp("gaussian", name) == 0)
    {
        return GAUSSIANBLUR;
    }
    else if (strcasecmp("sobel", name) == 0 || strcasecmp("sobeledge", name) == 0 || strcasecmp("sobel_edge", name) == 0 || strcasecmp("sobel-edge", name) == 0)
    {
        return SOBELEDGE;
    }
    else if (strcasecmp("fliphorizontal", name) == 0 || strcasecmp("flip_horizontal", name) == 0 || strcasecmp("flip-horizontal", name) == 0)
    {
        return FLIPHORIZONTAL;
    }
    else if (strcasecmp("flipvertical", name) == 0 || strcasecmp("flip_vertical", name) == 0 || strcasecmp("flip-vertical", name) == 0)
    {
        return FLIPVERTICAL;
    }

    return INVALID;
}