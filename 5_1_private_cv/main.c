#include "main.h"
#include "math.h"
#include "stdio.h"
#include "cv_ahead.h"

#include "draw.h"
#include "vector.h"
#define more 10
void PIC_go_error(char *mes)
{
    printf("\nPIC error:");
    printf(mes);
    exit(0);
}
void CV_go_error(char *mes)
{
    printf("\nCV error:");
    printf(mes);
    exit(0);
}
void PIC_free(PIC *pic)
{
    if(pic->channel==3)
    {
        free(pic->r.element);
        free(pic->g.element);
        free(pic->b.element);
        pic->r.element=0;
        pic->g.element=0;
        pic->b.element=0;
    }
    else
    {
        free(pic->r.element);
        pic->r.element=0;
    }

    free(pic);

    pic=0;
}
PIC *PIC_new(u16 width,u16 height,unsigned char channel)
{
    PIC *pic=0;
    if(channel==3)
    {

        pic=(PIC *)malloc(1*sizeof(PIC));
        pic->b.element=(unsigned char *)malloc(width*height+more);
        pic->r.element=(unsigned char *)malloc(width*height+more);
        pic->g.element=(unsigned char *)malloc(width*height+more);
        pic->b.col=width;
        pic->b.row=height;
        pic->g.col=width;
        pic->g.row=height;
        pic->r.col=width;
        pic->r.row=height;
        pic->channel=3;
        return pic;
    }
    else if(channel==1)
    {
        pic=(PIC *)malloc(1*sizeof(PIC));
        pic->r.element=(unsigned char *)malloc(width*height+more);
        pic->g.element=0;
        pic->b.element=0;
        pic->r.col=width;
        pic->r.row=height;
        pic->channel=1;
        return pic;
    }
    else PIC_go_error("channel should be 1 or 2 at:PIC_new(PIC *pic,u16 width,u16 height,unsigned char channel)");
}
PIC *PIC_copy(PIC *pic)
{
    PIC *mypic=0;
    if(pic->channel==3)
    {

        mypic=PIC_new(pic->b.col,pic->b.row,3);
        mypic->channel=3;
        for(int count=0;count<pic->b.col*pic->b.row;count++)
            mypic->b.element[count]=pic->b.element[count];
        for(int count=0;count<pic->b.col*pic->b.row;count++)
            mypic->g.element[count]=pic->g.element[count];
        for(int count=0;count<pic->b.col*pic->b.row;count++)
            mypic->r.element[count]=pic->r.element[count];

    }
    else
    {
        mypic=PIC_new(pic->r.col,pic->r.row,1);
        for(int count=0;count<pic->r.col*pic->r.row;count++)
        mypic->r.element[count]=pic->r.element[count];
        mypic->channel=1;
    }
    return mypic;
}
void CV_inRange(PIC *pic,RGBQuAD lower,RGBQuAD upper)
{
    if(pic->channel==3)
    {
        for(int count=0;count<pic->r.col*pic->r.row;count++)
        {
            if(pic->r.element[count]<upper.rgbRed&&pic->r.element[count]>lower.rgbRed)
            {
                pic->r.element[count]=0;
                continue;
            }
            if(pic->g.element[count]<upper.rgbGreen&&pic->g.element[count]>lower.rgbGreen)
            {
                pic->r.element[count]=0;
                continue;
            }
            if(pic->b.element[count]<upper.rgbBlue&&pic->b.element[count]>lower.rgbBlue)
            {
                pic->r.element[count]=0;
                continue;
            }
            pic->r.element[count]=255;
        }
        pic->channel=1;
        free(pic->b.element);
        pic->b.element=0;
        free(pic->g.element);
        pic->g.element=0;
        return ;
    }
    return;
}
void cv_white_blance(PIC *pic)
{
    if(pic->channel!=3)
        PIC_go_error("channel should be 3 at:cv_white_blance(PIC *pic)");
    u32 sumR,sumG,sumB,sumAll;
    float giveR,giveG,giveB;
    sumR=Mat_sum(&pic->r);
    sumG=Mat_sum(&pic->g);
    sumB=Mat_sum(&pic->b);
    sumAll=(long long)(sumB+sumG+sumR)/3;
    giveR=(float)sumAll/sumR;
    giveG=(float)sumAll/sumG;
    giveB=(float)sumAll/sumB;
    for(int count=0;count<pic->r.col*pic->r.row;count++)
    {
        pic->b.element[count]*=giveB;
        pic->g.element[count]*=giveG;
        pic->r.element[count]*=giveR;
    }
    return ;
}
u8 protected_MedianBlur_fuction(u16 count,u16 count1,u8 ksize,PIC *pic)
{
    u16 xThis=count1-ksize,yThis=count-ksize;
    u16 x1,y1;
    u16 sum=0;
    u16 k2=1;
    for(int count2=0;count2<ksize;count2++)
        for(int count3=0;count3<ksize;count3++)
        {
            x1=xThis+count3,y1=yThis+count2;
            if(y1*pic->r.col+x1<pic->r.col*pic->r.row)
            {
                sum+=pic->r.element[y1*pic->r.col+x1];
                k2++;
            }
        }
    sum=sum/k2;
    return sum;
}
PIC *cv_medianBlur(PIC *pic,unsigned char ksize)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_medianBlur(PIC *pic,unsigned char ksize)");
    if(ksize%2==0)
        CV_go_error("ksize should be odd number at:cv_medianBlur(PIC *pic,unsigned char ksize)");
    if(ksize==1)
        return;
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    u16 xThis,yThis,sum,x1,y1;
    u16 k2=ksize*ksize;
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            xThis=count1-ksize,yThis=count-ksize;
            sum=0;
            for(int count2=0;count2<ksize;count2++)
                for(int count3=0;count3<ksize;count3++)
                {
                    x1=xThis+count3,y1=yThis+count2;
                    sum+=pic->r.element[y1*pic->r.col+x1];
                }
            sum=sum/k2;
            temp->r.element[count*pic->r.col+count1]=sum;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_MedianBlur_fuction(count,count1,ksize,pic);
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_MedianBlur_fuction(count,count1,ksize,pic);
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_MedianBlur_fuction(count1,count,ksize,pic);
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_MedianBlur_fuction(count1,count,ksize,pic);
    return temp;
}
u8 protected_gaussianBlur_fuction(u16 count,u16 count1,u8 ksize,PIC *pic,float *core)
{
    u16 xThis=count1-ksize,yThis=count-ksize;
    u16 x1,y1;
    float sum=0;
    for(int count2=0;count2<ksize;count2++)
        for(int count3=0;count3<ksize;count3++)
        {
            x1=xThis+count3,y1=yThis+count2;
            if(y1*pic->r.col+x1<pic->r.col*pic->r.row)
                sum+=pic->r.element[y1*pic->r.col+x1]*core[count3+count2*ksize];
        }
    return (u8)sum;
}
PIC *cv_gaussianBlur(PIC *pic,unsigned char ksize,float sigmaXY)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_medianBlur(PIC *pic,unsigned char ksize,float sigmaX,float sigmaY)");
    if(ksize%2==0)
        CV_go_error("ksize should be odd number at:cv_medianBlur(PIC *pic,unsigned char ksize,float sigmaX,float sigmaY)");
    if(ksize==1)
        return;
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    float *gaussian_core;
    float value,value_all=0;
    float e=2.71828,sum;
    u16 xThis,yThis,x1,y1;
    u16 k2=ksize*ksize;
    u16 number=0;
    gaussian_core=(float *)malloc(sizeof(float)*ksize*ksize);
    for(int count=(int)-ksize/2;count<=(int)ksize/2;count++)
        for(int count1=(int)-ksize/2;count1<=(int)ksize/2;count1++)
        {
            value=pow(e,(double)-(count*count+count1*count1)/2/sigmaXY/sigmaXY);
            gaussian_core[number]=value;
            number++;
            value_all+=value;
        }
    number=0;
    for(int count=(int)-ksize/2;count<=(int)ksize/2;count++)
        for(int count1=(int)-ksize/2;count1<=(int)ksize/2;count1++)
        {
            gaussian_core[number]/=value_all;
            number++;
        }
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            xThis=count1-ksize,yThis=count-ksize;
            sum=0;
            for(int count2=0;count2<ksize;count2++)
                for(int count3=0;count3<ksize;count3++)
                {
                    x1=xThis+count3,y1=yThis+count2;
                    sum+=(float)pic->r.element[y1*pic->r.col+x1]*gaussian_core[count3+count2*ksize];
                }
            temp->r.element[count*pic->r.col+count1]=(u8)sum;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_gaussianBlur_fuction(count,count1,ksize,pic,gaussian_core);
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_gaussianBlur_fuction(count,count1,ksize,pic,gaussian_core);
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_gaussianBlur_fuction(count1,count,ksize,pic,gaussian_core);
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_gaussianBlur_fuction(count1,count,ksize,pic,gaussian_core);
    free(gaussian_core);
    return temp;
}
u8 protected_erode_fuction(u16 count,u16 count1,u8 ksize,PIC *pic)
{
    u16 xThis=count1-ksize,yThis=count-ksize;
    u16 x1,y1;
    u16 sum=255;
    for(int count2=0;count2<ksize;count2++)
        for(int count3=0;count3<ksize;count3++)
        {
            x1=xThis+count3,y1=yThis+count2;
            if(y1*pic->r.col+x1<pic->r.col*pic->r.row)
            {
                if(pic->r.element[y1*pic->r.col+x1]<sum)
                    sum=pic->r.element[y1*pic->r.col+x1];
            }
        }
    return sum;
}
PIC *cv_erode(PIC *pic,unsigned char ksize)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_erode(PIC *pic,unsigned char ksize)");
    if(ksize%2==0)
        CV_go_error("ksize should be odd number at:cv_erode(PIC *pic,unsigned char ksize)");
    if(ksize==1)
        return;
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    u16 xThis,yThis,sum,x1,y1;
    u16 k2=ksize*ksize;
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            xThis=count1-ksize,yThis=count-ksize;
            sum=255;
            for(int count2=0;count2<ksize;count2++)
                for(int count3=0;count3<ksize;count3++)
                {
                    x1=xThis+count3,y1=yThis+count2;
                    if(pic->r.element[y1*pic->r.col+x1]<sum)
                        sum=pic->r.element[y1*pic->r.col+x1];
                }
            temp->r.element[count*pic->r.col+count1]=sum;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_erode_fuction(count,count1,ksize,pic);
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_erode_fuction(count,count1,ksize,pic);
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_erode_fuction(count1,count,ksize,pic);
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_erode_fuction(count1,count,ksize,pic);
    return temp;
}
u8 protected_dilate_fuction(u16 count,u16 count1,u8 ksize,PIC *pic)
{
    u16 xThis=count1-ksize,yThis=count-ksize;
    u16 x1,y1;
    u16 sum=0;
    for(int count2=0;count2<ksize;count2++)
        for(int count3=0;count3<ksize;count3++)
        {
            x1=xThis+count3,y1=yThis+count2;
            if(y1*pic->r.col+x1<pic->r.col*pic->r.row)
            {
                if(pic->r.element[y1*pic->r.col+x1]>sum)
                    sum=pic->r.element[y1*pic->r.col+x1];
            }
        }
    return sum;
}
PIC *cv_dilate(PIC *pic,unsigned char ksize)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_erode(PIC *pic,unsigned char ksize)");
    if(ksize%2==0)
        CV_go_error("ksize should be odd number at:cv_erode(PIC *pic,unsigned char ksize)");
    if(ksize==1)
        return;
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    u16 xThis,yThis,sum,x1,y1;
    u16 k2=ksize*ksize;
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            xThis=count1-ksize,yThis=count-ksize;
            sum=0;
            for(int count2=0;count2<ksize;count2++)
                for(int count3=0;count3<ksize;count3++)
                {
                    x1=xThis+count3,y1=yThis+count2;
                    if(pic->r.element[y1*pic->r.col+x1]>sum)
                        sum=pic->r.element[y1*pic->r.col+x1];
                }
            temp->r.element[count*pic->r.col+count1]=sum;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_dilate_fuction(count,count1,ksize,pic);
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=protected_dilate_fuction(count,count1,ksize,pic);
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_dilate_fuction(count1,count,ksize,pic);
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=protected_dilate_fuction(count1,count,ksize,pic);
    return temp;
}

PIC *cv_sobel(PIC *pic,u16 Threshold)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_sobel(PIC *pic,u16 Threshold)");
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    u16 x,y;
    u8 ksize=3;
    int Gx,Gy,G;
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            x=count1,y=count;
            Gx=(pic->r.element[(y-1)*pic->r.col+(x+1)]+2*pic->r.element[(y)*pic->r.col+(x+1)]+pic->r.element[(y+1)*pic->r.col+(x+1)])-
            (pic->r.element[(y-1)*pic->r.col+(x-1)]+2*pic->r.element[(y)*pic->r.col+(x-1)]+pic->r.element[(y+1)*pic->r.col+(x-1)]);
            Gy=(pic->r.element[(y-1)*pic->r.col+(x-1)]+2*pic->r.element[(y-1)*pic->r.col+(x)]+pic->r.element[(y-1)*pic->r.col+(x+1)])-
            (pic->r.element[(y+1)*pic->r.col+(x-1)]+2*pic->r.element[(y+1)*pic->r.col+(x)]+pic->r.element[(y+1)*pic->r.col+(x+1)]);
            G=abs(Gx)+abs(Gy);
            if(G>Threshold)
                temp->r.element[count*pic->r.col+count1]=0;
            else
                temp->r.element[count*pic->r.col+count1]=255;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=255;
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=255;
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=255;
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=255;
    return temp;
}

PIC *cv_laplacian(PIC *pic,u16 Threshold)
{
    if(pic->channel!=1)
        CV_go_error("channel should be 3 at:cv_sobel(PIC *pic,u16 Threshold)");
    PIC *temp;
    temp=PIC_new(pic->r.col,pic->r.row,1);
    u16 x,y;
    u8 ksize=3;
    int L;
    for(int count=ksize;count<pic->r.row-ksize;count++)
        for(int count1=ksize;count1<pic->r.col-ksize;count1++)
        {
            x=count1,y=count;
            L=pic->r.element[(y)*pic->r.col+(x+1)]+pic->r.element[(y)*pic->r.col+(x-1)]+
            pic->r.element[(y+1)*pic->r.col+(x)]+pic->r.element[(y-1)*pic->r.col+(x)]-
            4*pic->r.element[(y)*pic->r.col+(x)];
            if(abs(L)>Threshold)
                temp->r.element[count*pic->r.col+count1]=0;
            else
                temp->r.element[count*pic->r.col+count1]=255;
        }
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=255;
    for(int count=pic->r.row-ksize;count<pic->r.row;count++)
        for(int count1=0;count1<pic->r.col;count1++)
            temp->r.element[count*pic->r.col+count1]=255;
    for(int count=0;count<ksize;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=255;
    for(int count=pic->r.col-ksize;count<pic->r.col;count++)
        for(int count1=0;count1<pic->r.row;count1++)
            temp->r.element[count1*pic->r.col+count]=255;
    return temp;
}

int main()
{
    PIC *mypic=0,*copypic=0;
    RGBQuAD colorBasic={130,40,230},colorLine={140,55,255};
    RGBQuAD lower={64,130,60},upper={90,190,70};
    mypic=BMP_bmp_to_pic("pic\\p2.bmp");
    //copypic=PIC_copy(mypic);
    //CV_inRange(mypic,lower,upper);
    //cv_white_blance(mypic);
    //copypic=cv_medianBlur(mypic,5);
    //copypic=cv_gaussianBlur(mypic,3,1);
    //copypic=cv_dilate(mypic,7);
    copypic=cv_laplacian(mypic,500);
    BMP_pic_to_bmp(copypic,"pic\\p4.bmp");

    PIC_free(copypic);
    PIC_free(mypic);
    printf("hello");
    return 0;
}



