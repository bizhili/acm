#include "iostream"


int main()
{
    char in,*p;
    char *pic="  1  \n 111 \n11111\n 111 \n  1  ";
    p=pic;
    in=getchar();
    while (*p)
    {
        *p=='1' ? printf("%c",in) : printf("%c",*p);
        p++;
    }
}