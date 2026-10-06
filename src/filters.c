#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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

void box_blur(Image *img, int r)
{
    if (img == NULL || img->pixel == NULL || r < 1)
    {
        return;
    }
    Pixel *blurred = (Pixel *)malloc(sizeof(Pixel) * img->height * img->width);
    if (blurred == NULL)
    {
        return;
    }
    
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            int red_sum = 0, green_sum = 0, blue_sum = 0, count = 0;
            // int r = 10; // blur radius
            for(int dx = -r; dx <= r; dx++)
            {
                for (int dy = -r; dy <= r; dy++)
                {
                    int neigh_x = x + dx, neigh_y = y + dy;
                    if (neigh_x >= 0 && neigh_x < img->width && neigh_y >= 0 && neigh_y < img->height)
                    {
                        Pixel p = PIXEL_AT(img, neigh_x, neigh_y);
                        red_sum += p.red;
                        green_sum += p.green;
                        blue_sum += p.blue;
                        count++;
                    }
                }
            }
            int index = y * img->width + x;
            blurred[index].red = (uint8_t)(red_sum / count);
            blurred[index].green = (uint8_t)(green_sum / count);
            blurred[index].blue = (uint8_t)(blue_sum / count);
        }
    }
    free(img->pixel);
    img->pixel = blurred; // replace old pixel buffer with blurred image
}


void gaussian_blur(Image *img, int r)
{
    if (img == NULL || img->pixel == NULL || r < 1)
    {
        return;
    }

    Pixel *blurred = (Pixel *)malloc(sizeof(Pixel) * img->height * img->width);
    if (blurred == NULL)
    {
        return;
    }

    float sigma = (r > 1) ? r/2.0f : 1.0f; //? took here for simplicity (its r/2 for r > 1 or we just take it 1)
    int k_size = (2 * r + 1); // the convolution matrix size would be this cuz like for 1 neighbour we would have 3x3 matrix
    float *weights = malloc(sizeof(float) * k_size * k_size); // allocate memory for the weights array
    float two_sigma_sqr = 2.0f * sigma * sigma;
    for(int dy = -r; dy <= r; dy++)
    {
        for(int dx = -r; dx <= r; dx++)
        {
            int weight_index = (dy + r) * (k_size) + (dx + r); // convert the 2d coordinates into 1d array index
            weights[weight_index] = expf(-(dx*dx + dy*dy)/two_sigma_sqr);
        }
    }
    
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            float red_sum = 0.0f, blue_sum = 0.0f, green_sum = 0.0f, weight_sum = 0.0f;
            for(int dy = -r; dy <= r; dy++)
            {
                for(int dx = -r; dx <= r; dx++)
                {
                    int neigh_x = x + dx;
                    int neigh_y = y + dy;
                    if (neigh_x >= 0 && neigh_x < img->width && neigh_y >= 0 && neigh_y < img->height)
                    {
                        float weight = weights[(dy + r) * k_size + (dx + r)];
                        Pixel p = PIXEL_AT(img, neigh_x, neigh_y);

                        red_sum += p.red * weight; 
                        green_sum += p.green * weight; 
                        blue_sum += p.blue * weight; 
                        weight_sum += weight;
                    }
                    
                }
            }
            int index = y * img->width + x;
            //? we added 0.5 so that the casting is done upward and nicely 
            blurred[index].red = (uint8_t)(red_sum/weight_sum + 0.5f);
            blurred[index].green = (uint8_t)(green_sum/weight_sum + 0.5f);
            blurred[index].blue = (uint8_t)(blue_sum/weight_sum + 0.5f);
        }
        
    }
    free(weights);
    free(img->pixel); //free old img 
    img->pixel = blurred; // replace old pixel buffer with blurred image
}

void sobel_edge(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
}

void flip_horizontal(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
}

void flip_vertical(Image *img)
{
    if (img == NULL || img->pixel == NULL)
    {
        return;
    }
}