#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "filters.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

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
    char* filter_name = NULL;
    int radius = 4;

    int option;
    // we use : in the string for getopt to show that after that flag we expect a value, optarg stores it
    while ((option = getopt(argc, argv, "f:r:")) != -1)
    {
        switch (option)
        {
        case 'f':
            filter_name = optarg;
            break;
        
        case 'r':
        {
            char* end; // this points to the last unconverted character
            long val = strtol(optarg, &end, 10);//strtol(null terminated string, endptr, base) //? base 10 means decimal
            if (*end != '\0' || val <= 0) //? end == '\0' means that we reached the end of the string hence it was successfully converted
            {
                printf("Invalid input: %s for radius!\n", optarg);
                return 1;
            }
            radius = (int)val; //strtol(string, endptr, base) //? base 10 means decimal
            break;
        }

        default:
            printf("Usage: %s -f <filter> [-r <radius>] <input> <output>\n", argv[0]);
            return 1;
        }
    }
    if (filter_name == NULL || argc - optind != 2)
    {
        printf("Usage: %s -f <filter> [-r <radius>] <input> <output>\n", argv[0]);
        return 1;
    }
    
    char* infile = NULL;
    char* outfile = NULL;
    //? optind is the index when the positional arguments start
    infile = argv[optind];
    outfile = argv[optind + 1];

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
            box_blur(img, radius);
            break;

        case GAUSSIANBLUR:
            gaussian_blur(img, radius);
            break;

        case SOBELEDGE:
            sobel_edge2(img);
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
        printf("Could not apply the filter! Try again!\n");
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