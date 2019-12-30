#include "img.h"
#define IMG_ROWS 900
#define IMG_COLS 900

pixel_type pixelin[IMG_ROWS][IMG_COLS];
pixel_type pixelout[IMG_ROWS][IMG_COLS];
arithmetic_filter_type userin;


uint16_t bpp;
uint32_t pixel_arr_loc;
	/*unsigned int r_in[IMG_ROWS][IMG_COLS];
	unsigned int g_in[IMG_ROWS][IMG_COLS];
	unsigned int b_in[IMG_ROWS][IMG_COLS];
	unsigned int a_in[IMG_ROWS][IMG_COLS];
	unsigned int r_out[IMG_ROWS][IMG_COLS];
	unsigned int g_out[IMG_ROWS][IMG_COLS];
	unsigned int b_out[IMG_ROWS][IMG_COLS];
	unsigned int a_out[IMG_ROWS][IMG_COLS];*/

void filter_grayscale();
void filter_red();
void filter_instagram();
void filter_blur();
int load_image(FILE *fp);
int write_image(FILE *out, FILE *in);


int load_image(FILE *fp) {
	unsigned char signature[2];
	uint32_t file_size;
	uint32_t dib_size;
	uint16_t comp_method;

	if(fp == NULL) {
		printf("Unable to open file.\n");
		return 1;
	}

	fread(signature, 1, 2, fp);
	printf("sig %c %c\n", signature[0], signature[1]);
	if(signature[0] != 'B' || signature[1] != 'M') {
		printf("Not a valid BMP file.\n");
		return 1;
	}

	fseek(fp, 2, SEEK_SET);
	fread(&file_size, 4, 1, fp);
	printf("file size %u\n", file_size);

	fseek(fp, 10, SEEK_SET);
	fread(&pixel_arr_loc, 4, 1, fp);
	printf("pixel array location %u\n", pixel_arr_loc);

	fseek(fp, 14, SEEK_SET);
	fread(&dib_size, 4, 1, fp);
	printf("dib size %u\n", dib_size);
	if(dib_size != 40) {
		printf("BMP version not supported.\n");
		return 1;
	}

	fseek(fp, 28, SEEK_SET);
	fread(&bpp, 2, 1, fp);
	printf("bits per pixel %u\n", bpp);
	if(bpp != 32) {
		printf("Bits per pixel not supported.\n");
		return 1;
	}

	fseek(fp, 30, SEEK_SET);
	fread(&comp_method, 2, 1, fp);
	printf("compression method %u\n", comp_method);
	if(comp_method != 0) {
		printf("Compression method not supported.\n");
		return 1;
	}

	int bytes_per_row = IMG_COLS * bpp / 8;
	int padding = 0;
	if(bytes_per_row % 4 != 0) {
		padding = 4 - bytes_per_row % 4;
	}
	printf("input padding %u\n", padding);
	fseek(fp, pixel_arr_loc, SEEK_SET);
	int row;
	int col;
	for(row = IMG_ROWS - 1; row >= 0; row--) {
		for(col = 0; col < IMG_COLS; col++) {
			fread(&pixelin[row][col].b, 1, 1, fp);
			fread(&pixelin[row][col].g, 1, 1, fp);
			fread(&pixelin[row][col].r, 1, 1, fp);
			fread(&pixelin[row][col].a, 1, 1, fp);
			pixelout[row][col].b = pixelin[row][col].b;
			pixelout[row][col].g = pixelin[row][col].g;
			pixelout[row][col].r = pixelin[row][col].r;
			pixelout[row][col].a = pixelin[row][col].a;
			fseek(fp, padding, SEEK_CUR);
		}
	}
	return 0;
}

int write_image(FILE *out, FILE *in) {
	int c;

	rewind(out);
	rewind(in);

	c = fgetc(in);
	while(c != EOF) {
		fputc(c, out);
		c = fgetc(in);
	}

	int bytes_per_row = IMG_COLS * bpp / 8;
	int padding = 0;
	if(bytes_per_row % 4 != 0) {
		padding = 4 - bytes_per_row % 4;
	}
	int row;
	int col;
	printf("output padding %u\n", padding);
	fseek(out, pixel_arr_loc, SEEK_SET);
	for(row = IMG_ROWS - 1; row >= 0; row--) {
		for( col = 0; col < IMG_COLS; col++) {
			fwrite(&pixelout[row][col].b, 1, 1, out);
			fwrite(&pixelout[row][col].g, 1, 1, out);
			fwrite(&pixelout[row][col].r, 1, 1, out);
			fwrite(&pixelout[row][col].a, 1, 1, out);
			fseek(out, padding, SEEK_CUR);
		}
	}
	return 0;
}
void filter_blur(){
    printf("enter the blur radius\n");
    int r, i, j, n, m, k, count, red, blue, green;
    scanf("%d", &r);
    for(i = r; i < IMG_ROWS-r; i++){
		for(j = r; j < IMG_COLS-r; j++){
            red = blue = green = 0;
            count = 0;
            for(n = -r; n < r; n++){
                
                for(m = -r; m < r; m++){
                    
                    int currentj = j + m;
                    int currenti = i + n;

                    if(currentj >= 0 && currentj < IMG_COLS && currenti >= 0 & currenti < IMG_ROWS){
                    
                    count++;
                    red = red + pixelin[i+n][j+m].r;
                    blue = blue + pixelin[i+n][j+m].b;
                    green = green + pixelin[i+n][j+m].g;
                    pixelout[i][j].r = red / count;
                    pixelout[i][j].b = blue / count;
                    pixelout[i][j].g = green / count;
                    }
                }
            }
        
        }
    }


    
}
void filter_sharpen(){
   int r, i, j, n, m, k, count, red, blue, green;
    int sharpen[3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
    for(i = 1; i < IMG_ROWS-1; i++){
		for(j = 1; j < IMG_COLS-1; j++){
            red = blue = green = 0;
            count = 0;
            for(n = 3; n < 1; n--){
                
                for(m = 3; m < 1; m--){
                    
                    int currentj = j + m;
                    int currenti = i + n;

                    if(currentj >= 0 && currentj < IMG_COLS && currenti >= 0 & currenti < IMG_ROWS){
                    
                    count++;
                    red = red + pixelin[i - 1 + n][j - 1 + m].r * sharpen[n][m];
                    blue = blue + pixelin[i -1 + n][j -1 + m].b * sharpen[n][m];
                    green = green + pixelin[i -1 + n][j - 1 + m].g * sharpen[n][m];
                    pixelout[i][j].r = red;
                    pixelout[i][j].b = blue;
                    pixelout[i][j].g = green;
                    }
                }
            }
        
        }
    }
    

}
void filter_edge(){
   int r, i, j, n, m, k, count, red, blue, green;
    int sobel[3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
    for(i = 1; i < IMG_ROWS-1; i++){
		for(j = 1; j < IMG_COLS-1; j++){
            red = blue = green = 0;
            count = 0;
            for(n = 0; n < 3; n++){
                
                for(m = 0; m < 3; m++){
                    
                    int currentj = j + m;
                    int currenti = i + n;

                    if(currentj >= 0 && currentj < IMG_COLS && currenti >= 0 & currenti < IMG_ROWS){
                    
                    count++;
                    red = red + pixelin[i+n][j+m].r * sobel[n][m];
                    blue = blue + pixelin[i+n][j+m].b * sobel[n][m];
                    green = green + pixelin[i+n][j+m].g * sobel[n][m];
                    pixelout[i][j].r = red / count;
                    pixelout[i][j].b = blue / count;
                    pixelout[i][j].g = green / count;
                    }
                }
            }
        
        }
    }

}
void filter_instagram(){
    
    printf("enter a signed intiger to add or subtract from red, green, and blue\n");
    scanf("%d", &userin.r_add);
    scanf("%d", &userin.g_add);
    scanf("%d", &userin.b_add);
    printf("enter an unsigned muntiplier for red, green, and blue\n");
    scanf("%lf", &userin.r_mult);
    scanf("%lf", &userin.g_mult);
    scanf("%lf", &userin.b_mult);
	int i, j;
	for(i = 0; i < IMG_ROWS; i++){
		for(j = 0; j < IMG_COLS; j++){
			pixelout[i][j].r = userin.r_add + (pixelin[i][j].r * userin.r_mult);
            pixelout[i][j].g = userin.g_add + (pixelin[i][j].g * userin.g_mult);
            pixelout[i][j].b = userin.b_add + (pixelin[i][j].b * userin.b_mult);
		}
	}
}
void filter_grayscale(){
	int i, j;
	for(i = 0; i <= IMG_ROWS; i++){
		for(j = 0; j <= IMG_COLS; j++){
			if(pixelin[i][j].r > pixelin[i][j].g && pixelin[i][j].r > pixelin[i][j].b){
				pixelout[i][j].b = pixelin[i][j].r;
				pixelout[i][j].g = pixelin[i][j].r;
				pixelout[i][j].r = pixelin[i][j].r;
			}
			if(pixelin[i][j].b > pixelin[i][j].g && pixelin[i][j].b > pixelin[i][j].r){

				pixelout[i][j].r = pixelin[i][j].b;
				pixelout[i][j].g = pixelin[i][j].b;
				pixelout[i][j].b = pixelin[i][j].b;
			}
			if(pixelin[i][j].g > pixelin[i][j].b && pixelin[i][j].g > pixelin[i][j].r){
				pixelout[i][j].r = pixelin[i][j].g;
				pixelout[i][j].b = pixelin[i][j].g;
				pixelout[i][j].g = pixelin[i][j].g;
			}
		}
	}
}
void filter_red(){
	int i, a;
	for(i = 0; i <= IMG_ROWS; i++){
		for(a = 0; a <= IMG_COLS; a++){
			
			/*if(b_in[i][a] > g_in[i][a] && b_in[i][a] > r_in[i][a]){
	
			}
			if(g_in[i][a] > b_in[i][a] && g_in[i][a] > r_in[i][a]){
				
			}*/
		}
	}
}

