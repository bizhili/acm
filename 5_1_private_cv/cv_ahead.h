#ifndef CV_AHEAD_H
#define CV_AHEAD_H
#include "main.h"
extern void CV_go_error(char *mes);
extern void PIC_free(PIC *pic);
extern PIC *PIC_new(u16 width,u16 height,unsigned char channel);
extern PIC *PIC_copy(PIC *pic);
extern void CV_inRange(PIC *pic,RGBQuAD lower,RGBQuAD upper);
extern void cv_white_blance(PIC *pic);
extern PIC *cv_medianBlur(PIC *pic,unsigned char ksize);
extern PIC *cv_gaussianBlur(PIC *pic,unsigned char ksize,float sigmaXY);
extern PIC *cv_erode(PIC *pic,unsigned char ksize);
extern PIC *cv_dilate(PIC *pic,unsigned char ksize);
extern PIC *cv_sobel(PIC *pic,u16 Threshold);
extern PIC *cv_laplacian(PIC *pic,u16 Threshold);
extern PIC *cv_canny(PIC *pic,u16 lower_threshold,u16 upper_threshod);
extern PIC *cv_pyrDownHalf(PIC *pic);
extern PIC *cv_pyrUpTwice(PIC *pic);
extern PIC *cv_otsuThreshold(PIC *pic);
extern PIC *cv_color_to_gray(PIC *pic,GrayMethod method);
extern PIC *cv_adaptiveThreshold(PIC *pic,u8 ksize,float C);
#endif // MAIN_H
