#ifndef MAIN_H
#define MAIN_H

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef uint8_t bool_t;

typedef struct
{
    u16 row, col;
    u8 *element;
}Mat;
typedef struct tagRGBQuAD
{
    //定义每个像素的数据类型
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char rgbReserved; //保留值
}RGBQuAD;
typedef struct
{
    unsigned short x;
    unsigned short y;
}Axis;
typedef struct
{
    Mat r;//1 channel used
    Mat g;
    Mat b;
    unsigned char channel;
}PIC;

typedef enum
{
    CalculatePlus=0,
    CalculateMinus,
    CalculateMultiply,
    CalculateDivide,
}CalculateType;

extern void PIC_free(PIC *pic);
extern PIC *PIC_new(u16 width,u16 height,unsigned char channel);
#endif // MAIN_H
