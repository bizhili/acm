/**
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  * @file       main.c/h
  * @brief      mini vector library
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0    2020/1/9         BIZHI           on programing
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  */
#include "stdio.h"
#include "vector.h"
#include "math.h"

static u8 testMat[]=
{
    2,56,3,4,5,6,
    1,56,3,4,5,6,
    1,56,3,4,5,6,
    1,56,3,4,5,6,
    1,56,3,4,5,6,
    1,56,3,4,5,6
};//6*6
static u8 testMat2[]=
{
    12,34,6,4,3,0,
    1,34,6,4,3,0,
    12,3,6,4,3,80,
    12,34,60,4,3,0,
    12,34,6,4,39,44,
    12,34,6,4,39,0
};//6*6
//create a special Mat


//operation of a Mat
Mat *new_Mat(u16 row,u16 col);
Mat *new_Mat_with_value(u16 row,u16 col,u8 value);
Mat *new_identity_Mat(u16 row);
void free_Mat(Mat *src1);

void Mat_x_value(Mat *src1,float value);
void Mat_add_value(Mat *src1,int value);

u8 *Mat_at(Mat *scr1,u16 row,u16 col);
void Mat_go_error(char *mes);
void Mat_printf(const char *fmt,Mat *scr1);
u32 Mat_sum(Mat *src1);
bool_t Mat_transpose(Mat *scr1);
bool_t Mat_invert(Mat *scr1);
int Mat_determinant(Mat *src1);
//operation between 2 Mats
bool_t Mat_compare_shape(Mat* src1, Mat* src2);
void Mat_caculate_Mat(Mat* src1, Mat* src2,CalculateType type);

Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst);
Mat* MatSub(Mat* src1, Mat* src2, Mat* dst);
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst);
Mat* MatTrans(Mat* src, Mat* dst);
float MatDet(Mat* mat);
Mat* MatAdj(Mat* src, Mat* dst);
Mat* MatInv(Mat* src, Mat* dst);

  /**
    * @brief          main example
    * @author         BIZHI
    * @param[in]
    * @return         zero
    * @waring         none
  */
int main6()
{
    Mat myMat,myMat2;
    myMat.element=testMat;
    myMat.col=6;
    myMat.row=6;
    myMat2.element=testMat2;
    myMat2.col=6;
    myMat2.row=6;
    Mat_x_value(&myMat,0.5);
    Mat_printf("%d",&myMat);
    free_Mat(&myMat);
    printf("Hello world!\n");
    return 0;
}
/**
* @brief          give Mat errors and go to end of the program
* @author         BIZHI
* @param[in]
* @return
* @waring
*/
void Mat_go_error(char *mes)
{
    printf("\n Mat error:");
    printf(mes);
    exit(0);
}
/**
* @brief          To create a new Mat with row*col
* @author         BIZHI
* @param[in]
* @return         a Mat which satisfied your needs
* @waring         row>0,col>0
*/
Mat *new_Mat(u16 row,u16 col)
{
    Mat *newMat;
    if(row&&col)
    {
        newMat=(Mat*)malloc(1);
        newMat->col=col;
        newMat->row=row;
        newMat->element=(u8*)malloc(row*col);
        return newMat;
    }
    Mat_go_error("row>0,col>0 at :*new_Mat(u16 row,u16 col)");
}
/**
* @brief          To create a new Mat with row*col and all value with value
* @author         BIZHI
* @param[in]
* @return         a Mat which satisfied your needs
* @waring         row>0,col>0,value<=255
*/
Mat *new_Mat_with_value(u16 row,u16 col,u8 value)
{
    Mat *newMat;
    if(row&&col)
    {
        newMat=(Mat*)malloc(1);
        newMat->col=col;
        newMat->row=row;
        newMat->element=(u8*)malloc(row*col);
        for(int count=0;count<row*col;count++)
            newMat->element[count]=value;
        return newMat;
    }
    Mat_go_error("row>0,col>0,0<=value<=255at :*new_Mat_with_value(u16 row,u16 col,u8 value)");
}
/**
* @brief          To get the sum of a Mat
* @author         BIZHI
* @param[in]
* @return         long long sum value
* @waring
*/
u32 Mat_sum(Mat *src1)
{
    u32 sum=0;

    for(int count=0;count<10;count++)
        sum+=src1->element[count];
    return sum;
}
/**
* @brief          To create a new row*row identity Mat
* @author         BIZHI
* @param[in]
* @return         a Mat which satisfied your needs
* @waring         row>0
*/
Mat *new_identity_Mat(u16 row)
{
    Mat *newMat;
    if(row)
    {
        newMat=(Mat*)malloc(1);
        newMat->col=row;
        newMat->row=row;
        newMat->element=(u8*)malloc(row*row);
        for(int count=0;count<row*row;count++)
            newMat->element[count]=0;
        for(int count=0;count<row;count++)
            newMat->element[count*row+count]=1;
        return newMat;
    }
    Mat_go_error("row>0 at :*new_identity_Mat(u16 row)");
}
/**
* @brief          To free the space
* @author         BIZHI
* @param[in]
* @return         none
* @waring         must use new_Mat() first.
*/
void free_Mat(Mat *src1)
{
    free(src1->element);
    free(src1);
}
/**
* @brief          return the pointer of the value
* @author         BIZHI
* @param[in]
* @return         pointer with type u8*
* @waring         row>0,col>0
*/
u8 *Mat_at(Mat *scr1,u16 row,u16 col)
{
    if(row<scr1->row&&col<scr1->col)
        return scr1->element+row*scr1->row+col;
    else
        Mat_go_error("input value out of range at:*Mat_at(Mat *scr1,u16 row,u16 col)");
}
/**
* @brief          compare the shape of two Mats
* @author         BIZHI
* @param[in]
* @return         0:not same,1same shape
* @waring         row>0,col>0
*/
bool_t Mat_compare_same(Mat* src1, Mat* src2)
{
    if(!src1->row||!src1->col||!src2->row||!src2->col)
        return 0;
    return (src1->row==src2->row&&src1->col==src2->col)? 1: 0;
}
/**
* @brief          print a Mat in different format
* @brief          example:
                            Mat_printf("%d",&mat1);
* @author         BIZHI
* @param[in]      standard printf(char *fmt,...)
* @return         none
* @waring         row>0,col>0
*/
void Mat_printf(const char *fmt,Mat *scr1)
{
    printf("\n");
    if(Mat_compare_same(scr1,scr1))
    {
        for(int count=0;count<scr1->row;count++)
        {
            for(int count1=0;count1<scr1->col;count1++)
                    printf(fmt,scr1->element[scr1->col*count+count1]);
            printf("\n");
        }
        return;
    }
    Mat_go_error("size error at:Mat_printf(const char *fmt,Mat *scr1)");
}
/**
* @brief          transpose a Mat
* @author         BIZHI
* @param[in]
* @return         0:failed,wrong format;1:succeed
* @waring         one Mat with the same rows and columns
*/
bool_t Mat_transpose(Mat *scr1)
{
    u8 *point,*point1;
    if(Mat_compare_same(scr1,scr1)&&scr1->col==scr1->row)
    {
        for(int count=0;count<scr1->row;count++)
            for(int count1=count+1;count1<scr1->col;count1++)
            {
                point=Mat_at(scr1,count,count1);
                point1=Mat_at(scr1,count1,count);
                *point^=*point1;
                *point1^=*point;
                *point^=*point1;
            }
        return 1;
    }
    else return 0;
}

/**
* @brief          calculate the invert of a Mat
* @author         BIZHI
* @param[in]
* @return         0:failed,wrong format;1:succeed
* @waring         one Mat with the same rows and columns
*/

/*
bool_t Mat_invert(Mat *scr1,Mat *scr2)
{
    ;
}
*/
/**
* @brief          calculate between two Mats with your need and give value to the first Mat
* @author         BIZHI
* @param[in]
* @return         none
* @waring         see CalculateType in main.h
*/

void Mat_caculate_Mat(Mat* src1, Mat* src2,CalculateType type)
{
    int value=0;
    if(Mat_compare_same(src1,src2))
    {
        for(int count=0;count<src1->row*src1->col;count++)
        {
            switch(type)
            {
            case CalculatePlus:
                {
                    value=src1->element[count]+src2->element[count];
                    break;
                }
            case CalculateMinus:
                {
                    value=src1->element[count]-src2->element[count];
                    break;
                }
            case CalculateMultiply:
                {
                    value=src1->element[count]*src2->element[count];
                    break;
                }
            case CalculateDivide:
                {
                    value=src1->element[count]/src2->element[count];
                    break;
                }
            }
            if(value<0)
                src1->element[count]=0;
            else if(value>255)
                src1->element[count]=255;
            else
                src1->element[count]=value;
        }
        return ;
    }
    Mat_go_error("size error at:*Mat_abs_diff(Mat* src1, Mat* src2)");
}
/**
* @brief          add all value of Mat with value
* @author         BIZHI
* @param[in]
* @return         none
* @waring         bigger than 255 should be 255, lower than 0 should be 0
*/
void Mat_add_value(Mat *src1,int value)
{
    if(src1->col&&src1->row)
    {
        for(int count=0;count<src1->col*src1->row;count++)
        {
            if(value+src1->element[count]<0)
               src1->element[count]=0;
            else if(value+src1->element[count]>255)
               src1->element[count]=255;
            else
               src1->element[count]+=value;
        }
        return;
    }
    Mat_go_error("size error at :Mat_add_value(Mat *src1,int value)");
}

/**
* @brief          x all value of Mat with value
* @author         BIZHI
* @param[in]
* @return         none
* @waring         bigger than 255 should be 255, lower than 0 should be 0,value should be float type instead of fraction
*/
void Mat_x_value(Mat *src1,float value)
{
    if(src1->col&&src1->row)
    {
        for(int count=0;count<src1->col*src1->row;count++)
        {
            if(value*src1->element[count]<0)
               src1->element[count]=0;
            else if(value*src1->element[count]>255)
               src1->element[count]=255;
            else
               src1->element[count]*=value;
        }
        return;
    }
    Mat_go_error("size error at :Mat_x_value(Mat *src1,float value)");
}


int Mat_determinant(Mat *src1)
{
    ;
}

