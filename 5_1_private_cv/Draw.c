#include "BMP.h"
#include "main.h"
#include "math.h"

void Draw_go_error(char *mes)
{
    free_all();
    printf("\n Draw error:");
    printf(mes);
    exit(0);
}
void Draw_1_channel_color_all(PIC *pic,unsigned char color)
{
    for(int count=0;count<pic->b.row*pic->b.col;count++)
        pic->r.element[count]=color;
}
void Draw_color_all(PIC *pic,RGBQuAD color)
{
    for(int count=0;count<pic->b.row*pic->b.col;count++)
        pic->b.element[count]=color.rgbBlue;
    for(int count=0;count<pic->b.row*pic->b.col;count++)
        pic->g.element[count]=color.rgbGreen;
    for(int count=0;count<pic->b.row*pic->b.col;count++)
        pic->r.element[count]=color.rgbRed;
}
u16 Draw_line_hough(PIC *pic,Line *lines,RGBQuAD color)
{
    float cosX,sinX,P,PI=3.1415926,cotX,Y,X;
    int thisP;
    int test,test1;
    test=lines[0].cita;
    test1=lines[0].distance;
    u16 countnum=0;
    for(int count=0;lines[count].cita || lines[count].distance;count++)
    {
        countnum++;
        P=lines[count].distance;
        cosX=cos((float)lines[count].cita/180*PI);
        sinX=sin((float)lines[count].cita/180*PI);
        if(sinX)
        {
            cotX=cosX/sinX;
            P/=sinX;
            for(int count1=1;count1<pic->r.col-1;count1++)
            {
                X=count1;
                Y=(int)(P-X*cotX);
                thisP=(int)X+Y*pic->r.col;
                if(X<pic->r.col && Y<pic->r.row &&Y>0)
                {
                    pic->r.element[(int)thisP]=color.rgbRed;
                    if(pic->channel==3)
                    {
                        pic->g.element[(int)thisP]=color.rgbGreen;
                        pic->b.element[(int)thisP]=color.rgbBlue;
                    }
                }

            }
        }
        else
        {
            X=P/cosX;
            for(int count1=3;count1<pic->r.row-3;count1++)
            {
                Y=count1;
                pic->r.element[(int)(X+Y*pic->r.col)]=color.rgbRed;
                if(pic->channel==3)
                    {
                        pic->g.element[(int)(X+Y*pic->r.col)]=color.rgbGreen;
                        pic->b.element[(int)(X+Y*pic->r.col)]=color.rgbBlue;
                    }
            }
        }
    }
    return countnum;

}
void Draw_line(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color,unsigned short pixel)
{
    u16 deltaX=abs(axis1.x-axis2.x),deltaY=abs(axis1.y-axis2.y);
    if(axis2.x-pixel<pic->b.col&&axis2.x+pixel<pic->b.col&&axis1.x-pixel<pic->b.col&&axis1.x+pixel<pic->b.col&&
       axis1.y<pic->b.row&&axis2.y<pic->b.row)
    {
        if(deltaX<deltaY)
        {
            for(int count1=-pixel;count1<pixel;count1++)
            {

                int unitX=1,unitY=1;
                if(deltaX)
                    unitX=(axis1.x-axis2.x)/deltaX;
                if(deltaY)
                    unitY=(axis1.y-axis2.y)/deltaY;
                u16 thisX,thisY;
                if(deltaX>deltaY)
                {
                    float deltaD=(float)deltaY/deltaX;
                    for(int count=0;count<deltaX;count++)
                    {
                            thisX=axis2.x+count1+unitX*count;
                            thisY=axis2.y+(float)unitY*count*deltaD;
                            pic->b.element[pic->b.col*thisY+thisX]=color.rgbBlue;
                            pic->g.element[pic->b.col*thisY+thisX]=color.rgbGreen;
                            pic->r.element[pic->b.col*thisY+thisX]=color.rgbRed;
                    }
                }
                else
                {
                    float deltaD=(float)deltaX/deltaY;
                    for(int count=0;count<deltaY;count++)
                    {
                            thisX=axis2.x+count1+(float)unitX*count*deltaD;
                            thisY=axis2.y+unitY*count;
                            pic->b.element[pic->b.col*thisY+thisX]=color.rgbBlue;
                            pic->g.element[pic->b.col*thisY+thisX]=color.rgbGreen;
                            pic->r.element[pic->b.col*thisY+thisX]=color.rgbRed;

                    }
                }
            }
        }
        else
        {
            for(int count1=-pixel;count1<pixel;count1++)
            {

                int unitX=1,unitY=1;
                if(deltaX)
                    unitX=(axis1.x-axis2.x)/deltaX;
                if(deltaY)
                    unitY=(axis1.y-axis2.y)/deltaY;
                u16 thisX,thisY;
                if(deltaX>deltaY)
                {
                    float deltaD=(float)deltaY/deltaX;
                    for(int count=0;count<deltaX;count++)
                    {
                            thisX=axis2.x+unitX*count;
                            thisY=axis2.y+count1+(float)unitY*count*deltaD;
                            pic->b.element[pic->b.col*thisY+thisX]=color.rgbBlue;
                            pic->g.element[pic->b.col*thisY+thisX]=color.rgbGreen;
                            pic->r.element[pic->b.col*thisY+thisX]=color.rgbRed;
                    }
                }
                else
                {
                    float deltaD=(float)deltaX/deltaY;
                    for(int count=0;count<deltaY;count++)
                    {
                            thisX=axis2.x+(float)unitX*count*deltaD;
                            thisY=axis2.y+count1+unitY*count;
                            pic->b.element[pic->b.col*thisY+thisX]=color.rgbBlue;
                            pic->g.element[pic->b.col*thisY+thisX]=color.rgbGreen;
                            pic->r.element[pic->b.col*thisY+thisX]=color.rgbRed;

                    }
                }
            }

        }
        return;
    }
    Draw_go_error("out of size at:Draw_line(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color)");
}

void Draw_cirle(PIC *pic,Axis center,unsigned short rad,RGBQuAD color,unsigned short pixel)
{
    if(center.x+rad+pixel<pic->b.col&&center.x-rad-pixel>0&&center.y+rad+pixel<pic->b.col&&center.y-rad-pixel>0)
    {
        for(int count1=-pixel;count1<=pixel;count1++)
        {
                int rad1=rad+count1;
                float thisX,thisY;
                int thisX1,thisY1;
                for(int count=0;count<rad1*12;count++)
                {
                    thisX=rad1*cos((float)count/rad1/6*3.1416)+center.x;
                    thisY=rad1*sin((float)count/rad1/6*3.1416)+center.y;
                    thisX1=(int)thisX,thisY1=(int)thisY;
                    pic->b.element[(pic->b.col*thisY1+thisX1)]=color.rgbBlue;
                    pic->g.element[(pic->b.col*thisY1+thisX1)]=color.rgbGreen;
                    pic->r.element[(pic->b.col*thisY1+thisX1)]=color.rgbRed;
                }
        }
        return ;
    }
    Draw_go_error("out of size at:Draw_cirle(PIC *pic,Axis center,unsigned short rad,RGBQuAD color,unsigned short pixel)");
}

void Draw_rectangle(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color,unsigned short pixel)
{
    if(axis1.x+pixel<pic->b.col&&axis1.x-pixel>0&&axis2.x+pixel<pic->b.col&&axis2.x-pixel>0&&
       axis1.y+pixel<pic->b.row&&axis1.y-pixel>0&&axis2.y+pixel<pic->b.row&&axis2.y-pixel>0)
    {
        Axis l1={axis1.x,axis2.y},l2={axis2.x,axis1.y};
        Draw_line(pic,l1,axis1,color,pixel);
        Draw_line(pic,l1,axis2,color,pixel);
        Draw_line(pic,axis2,l2,color,pixel);
        Draw_line(pic,l2,axis1,color,pixel);
        return;
    }
    Draw_go_error("out of size at: Draw_rectangle(PIC *pic,Axis axis1,Axis axis2,RGBQuAD color,unsigned short pixel)");
}
void Draw_1_channel_point(PIC *pic,Axis axis,unsigned char color)
{
    if(axis.x<pic->b.col&&axis.y<pic->b.row)
    {
        pic->b.element[(pic->b.col*axis.y+axis.x)]=color;
        return;
    }
    Draw_go_error("out of size at:Draw_point(PIC *pic,Axis axis,RGBQuAD color)");
}
void Draw_point(PIC *pic,Axis axis,RGBQuAD color)
{
    if(axis.x<pic->b.col&&axis.y<pic->b.row)
    {
        pic->b.element[(pic->b.col*axis.y+axis.x)]=color.rgbBlue;
        pic->g.element[(pic->b.col*axis.y+axis.x)]=color.rgbGreen;
        pic->r.element[(pic->b.col*axis.y+axis.x)]=color.rgbRed;
        return;
    }
    Draw_go_error("out of size at:Draw_point(PIC *pic,Axis axis,RGBQuAD color)");
}
int main4()
{
    PIC mypic;
    RGBQuAD colorBasic={0,255,0},colorLine={0,0,0};
    Axis start={20,20},end={130,210},center={60,60};
    PIC_new(&mypic,140,220);
    Draw_color_all(&mypic,colorBasic);
    //Draw_line(&mypic,start,end,colorLine,9);
    //Draw_cirle(&mypic,center,40,colorLine,14);
    Draw_rectangle(&mypic,start,end,colorLine,2);
    BMP_pic_to_bmp(&mypic,"p6.bmp");
    PIC_free(&mypic);
    printf("hello");
    return 0;
}
