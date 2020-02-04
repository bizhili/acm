//
//#include <stdio.h>
//#include <fcntl.h>
//#include <unistd.h>
//
//#define  RGB(r,g,b)         (((r>>3)<<11)|((g>>2)<<5)|(b>>3))
//
//typedef  unsigned char  U8 ;
//typedef  unsigned short U16 ;
//typedef  unsigned int   U32 ;
//
//#pragma  pack(1)
//struct bmp_header    //这个结构体就是对上面那个图做一个封装。
//{
//    //bmp header
//    U8  Signatue[2] ;   // B  M
//    U32 FileSize ;     //文件大小
//    U16 Reserv1 ;
//    U16 Reserv2 ;
//    U32 FileOffset ;   //文件头偏移量
//
//    //DIB header
//    U32 DIBHeaderSize ; //DIB头大小
//    U32 ImageWidth   ;  //文件宽度
//    U32 ImageHight   ;  //文件高度
//    U16 Planes       ;
//    U16 BPP          ;  //每个相素点的位数
//    U32 Compression  ;
//    U32 ImageSize    ;  //图文件大小
//    U32 XPPM ;
//    U32 YPPM ;
//    U32 CCT ;
//    U32 ICC ;
//};
//#pragma  pack()
//
//int main(int argc , char **argv)
//{
//
//
//    int fd ;
//    int dest_fd ;
//    //以只读的方式打开一个文件  这个文件是rgb888
//    fd = open("p1.bmp" , O_RDONLY);
//    if(-1 == fd)
//    {
//        perror("open bmp file fail");
//        return -2 ;
//    }
//
//    //打开一个rgb565.bmp的图片
//    dest_fd = open("p2.bmp" , O_RDWR | O_CREAT|O_TRUNC , 0777);
//    if(dest_fd < 0 )
//    {
//        perror("open rgb565 fail");
//        return -3 ;
//    }
//
//
//    //定义一个bmp的头
//    struct bmp_header  header ;
//
//    int ret ;
//
//    //读出bmp文件头的数据
//    ret = read(fd , &header , sizeof(struct bmp_header));
//
//    //以下是bmp图的相关数据
//    printf(" Signatue[0]      : %c  \n " , header.Signatue[0]  );
//    printf(" Signatue[1]      : %c  \n " , header.Signatue[1]  );
//    printf(" FileSize         : %d  \n " , header.FileSize     );
//    printf(" Reserv1          : %d  \n " , header.Reserv1      );
//    printf(" Reserv2          : %d  \n " , header.Reserv2      );
//    printf(" FileOffset       : %d  \n " , header.FileOffset   );
//    printf(" DIBHeaderSize    : %d  \n " , header.DIBHeaderSize);
//    printf(" ImageWidth       : %d  \n " , header.ImageWidth   );
//    printf(" ImageHight       : %d  \n " , header.ImageHight   );
//    printf(" Planes           : %d  \n " , header.Planes       );
//    printf(" BPP              : %d  \n " , header.BPP          );
//    printf(" Compression      : %d  \n " , header.Compression  );
//    printf(" ImageSize        : %d  \n " , header.ImageSize    );
//    printf(" XPPM             : %d  \n " , header.XPPM         );
//    printf(" YPPM             : %d  \n " , header.YPPM         );
//    printf(" CCT              : %d  \n " , header.CCT          );
//    printf(" ICC              : %d  \n " , header.ICC          );
//
//    char buffer[header.ImageSize] ;
//
//    read(fd , buffer , header.ImageSize);
//
//    close(fd);
//
//    //改变地方
//    header.BPP = 16 ;
//    header.FileSize = header.ImageWidth*header.ImageHight*2+54 ;
//    header.ImageSize = header.ImageWidth*header.ImageHight*2;
//
//    write(dest_fd , &header , sizeof(struct bmp_header));
//
//    int row , col ;
//    short data ;
//    char *p = NULL ;
//
//    for(row = 0 ; row < header.ImageHight ; row++)
//    {
//        for(col = 0 ; col < header.ImageWidth ; col++)
//        {
//            p =( buffer+(row*header.ImageWidth + col)*3);
//            data = RGB((unsigned char)(*(p+2)) , (unsigned char)(*(p                +1)) , (unsigned char )(*(p)));
//            write(dest_fd , &data , 2);
//        }
//    }
//
//    close(dest_fd);
//
//    return 0 ;
//}
//
