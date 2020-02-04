#ifndef VECTOR_H
#define VECTOR_H
#include "main.h"

extern Mat *new_Mat(u16 row,u16 col);
extern Mat *new_Mat_with_value(u16 row,u16 col,u8 value);
extern Mat *new_identity_Mat(u16 row);
extern void free_Mat(Mat *src1);
extern void Mat_x_value(Mat *src1,float value);
extern void Mat_add_value(Mat *src1,int value);
extern u8 *Mat_at(Mat *scr1,u16 row,u16 col);
extern void Mat_go_error(char *mes);
extern void Mat_printf(const char *fmt,Mat *scr1);
extern bool_t Mat_transpose(Mat *scr1);
extern bool_t Mat_invert(Mat *scr1);
extern int Mat_determinant(Mat *src1);
//operation between 2 Mats
extern bool_t Mat_compare_shape(Mat* src1, Mat* src2);
extern void Mat_caculate_Mat(Mat* src1, Mat* src2,CalculateType type);
extern Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst);
extern Mat* MatSub(Mat* src1, Mat* src2, Mat* dst);
extern Mat* MatMul(Mat* src1, Mat* src2, Mat* dst);
extern Mat* MatTrans(Mat* src, Mat* dst);
extern float MatDet(Mat* mat);
extern Mat* MatAdj(Mat* src, Mat* dst);
extern Mat* MatInv(Mat* src, Mat* dst);
#endif // MAIN_H

