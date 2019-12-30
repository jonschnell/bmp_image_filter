#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void filter_blur();
void filter_sharpen();
void filter_edge();
void filter_instagram();
void filter_grayscale();
void filter_red();
int load_image(FILE *fp);
int write_image(FILE *out, FILE *in);


typedef struct pixel {
    unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
} pixel_type;

typedef struct arithmetic_filter{
    signed int r_add;
	signed int g_add;
	signed int b_add;
	double r_mult;
    double b_mult;
    double g_mult;
} arithmetic_filter_type;
