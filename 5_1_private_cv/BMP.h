#ifndef BMP_H
#define BMP_H
#include "main.h"
#include <fcntl.h>
#include "stdlib.h"

extern void BMP_clonebmp(char *name1,char *name2);
extern int BMP_color(RGBQuAD *color, int xsize, int ysize, char *filename);
extern PIC *BMP_bmp_to_pic(char *fileName);
extern int BMP_pic_to_bmp(PIC *pic,char *fileName);
extern void bmp_print_info(char *name1);
#endif // MAIN_H
