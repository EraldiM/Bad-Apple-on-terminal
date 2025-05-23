#include "bmp.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// This function fills the filds for the struct that describes the file header
void fill_bfh(bmp_bfh* immagine, int file) {
    // reading the firsts 2 bytes
    read(file, immagine->header, sizeof(immagine->header) - 1);

    // reading the dimensions
    read(file, immagine->size, sizeof(immagine->size) - 1);

    // reading the bytes for the creator of the image
    read(file, immagine->creator, sizeof(immagine->creator) - 1);

    // reading the offset for the pixel's position
    read(file, immagine->offset_bd, sizeof(immagine->offset_bd) - 1);
}

// This function fills the struct that describes the dib part
void fill_dib(bmp_dib* immagine, int file) {
    // reading the dib size
    lseek(file, 14, SEEK_SET);
    read(file, immagine->size, sizeof(immagine->size) - 1);

    // reading the image width
    read(file, immagine->bm_width, 4);

    // reading the image height
    read(file, immagine->bm_height, 4);

    // reading the number of color planes
    read(file, immagine->color, 2);

    // number of bits per pixel
    read(file, immagine->bitXpixel, 2);
}

// converting the size of the pixel offset from char little endian to decimal
void bmp_CR(bmp_bfh* immagine) {
    immagine->Ioffset_bd = 0;
    for (int i = 0; i < 4; i++) {
        immagine->Ioffset_bd += (int)immagine->offset_bd[i] << (8 * i);  // bit shifting is equivalent to multiply by the power of 256, this approach will appear again
    }
}

// converting the sizes from char little endian to int
void convertCTI(bmp_dib* immagine) {
    immagine->Ibm_width = 0;
    for (int i = 0; i < 4; i++)
        immagine->Ibm_width += (int)immagine->bm_width[i] << (8 * i);

    immagine->Ibm_height = 0;
    for (int i = 0; i < 4; i++)
        immagine->Ibm_height += (int)(immagine->bm_height[i] << (8 * i));
}

// functon that reads the pixels and creates the 12 x 8 block of pixel
void draw_image(bmp_bd* immagine, int file) {
    short black;
    int row_size = ((immagine->width + 31) / 32) * 4;
    lseek(file, immagine->offset, SEEK_SET);
    unsigned char* pixel = malloc(12 * sizeof(char));
    printf("\033[2J");
    for (int row = 0; row < 30; row++) {
        for (int col = 0; col < 60; col++) {
            for (int i = 0; i < 12; i++) {
                read(file, &pixel[i], 1);
                if (i != 11) lseek(file, row_size - 1, SEEK_CUR);
                // going one row up to read the pixel above
            }
            black = count_BP(pixel);
            printf("\033[%d;%dH", row, col + 30);
            if (black > 76) {
                printf("#");
            } else if (black != 0)
                printf("/");
            else
                printf(" ");
            fflush(stdout);
            if (col != 59) lseek(file, -row_size * 11, SEEK_CUR);  // going 11 rows down to start reading the next block of pixel
        }
        fflush(stdout);
    }
    free(pixel);
}

// counts how many black pixels are in the block
short count_BP(unsigned char* pixel) {
    short black = 0;
    for (int i = 0; i < 12; i++) {
        black += __builtin_popcount((unsigned char)pixel[i]);
    }
    return black;
}

void dump_hex_txt(bmp_bfh* header, int file) {
    write(file, header->header, 2);
    write(file, header->size, 4);
    write(file, header->creator, 4);
    write(file, header->offset_bd, 4);
}

// function that fills the bmp_bd strunct with the important information
void fill_bd(bmp_bd* to, bmp_dib* inf, bmp_bfh* inf2) {
    to->height = inf->Ibm_height;
    to->width = inf->Ibm_width;
    to->offset = inf2->Ioffset_bd;
}

// debug function to display the file content in hex
// little trivia: this function also displays the image in the terminal but
// stretched :)
void print_hex_pixel(bmp_bd* bd, int file) {
    char buf;
    lseek(file, bd->offset, SEEK_SET);
    for (int i = 1; i < 480 * 360 / 8 + 1; i++) {
        read(file, &buf, 1);
        printf("%02X", (unsigned char)buf);
        if (i % 60 == 0 && i != 0) br;
    }
    fflush(stdout);
}

void draw_image_HD(bmp_bd* immagine, int file) {
    int black;
    int row_size = (immagine->width + 31) / 32 * 4;
    lseek(file, immagine->offset, SEEK_SET);
    char* pixel = malloc(12 * sizeof(char));
    printf("\033[2J");

    for (int row = 0; row < 60; row++) {
        // we don't need to read twice the same pixel since we are reading 8 pixel each read() but we only need
        // the firts/last 4 since the quality (number of characters) is higher
        for (int col = 0; col < 60; col++) {
            for (int i = 0; i < 6; i++) {
                read(file, &pixel[i], 1);
                if (i != 5) lseek(file, row_size - 1, SEEK_CUR);
            }
            for (int i = 0; i < 2; i++) {
                black = divide_pixel(pixel, col + i);
                printf("\033[%d;%dH", row + 10, 2 * col + 70 + i);
                if (black >= 20) {
                    printf("#");
                } else if (black >= 15) {
                    printf("/");
                } else if (black >= 10) {
                    printf("*");
                } else if (black >= 6) {
                    printf(".");
                } else {
                    printf(" ");
                }
            }
            fflush(stdout);
            if (col != 59) lseek(file, -row_size * 5, SEEK_CUR);
        }
    }
}

short divide_pixel(char* pixel, int col) {
    short black = 0;
    if (col % 2) {
        for (int i = 0; i < 6; i++) {
            black += __builtin_popcount((unsigned char)(pixel[i] << 4));
        }
    } else {
        for (int i = 0; i < 6; i++) {
            black += __builtin_popcount((unsigned char)pixel[i] >> 4);
        }
    }
    return black;
}

void draw_image_QHD(bmp_bd* immagine, int file) {
    int black;
    int row_size = (immagine->width + 31) / 32 * 4;
    lseek(file, immagine->offset, SEEK_SET);
    char* pixel = malloc(12 * sizeof(char));
    printf("\033[2J");

    for (int row = 0; row < 120; row++) {
        for (int col = 0; col < 60; col++) {
            for (int i = 0; i < 3; i++) {
                read(file, &pixel[i], 1);
                if (i != 2) lseek(file, row_size - 1, SEEK_CUR);
            }
            for (int i = 0; i < 4; i++) {
                black = divide_pixel(pixel, col);
                printf("\033[%d;%dH", row + 10, 4 * col + 70 + i);
                if (black >= 11) {
                    printf("@");
                } else if (black >= 8) {
                    printf("^");
                } else if (black >= 6) {
                    printf("'");
                } else if (black >= 4) {
                    printf(",");
                } else if (black >= 2) {
                    printf(".");
                } else {
                    printf(" ");
                }
            }
            fflush(stdout);
            if (col != 59) lseek(file, -row_size * 2, SEEK_CUR);
        }
    }
}

short sub_divide_pixel(char* pixel, int col) {
    short black = 0;
    switch (col % 4) {
    case 0:
        for (int i = 0; i < 3; i++)
            black = __builtin_popcount((unsigned char)pixel[i] >> 6);
        break;
    case 1:
        for (int i = 0; i < 3; i++) {
            black = __builtin_popcount((unsigned char)(pixel[i] >> 4) % 16);
        }
        break;
    case 2:
        for (int i = 0; i < 3; i++) {
            black = __builtin_popcount((unsigned char)(pixel[i] % 16) >> 2);
        }
        break;
    case 3:
        for (int i = 0; i < 3; i++) {
            black = __builtin_popcount((unsigned char)(pixel[i] % 4));
        }
        break;
    }

    return black;
}
