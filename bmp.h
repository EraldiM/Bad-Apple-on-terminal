#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define br printf("\n")

typedef struct bmp_bd {
    int offset;
    int32_t height;
    int32_t width;
} bmp_bd;

typedef struct bmp_bfh {
    unsigned char header[3];  // 2 + 1 byte dedicated for the header
    unsigned char size[5];  // 4 + 1 byte dedicated for the size of the file
    unsigned char creator[5];  // 4 + 1 byte dedicated for the creator of the file
    unsigned char offset_bd[5];  // 4 + 1 byte dedicated for the bitmapdata offset
    int32_t Ioffset_bd;
} bmp_bfh;

typedef struct bmp_dib {
    unsigned char size[5];
    unsigned char bm_width[5];
    int32_t Ibm_width;
    unsigned char bm_height[5];
    int32_t Ibm_height;
    unsigned char color[2];
    unsigned char bitXpixel[3];
} bmp_dib;

void fill_bfh(bmp_bfh*, int);
void fill_dib(bmp_dib*, int);
void bmp_CR(bmp_bfh*, int);
void convertCTI(bmp_dib*);
void fill_bd(bmp_bd*, bmp_dib*, bmp_bfh*);
void draw_image(bmp_bd*, int);
short draw(char**);
void dump_hex_txt(bmp_bfh*, bmp_dib*, bmp_bd*, int);
void print_hex_pixel(bmp_bd*, int);
void fill_bd(bmp_bd*, bmp_dib*, bmp_bfh*);
short count_BP(unsigned char*);
void draw_image_HD(bmp_bd*, int);
short divide_pixel(char*, int);
