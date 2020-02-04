#ifndef DRAW_H
#define DRAW_H

#include "main.h"

extern void Draw_color_all(PIC *pic,RGBQuAD color);
extern void Draw_line(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color,unsigned short pixel);
extern void Draw_cirle(PIC *pic,Axis center,unsigned short rad,RGBQuAD color,unsigned short pixel);
extern void Draw_rectangle(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color,unsigned short pixel);
extern void Draw_point(PIC *pic,Axis axis,RGBQuAD color);

#endif // MAIN_H
