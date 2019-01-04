// test connected component labeling algorithm of the library
#include <stdio.h>
#include <stdint.h>
#include "include/cbmp.h"

void matris_lab(BMP *P, int inmax)
{
    int i, j;

    // create three array to keep random colors
    unsigned char *redch   = (unsigned char *)calloc(inmax + 1, sizeof(unsigned char));
    unsigned char *greench = (unsigned char *)calloc(inmax + 1, sizeof(unsigned char));
    unsigned char *bluech  = (unsigned char *)calloc(inmax + 1, sizeof(unsigned char));

    // fill the arrays with random color for each input value
    for (i = 1; i <= inmax; i++)
    {
        redch[i]   = rand() % 200 + 55;
        greench[i] = rand() % 220 + 35;
        bluech[i]  = rand() % 180 + 75;
    }

    for (j = 0; j < P->bminfo.height; j++)
    {
        for (i = 0; i < P->bminfo.width; i++)
        {
            int label = P->pixels[i][j].red + 256 * P->pixels[i][j].green + 256*256*P->pixels[i][j].blue;

            P->pixels[i][j].red = redch[label];
            P->pixels[i][j].green = greench[label];
            P->pixels[i][j].blue = bluech[label];
        }
    }

    free(redch);
    free(greench);
    free(bluech);
}

int main()
{
    // read image
    BMP img = resim_oku("../data/bb.bmp");

    // convert input to grayscale and binarize the grayscale image
    BMP bwimg = resim_iki(img, 128);

    // find connected components
    int conn = 0;
    BMP label = resim_bbe(bwimg, &conn);

    // print the number of labels
    printf("Label: %d\n", conn);

    // assign different color for eack connected component
    matris_lab(&label, conn);

    // test output
    resim_yaz(label, "../data/bbrgb_cbmp.bmp");

    return 0;
}
