#ifndef FILTER_H
#define FILTER_H

#include "image.h"

void grayscale(Image *img);
void invert(Image *img);
void sepia(Image *img);
void box_blur(Image *img);
void gaussian_blur(Image *img);
void sobel_edge(Image *img);
void flip_horizontal(Image *img);
void flip_vertical(Image *img);

#endif