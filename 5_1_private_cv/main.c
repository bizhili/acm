#include "main.h"
#include "math.h"
#include "stdio.h"
#include "cv_ahead.h"
#include "draw.h"
#include "vector.h"

Line *cv_houghLines(PIC *pic,u16 threshold)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 1 at: cv_houghLines(PIC *pic,int threshold,u8 pixel)");
    float PI=3.1415926;
    float *table;
    u32 MaxLength;
    u16 *check;
    MaxLength=(u32)sqrt(pic->r.row*pic->r.row+pic->r.col*pic->r.col)+1;
    table=(float *)malloc(4*91);
    check=(u16 *)malloc(2*MaxLength*180);
    u16 cita=0,distance=0,x=0,y=0;
    float P=0;
    for(int count=0;count<91;count++)
    {
        table[count]=sin((float)count/180*PI);
    }
    for(int count=0;count<MaxLength*180;count++)
        check[count]=0;
    int countnum=0;
    for(int count=0;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            if(! pic->r.element[count1+count*pic->r.col])
            {
                countnum++;
                x=count1,y=count;
                for(int count2=0;count2<91;count2++)
                {
                    P=(float)x*table[90-count2]+y*table[count2];
                    check[(u32)P*180+count2]++;
                }
                for(int count2=91;count2<180;count2++)
                {
                    P=(float)-x*table[count2-90]+y*table[180-count2];
                    if(P<0)
                        P=-P;
                    check[(u32)P*180+count2]++;
                }
            }
    Line *save;
    save=(Line *)malloc(sizeof(Line));
    if(threshold<countnum/270)
        threshold=(u16)countnum/270;
    u16 countnum1=0;
    for(int count=0;count<MaxLength;count++)
        for(int count1=0;count1<180;count1++)
            if(check[count1+count*180]>threshold)
                countnum1++;
    save=(Line *)malloc(sizeof(Line)*(countnum1+1));
    countnum1=0;
    for(int count=0;count<MaxLength;count++)
        for(int count1=0;count1<180;count1++)
            if(check[count1+count*180]>threshold)
            {
                save[countnum1].cita=count1;
                save[countnum1].distance=count;
                countnum1++;
            }
    save[countnum1].cita=0;
    save[countnum1].distance=0;
    free(table);
    free(check);
    return save;
}
int main()
{
    PIC *mypic=0,*copypic=0;
    Line *getLines;
    RGBQuAD colorBasic={130,40,230},colorLine={140,55,255};
    RGBQuAD lower={170,80,70},upper={220,140,120};
    mypic=BMP_bmp_to_pic("pic\\p4_7.bmp");
    //copypic=PIC_copy(mypic);
    //CV_inRange(mypic,lower,upper);
    //cv_white_blance(mypic);
    //copypic=cv_medianBlur(mypic,5);
    //copypic=cv_gaussianBlur(mypic,3,1);
    //copypic=cv_dilate(mypic,7);
    //copypic=cv_laplacian(mypic,500);
    //copypic=cv_medianBlur(mypic,3);
    //CV_inRange(mypic,lower,upper);
    //copypic=cv_sobel(mypic,400);
    getLines=cv_houghLines(mypic,200);
    //BMP_pic_to_bmp(copypic,"pic\\p4_7.bmp");
    //PIC_free(copypic);
    free(getLines);
    PIC_free(mypic);
    printf("hello");

    return 0;
}
