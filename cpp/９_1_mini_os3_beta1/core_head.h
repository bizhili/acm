#include "stdio.h"
#include "setjmp.h"
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
#define JB_RBP        1
#define JB_RSP        6
#define JB_PC        7

enum state{            /*枚举类型*/
	future,
	ready,
	execute,
	block,
	finish
};


struct pcb{             /*进程控制块数据结构*/
    u16 pid;
	u32 startTime;//set :ms
    u32 save_startTime;//set :ms
	u32 endTime;
	fp32 priority;
	u16 timePriority;
    u32 sleepTime;
	int waitTime;
	int cputime;//set :ms
    unsigned char* stack;
	jmp_buf buf;
	enum state process_state;
	struct pcb * next;
    void (*p)(void);
};

struct init_pcb{
    u32 startTime;//ms
    u32 stackSpace;//bytes
    u32 time_priority;//ms
    void (*p)(void);//processing function
};
unsigned long PTR_MANGLE(unsigned long var)
{
    asm (   "movq %1, %%rdx \n"
            "xor    %%fs:0x30, %%rdx\n"
            "rol    $0x11,%%rdx\n"
            "movq %%rdx, %0\t\n"
            : "=r" (var)
            :"0" (var));
    return var;
}

unsigned long PTR_DEMANGLE(unsigned long var)
{
    asm (   "ror $0x11, %0\n"
            "xor %%fs:0x30, %0"
            : "=r" (var)
            : "0" (var));
    return var;
}




