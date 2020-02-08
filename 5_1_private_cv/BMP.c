#include "BMP.h"
#include "main.h"
#include "stdio.h"
#include "Draw.h"
unsigned char *image1=0;

#pragma  pack(1)
typedef struct     //这个结构体就是对上面那个图做一个封装。
{
    //bmp header
    u8  Signatue[2] ;   // B  M
    u32 FileSize ;     //文件大小
    u16 Reserv1 ;
    u16 Reserv2 ;
    u32 FileOffset ;   //文件头偏移量
    //DIB header
    u32 DIBHeaderSize ; //DIB头大小
    u32 ImageWidth   ;  //文件宽度
    u32 ImageHight   ;  //文件高度
    u16 Planes       ;
    u16 BPP          ;  //每个相素点的位数
    u32 Compression  ;
    u32 ImageSize    ;  //图文件大小
    u32 XPPM ;
    u32 YPPM ;
    u32 CCT ;
    u32 ICC ;
}bmp_header;
#pragma  pack()
void bmp_print_info(char *name1)
{
    bmp_header bmpFileHeader;
    bmp_read(name1,&bmpFileHeader);
    free(image1);
	printf("位图文件头主要是对位图文件的一些描述:BMPFileHeader\n\n");
	printf("标识帧：%d,%d \n",bmpFileHeader.Signatue[0],bmpFileHeader.Signatue[1]);
	printf("BMP 文件大小 = %d 字节\n", bmpFileHeader.FileSize);
	printf("保留值1 = %d \n", bmpFileHeader.Reserv1);
	printf("保留值2 = %d \n", bmpFileHeader.Reserv2);
	printf("文件头的最后到图像数据位开始的偏移量 = %d 字节\n", bmpFileHeader.FileOffset);
	// 输出BMP文件的位图信息头的所有信息
	printf("\n\n位图信息头主要是对位图图像方面信息的描述:bmpFileHeader\n\n");
	printf("信息头的大小 = %d 字节\n", bmpFileHeader.DIBHeaderSize);
	printf("位图的高度 = %d \n", bmpFileHeader.ImageHight);
	printf("位图的宽度 = %d \n", bmpFileHeader.ImageWidth);
	printf("图像的位面数(位面数是调色板的数量,默认为1个调色板) = %d \n", bmpFileHeader.Planes);
	printf("每个像素的位数 = %d 位\n", bmpFileHeader.BPP);
	printf("压缩类型 = %d \n", bmpFileHeader.Compression);
	printf("图像的大小 = %d 字节\n", bmpFileHeader.ImageSize);
	printf("水平分辨率 = %d \n", bmpFileHeader.XPPM);
	printf("垂直分辨率 = %d \n", bmpFileHeader.YPPM);
	printf("使用的色彩数 = %d \n", bmpFileHeader.CCT);
	printf("重要的色彩数 = %d \n", bmpFileHeader.ICC);
}
void bmp_go_error(char *mes)
{
    printf("\n BMP error:");
    printf(mes);
    exit(0);
}

int bmp_read(char *filename,bmp_header *header)
{
    FILE *fp=0;
    if (!(fp = fopen(filename, "rb")))
        bmp_go_error("can not open file at: bmp_read(unsigned char *image, char *filename,bmp_header1 *header)");

    fread(header ,1, sizeof(bmp_header),fp);
    image1=(unsigned char *)malloc(header->ImageHight*header->ImageWidth*header->BPP/8+header->FileOffset-54);//

    if (image1==NULL)
        bmp_go_error("failed to malloc at:bmp_read(unsigned char *image, char *filename,bmp_header1 *header)");

    fread(image1,1,(size_t)(long long)(header->ImageHight*header->ImageWidth*header->BPP/8+header->FileOffset-54),fp);
    fclose(fp);
    return 0;
}
int bmp_write(char *filename,bmp_header *header1)//3 cannel
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };
    long width = header1->ImageWidth;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;

    long height = header1->ImageHight;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;

    long file_size = (long)header1->ImageWidth * (long)header1->ImageHight * 3 + 54;

    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    FILE *fp;
    if (!(fp = fopen(filename, "wb")))
        return -1;

    u8 zero=0xff;

    fwrite(header, sizeof(unsigned char), 54, fp);


    if(header1->BPP==24)//24 bits color
        fwrite(image1, sizeof(unsigned char), (size_t)(long)width * height * 3+header1->FileOffset-54, fp);

    else if(header1->BPP==32)//32bits color
        for(int count=0;count<(width*height*4-header1->FileOffset)/4;count++)//312,123,321,231,213,
        {
            fwrite(&image1[header1->FileOffset-54+count*4+0], sizeof(unsigned char),1, fp);
            fwrite(&image1[header1->FileOffset-54+count*4+1], sizeof(unsigned char),1, fp);
            fwrite(&image1[header1->FileOffset-54+count*4+2], sizeof(unsigned char),1, fp);
        }
    for(int count1=0;count1<2048;count1++)
            fwrite(&zero, sizeof(unsigned char),1, fp);
    fclose(fp);

    return 0;
}
int bmp_write_1_channel(char *filename,bmp_header *header1)//1 cannel
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 8, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };
    long file_size = (long)header1->ImageWidth * (long)header1->ImageHight  + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    long width = header1->ImageWidth;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    long height = header1->ImageHight;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    FILE *fp = NULL;
    fp =fopen(filename, "wb");
    RGBQuAD pRGB[256];//调色板
    for(int i = 0;i < 256;i++){
        pRGB[i].rgbRed = i;
        pRGB[i].rgbGreen = i;
        pRGB[i].rgbBlue = i;
        pRGB[i].rgbReserved = 0;
    }
    u32 offset_map=54 + 4 * 256;
    header[10] = offset_map &0x000000ff;
    header[11] = (offset_map >> 8) &0x000000ff;
    header[12] = (offset_map >> 16) &0x000000ff;
    header[13] = (offset_map >> 24) &0x000000ff;
    fwrite(header, sizeof(unsigned char), 54, fp);
    fwrite(&pRGB,4*256,1,fp);
    fwrite(image1, sizeof(unsigned char), (size_t)(long)width * height , fp);
    fclose(fp);
    return 0;
}

void BMP_clonebmp(char *name1,char *name2)
{
    PIC *mypic;
    mypic=BMP_bmp_to_pic(name1);
    BMP_pic_to_bmp(mypic,name2);
    free(&mypic);
}

int BMP_color(RGBQuAD *color, int xsize, int ysize, char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };
    int i;
    int j;
    long file_size = (long)xsize * (long)ysize * 3 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    long width;
    if(!(xsize%4))
        width=xsize;
    else
        width= xsize+(4-xsize%4);    //如不是4的倍数，则转换成4的倍数
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    long height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    FILE *fp;
    if (!(fp = fopen(filename, "wb")))
        return -1;
    fwrite(header, sizeof(unsigned char), 54, fp);
    RGBQuAD color1;
    color1.rgbBlue=color->rgbBlue;
    color1.rgbGreen=color->rgbGreen;
    color1.rgbRed=color->rgbRed;
    for(j=0; j<ysize*xsize; j++)
                fwrite(&color1, sizeof(RGBQuAD)-1,1, fp);
    fclose(fp);
    return 0;
}
int BMP_1_channel_color(unsigned char color, int xsize, int ysize, char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 8, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };
    int i;
    int j;
    long file_size = (long)xsize * (long)ysize + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    long width;
    if(!(xsize%4))
        width=xsize;
    else
        width= xsize+(4-xsize%4);    //如不是4的倍数，则转换成4的倍数
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    long height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    FILE *fp;
    if (!(fp = fopen(filename, "wb")))
        return -1;

    unsigned char color1=color;
    RGBQuAD pRGB[256];//调色板
    for(int i = 0;i < 256;i++){
        pRGB[i].rgbRed = i;
        pRGB[i].rgbGreen = i;
        pRGB[i].rgbBlue = i;
        pRGB[i].rgbReserved = 0;
    }
    u32 offset_map=54 + 4 * 256;
    header[10] = offset_map &0x000000ff;
    header[11] = (offset_map >> 8) &0x000000ff;
    header[12] = (offset_map >> 16) &0x000000ff;
    header[13] = (offset_map >> 24) &0x000000ff;
    fwrite(header, sizeof(unsigned char), 54, fp);
    fwrite(&pRGB,4*256,1,fp);
    for(j=0; j<ysize*xsize; j++)
                fwrite(&color1,sizeof(unsigned char),1, fp);
    fclose(fp);
    return 0;
}
PIC *BMP_bmp_to_pic(char *fileName)
{
    PIC *pic=0;
    bmp_header header;

    bmp_read(fileName,&header);

    if(header.BPP==24||header.BPP==32)
    {

        pic=PIC_new(header.ImageWidth,header.ImageHight,3);
        if(header.BPP==32)
        {
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->b.element[count]=image1[count*4+2+header.FileOffset-54];
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->g.element[count]=image1[count*4+1+header.FileOffset-54];
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->r.element[count]=image1[count*4+header.FileOffset-54];
        }
        else
        {
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->b.element[count]=image1[count*3+2+header.FileOffset-54];
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->g.element[count]=image1[count*3+1+header.FileOffset-54];
            for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
                pic->r.element[count]=image1[count*3+header.FileOffset-54];
        }
    }
    else if(header.BPP=8)
    {
        pic=PIC_new(header.ImageWidth,header.ImageHight,1);
        for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
            pic->r.element[count]=image1[count+header.FileOffset-54];
    }
    free(image1);
    return pic;
}
int BMP_pic_to_bmp(PIC *pic,char *fileName)
{

    bmp_header header;

    header.ImageHight=pic->r.row;
    header.ImageWidth=pic->r.col;

    if(pic->channel==3)
    {
        header.FileOffset=54;
        image1=(unsigned char *)malloc(header.ImageHight*header.ImageWidth*3);
        for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
        {
            image1[3*count]=pic->r.element[count];
            image1[3*count+1]=pic->g.element[count];
            image1[3*count+2]=pic->b.element[count];
        }
        header.BPP=24;
        bmp_write(fileName,&header);
        free(image1);
    }
    else if(pic->channel==1)
    {
        header.FileOffset=54+256;
        image1=(unsigned char *)malloc(header.ImageHight*header.ImageWidth);
        for(int count=0;count<header.ImageHight*header.ImageWidth;count++)
            image1[count]=pic->r.element[count];
        header.BPP=8;
        bmp_write_1_channel(fileName,&header);
        free(image1);
    }
    return 1;
}

int main2()
{
    BMP_1_channel_color(100,140,200,"pic\\one.bmp");//生成单色8位图片
    BMP_clonebmp("pic\\p1.bmp","pic\\p2.bmp");  //实现一副图像的拷贝,suitable for 32 bits and 24 bits and 8 bits
    RGBQuAD color={0,255,255},lower={0,0,0},upper={200,200,200}; //一行像素值数组
    BMP_color(&color,100,1000,"pic\\p4.bmp");//制作固定大小单色图片
    PIC *mypic;
    mypic=BMP_bmp_to_pic("pic\\p1_1.bmp");//读取
    CV_inRange(mypic,lower,upper);//二值化
    BMP_pic_to_bmp(mypic,"pic\\p5.bmp");//保存
    PIC_free(mypic);
    printf("hello");
    getchar();
    return -1;
}
