#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
struct context {
	unsigned long rsp;//stack head pointer
	unsigned long rbp;//stack buttom pointer
	unsigned long rip;//progress start
	unsigned long r12;//following save some import register
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
};
int os_delay_ms(int delay_time)
{
    clock_t time1;
    int delay_clock=delay_time*1000;
    time1=clock();
    while(clock()-time1<delay_clock);
    return 1;
}
unsigned char *stack1, *stack2;
struct context *ctx1, *ctx2;