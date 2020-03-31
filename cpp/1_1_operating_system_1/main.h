#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <time.h>
#include "cv_stdlib.h"

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef float fp32;
typedef double fp64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef uint8_t bool_t;
typedef enum
{
	ready=0,
	execute,
	block,
	finish
}state;
typedef struct pcb
{
	char name[4];
	u32 pid;
	int priority;
	int cputime;
	int needtime;
	int count;
	int round;
	state process;
	pcb * next;
}pcb;
bool_t os_delay_ms(u32 delay_time)
{
    clock_t time1;
    u32 delay_clock=delay_time*1000;
    time1=clock();
    while(clock()-time1<delay_clock);
    return 1;
}




#endif // MAIN_H
