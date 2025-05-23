#include "bmp.h"
#include <fcntl.h>

int main() {
    FILE* titles;
    char* title = NULL;
    size_t len = 0;

    bmp_bfh* immagine_header;
    bmp_dib* immagine_dib;
    bmp_bd* immagine_bd;

    immagine_header = malloc(sizeof(bmp_bfh));
    immagine_dib = malloc(sizeof(bmp_dib));
    immagine_bd = malloc(sizeof(bmp_bd));
    titles = fopen("names.txt", "r");
    while (getline(&title, &len, titles) != -1) {
        *strchr(title, '\n') = '\0';
        // printf("%s\n", title);
        int file = open(title, O_RDONLY);
        // printf("%s", title);

        fill_bfh(immagine_header, file);
        fill_dib(immagine_dib, file);
        bmp_CR(immagine_header);
        convertCTI(immagine_dib);
        fill_bd(immagine_bd, immagine_dib, immagine_header);
        // draw_image(immagine_bd, file);
        // draw_image_HD(immagine_bd, file);
        draw_image_QHD(immagine_bd, file);
        fflush(stdout);
        usleep(16300);
        close(file);
    }
    free(immagine_header);
    free(immagine_dib);
    free(immagine_bd);

    return EXIT_SUCCESS;
}
