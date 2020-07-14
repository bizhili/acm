#include "main.h"
#include "math.h"
#include "stdio.h"
#include "cv_ahead.h"
#include "draw.h"
#include "vector.h"
#include "cv_stdlib.h"

/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CV_INIT_3X3_DELTAS( deltas, step, nch )            \
	((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch),  \
	(deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),  \
	(deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),  \
	(deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))

static const Axis icvCodeDeltas[8] =
{ {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1} };
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
    if(threshold<countnum/400)
        threshold=(u16)countnum/400;
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
EdgeGraph *EdgeGraph_new()
{
    EdgeGraph *graph;
    graph=(EdgeGraph *)my_malloc(sizeof(EdgeGraph)*1);
    graph->edgeAll=(Edge *)my_malloc(sizeof(Edge)*EdgeNum);
    graph->edgeNum=0;
    graph->frameNum=1;
    return graph;
}
void Edge_push(EdgeGraph *graph)
{
    Edge *temp;
    if(graph->edgeNum<60)
    {
        graph->edgeAll[graph->edgeNum].flag=1;
        graph->edgeAll[graph->edgeNum].point=(Axis *)my_malloc(sizeof(Axis)*EdgePointNum);
        graph->edgeAll[graph->edgeNum].nextEdge=0;
    }
    else
    {
        graph->frameNum++;
        temp=(Edge *)my_malloc(sizeof(Edge)*EdgeNum*graph->frameNum);
        for(int count=0;count<EdgeNum*(graph->frameNum-1);count++)
            temp[count]=graph->edgeAll[count];
        my_free(graph->edgeAll);
        graph->edgeAll=temp;
        graph->edgeNum=0;
        graph->edgeAll[graph->edgeNum].flag=1;
        graph->edgeAll[graph->edgeNum].pointNum=0;
        graph->edgeAll[graph->edgeNum].point=(Axis *)my_malloc(sizeof(Axis)*EdgePointNum);
        graph->edgeAll[graph->edgeNum].nextEdge=0;
        graph->edgeAll[graph->edgeNum].allPointNum=0;
    }
    graph->edgeNum++;
}
void Edge_delete(EdgeGraph *graph,u16 edgeID)
{
    if(edgeID>graph->edgeNum+EdgeNum*(graph->frameNum-1)-1)
        CV_go_error("edgeId out of range at : EdgePoint_push(EdgeGraph *graph,u16 edgeID,Axis point)");
    graph->edgeAll[edgeID].flag=-1;
}
void EdgePoint_push(EdgeGraph *graph,u16 edgeID,Axis point)
{
    if(edgeID>graph->edgeNum+EdgeNum*(graph->frameNum-1)-1)
        CV_go_error("edgeId out of range at : EdgePoint_push(EdgeGraph *graph,u16 edgeID,Axis point)");
    Edge *tempEdge;
    tempEdge=graph->edgeAll+edgeID;
    while(tempEdge->flag)
        tempEdge=tempEdge->nextEdge;
    u16 pointNum=tempEdge->pointNum;
    if(pointNum<EdgePointNum)
    {
        tempEdge->point[pointNum].x=point.x;
        tempEdge->point[pointNum].y=point.y;
    }
    else
    {
        tempEdge->flag=0;
        tempEdge=tempEdge->nextEdge;
        tempEdge=(Axis *)my_malloc(sizeof(Axis)*EdgePointNum);
        tempEdge->flag=1;
        tempEdge->nextEdge=0;
        tempEdge->pointNum=0;
        pointNum-=EdgePointNum;
        tempEdge->point[pointNum].x=point.x;
        tempEdge->point[pointNum].y=point.y;
    }
    tempEdge->pointNum++;
    graph->edgeAll[edgeID].allPointNum=0;
}
Axis get_EdgePoint(EdgeGraph *graph,u16 edgeID,u32 pointNum)
{
    if(edgeID>graph->edgeNum)
        CV_go_error("edgeNum out of range at:get_EdgePoint(EdgeGraph *graph,u16 edgeID,u32 pointNum)");
    if(edgeID>graph->edgeAll[edgeID].allPointNum)
    {
        Axis zero={-1,-1};
        return zero;
    }
    u16 frame=graph->edgeAll[edgeID].allPointNum/EdgePointNum;
    u16 thisFrame=pointNum/EdgePointNum;
    u16 thisPoint=pointNum%EdgePointNum;
    Axis temp;
    Edge *head;
    head=graph->edgeAll+edgeID;
    for(int count=0;count<thisFrame;count++)
        head=head->nextEdge;
    temp.x=head->point[thisPoint].x;
    temp.y=head->point[thisPoint].y;
    return temp;
}


PIC *cv_findCountours(PIC *pic)
{
    if(pic->channel==3)
            CV_go_error("channel should be 3 at:cv_findCountours(PIC *pic)");
    int thisPixel,rootPixel,thatPixel,otherPixel,lastPixel=0;
    u8 ID=1,flag=0;
    PIC *temp=0;
    temp=PIC_new(pic->r.col,pic->r.row,3);
    u32 size=pic->r.col*pic->r.row;
    RGBQuAD colorAll[8]={{0,255,0},{255,0,0},{0,0,255},{0,255,255},{255,255,0},{255,0,255},{255,225,255},{100,100,100}};
    for(u32 count=0;count<size;count++)
    {
        temp->r.element[count]=255;
        temp->g.element[count]=255;7
        temp->b.element[count]=255;
    }
    int ccur_d=0,ccur_c=0;

    int step[8];
    CV_INIT_3X3_DELTAS(step,pic->r.col,1);

    for(int count=1;count<pic->r.row-1;count++)
        for(int count1=1;count1<pic->r.col-1;count1++)
        {
            thisPixel=count1+count*pic->r.col;
            if(pic->r.element[thisPixel]!=255)
            {
                rootPixel=thisPixel;
                flag=0;
                temp->r.element[thisPixel]=colorAll[ccur_c].rgbRed;
                temp->g.element[thisPixel]=colorAll[ccur_c].rgbGreen;
                temp->b.element[thisPixel]=colorAll[ccur_c].rgbBlue;
                pic->r.element[thisPixel]=255;
                while(flag<9)
                {
                    if(!flag)
                    {
                    temp->r.element[thisPixel]=colorAll[ccur_c].rgbRed;
                    temp->g.element[thisPixel]=colorAll[ccur_c].rgbGreen;
                    temp->b.element[thisPixel]=colorAll[ccur_c].rgbBlue;
                    pic->r.element[thisPixel]=255;
                    }
                    if(flag)
                    {
                        if(flag==1)
                            otherPixel=thisPixel;
                        thisPixel=otherPixel+step[flag-1];
                    }
                    for(int count2=0;count2<8;count2++)
                    {
                        if (ccur_d >= 8)
							ccur_d -= 8;
                        else if (ccur_d < 0)
							ccur_d += 8;
                        thatPixel=thisPixel+step[ccur_d];
                        if(thatPixel>0 && thatPixel<size)
                        {
                            if(!pic->r.element[thatPixel])
                            {
                                if(flag)
                                {
                                    flag=0;
                                    temp->r.element[thisPixel]=colorAll[ccur_c].rgbRed;
                                    temp->g.element[thisPixel]=colorAll[ccur_c].rgbGreen;
                                    temp->b.element[thisPixel]=colorAll[ccur_c].rgbBlue;
                                    pic->r.element[thisPixel]=255;
                                }
                                if(lastPixel)
                                    for(int count3=0;count3<8;count3++)
                                        if(!pic->r.element[lastPixel+step[count3]])
                                        {
                                            pic->r.element[lastPixel+step[count3]]=255;
                                            temp->r.element[lastPixel+step[count3]]=colorAll[ccur_c].rgbRed;
                                            temp->g.element[lastPixel+step[count3]]=colorAll[ccur_c].rgbGreen;
                                            temp->b.element[lastPixel+step[count3]]=colorAll[ccur_c].rgbBlue;
                                        }
                                lastPixel=thisPixel;
                                thisPixel=thatPixel;
                                ccur_d-=2;
                                break;
                            }
                        }
                        if(count2==7)
                            flag++;
                        ccur_d++;
                    }
                }
                ID++;
                ccur_d=0;
                if(ID==255)
                    ID=1;
                ccur_c++;
                if (ccur_c > 8)
                    ccur_c -= 8;
                else if (ccur_c < 0)
                    ccur_c += 8;
            }
        }
    return temp;
}
int main()
{
    PIC *mypic=0,*mypic2=0,*copypic=0;
    Line *getLines;
    RGBQuAD colorBasic={130,40,230},colorLine={140,55,255};
    RGBQuAD lower={170,80,70},upper={220,140,120},zerpC={0,0,0};
    mypic=BMP_bmp_to_pic("pic\\p4_3.bmp");
    //Draw_color_all(mypic,zerpC);
    //mypic2=BMP_bmp_to_pic("pic\\p1_8.bmp");
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
    //getLines=cv_houghLines(mypic,190);
    //printf("%d",Draw_line_hough(mypic2,getLines,colorLine));
    mypic2=cv_findCountours(mypic);
    BMP_pic_to_bmp(mypic2,"pic\\p6_9.bmp");
    free_all();
    printf("hello");

    return 0;

}
