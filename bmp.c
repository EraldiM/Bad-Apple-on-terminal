#include "bmp.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// funzione che riepie la struct che identifica l'header dell'immagine
void fill_bfh(bmp_bfh *immagine, int file) {
  // lettura e scrittura dei primi 2 byte BM
  read(file, immagine->header, sizeof(immagine->header) - 1);

  // lettura delle dimensione del file
  read(file, immagine->size, sizeof(immagine->size) - 1);

  // lettura dei byte per il creatore dell'immagine
  read(file, immagine->creator, sizeof(immagine->creator) - 1);

  // lettura dell'offset per la posizione dei pixel
  read(file, immagine->offset_bd, sizeof(immagine->offset_bd) - 1);
}

// funzione che riempie la struct che identifica la DIB
void fill_dib(bmp_dib *immagine, int file) {
  // lettura e scrittura della dimensione del bitmap header
  lseek(file, 14, SEEK_SET);
  read(file, immagine->size, sizeof(immagine->size) - 1);

  // larghezza dell'immagine
  read(file, immagine->bm_width, 4);

  // altezza dell'immagine
  read(file, immagine->bm_height, 4);

  // numero del color plane (1)
  read(file, immagine->color, 2);

  // numero di bit per pixel
  read(file, immagine->bitXpixel, 2);
}

// funzione che converte il numero contenuto nel char che indentifica la
// posizione all'interno del file del primo pixel
void bmp_CR(bmp_bfh *immagine, int file) {
  immagine->Ioffset_bd = 0;
  for (int i = 0; i < 4; i++) {
    immagine->Ioffset_bd += (int)immagine->offset_bd[i] << (8 * i);
  }
}

// funzione che converte le altezze e le larghezze contenuto nel char letto da
// read in intero
void convertCTI(bmp_dib *immagine) {
  immagine->Ibm_width = 0;
  for (int i = 0; i < 4; i++)
    immagine->Ibm_width +=
        (int)immagine->bm_width[i]
        << (8 * i); // moltiplico per la potenza di 256 opportuna

  immagine->Ibm_height = 0;
  for (int i = 0; i < 4; i++)

    immagine->Ibm_height += (int)(immagine->bm_height[i] << (8 * i));
}

// funzione che legge i pixel a blocchi di 12 x 8
void draw_image(bmp_bd *immagine, int file) {
  short black;
  int row_size = ((immagine->width + 31) / 32) * 4;
  lseek(file, immagine->offset, SEEK_SET);
  unsigned char *pixel = malloc(12 * sizeof(char));
  printf("\033[2J");
  for (int row = 0; row < 30; row++) {
    for (int col = 0; col < 60; col++) {
      for (int i = 0; i < 12; i++) {
        read(file, &pixel[i], 1);
        if (i != 11)
          lseek(file, row_size - 1, SEEK_CUR);
        // salgo di una riga e torno indietro di un byte, ovvero vado all'inizio
        // della riga sopra
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
      if (col != 59)
        lseek(file, -row_size * 11, SEEK_CUR);
    }
    fflush(stdout);
    // sleep(1);
    // br;
  }
  free(pixel);
}

short count_BP(unsigned char *pixel) {
  short black = 0;
  for (int i = 0; i < 12; i++) {
    black += __builtin_popcount((unsigned char)pixel[i]);
  }
  return black;
}

void dump_hex_txt(bmp_bfh *header, bmp_dib *dib, bmp_bd *bd, int file) {
  write(file, header->header, 2);
  write(file, header->size, 4);
  write(file, header->creator, 4);
  write(file, header->offset_bd, 4);
}

void fill_bd(bmp_bd *to, bmp_dib *inf, bmp_bfh *inf2) {
  to->height = inf->Ibm_height;
  to->width = inf->Ibm_width;
  to->offset = inf2->Ioffset_bd;
}

void print_hex_pixel(bmp_bd *bd, int file) {
  char buf;
  lseek(file, bd->offset, SEEK_SET);
  int tmp = 1;
  for (int i = 1; i < 480 * 360 / 8 + 1; i++) {
    tmp = read(file, &buf, 1);
    printf("%02X", (unsigned char)buf);
    if (i % 60 == 0 && i != 0)
      br;
  }
  fflush(stdout);
}
