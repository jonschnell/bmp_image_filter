#include "img.h"


char input_file_name[] = "cy.bmp";
char output_file_name[] = "cy2.bmp";

int main() {

	FILE *in = fopen(input_file_name, "rb");
	FILE *out = fopen(output_file_name, "wb");
	load_image(in);

	/*------------Start of filter code-------------*/
    int input;
    printf("welcome! please select a filter for cy.bmp\n1.) instagram\n2.) grayscale\n3.) banksey red\n4.) blur\n5.) sharpen\n6.) edge detection\n");
    scanf("%d", &input);
    switch(input){
        case 1 :
            printf("instagram...\n");
            filter_instagram();

            break;
        case 2 :
            printf("grayscale...\n");
            filter_grayscale();

            break;
        case 3 :
            printf("banksey red...\n");
            filter_red();

            break;
        case 4 :
            printf("blur...\n");
            filter_blur();

            break;
        case 5 :
            printf("sharpen...\n");
            filter_sharpen();
            
            break;
        case 6 :
            printf("edge detection...\n");
            filter_edge();
            
            break;
    return 0;
    }
	/*------------End of filter code-------------*/

	write_image(out, in);
	fclose(in);
	fclose(out);
}
