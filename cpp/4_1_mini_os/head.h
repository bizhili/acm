#ifndef HEAD_H
#define HEAD_H
#include <stdio.h>
#include <stdlib.h>
#include "cv_stdlib.h"
#include "statu.h"

#define P_NUM 3  //testing processing number
#define P_TIME 50//the priority of a process = P_TIME-runtime
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

FILE *pFile;
enum state{            /*枚举类型*/
	future,
	ready,
	execute,
	block,
	finish
};

struct pcb{             /*进程控制块数据结构*/
	char name[4];
	u16 startTime;//set :ms
	u16 endTime;
	fp32 priority;
	int needtime;//set :ms
	int leastTime;
	int waitTime;
	int cputime;//set :ms
	int count;
	int round;
	enum state process;
	struct pcb * next;
};
extern struct pcb * get_process();
extern struct pcb;


#endif
