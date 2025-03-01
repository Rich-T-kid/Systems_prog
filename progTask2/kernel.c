#include <stdio.h>
#include <math.h>
#include <time.h>


void convert_to_YCrCb(unsigned char *rgb_pixels, unsigned char *ycc_pixels, int width, int height){
    if (width == 0 || height == 0) {
        for (int i = 0; i < width * height * 3; i++) {
            ycc_pixels[i] = 0;
        }
        return;
    }
    
    //printf("\n%d",*rgb_pixels);
    //printf("\n%d",*(rgb_pixels+1));
    //printf("\n%d",*(rgb_pixels+2));
    // pixels*3 = bytes, go by 3 by 3 bytes each
    for(int i = 0; i < width*height*3; i+=3){
        unsigned int r = *(rgb_pixels+i);
        unsigned int g = *(rgb_pixels+(i+1));
        unsigned int b = *(rgb_pixels+(i+2));
        char y = (char) round(0.299 * r + 0.587 * g + 0.114 * b);
        char cb = (char) round(128 - 0.168736 * r - 0.331264 * g + 0.5 * b);
        char cr = (char) round(128 + 0.5 * r - 0.418688 * g - 0.081312 * b);
        *(ycc_pixels+i) = y;
        *(ycc_pixels+(i+1)) = cb;
        *(ycc_pixels+(i+2)) = cr;
    }
    return;
}

