#include "main.h"

int main()
{
    clock_t time1,time2;
    u32 count1=1;
    time1=clock();
    printf("hello world");
    printf("\n%d",time1);
    os_delay_ms(5000);
    time2=clock();
    printf("\n%d",CLOCKS_PER_SEC);
    return 0;
}



















