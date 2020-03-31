#include <stdio.h>
//#include <dos.h>
#include <stdlib.h>
#include <conio.h>
//#include <io.h>
#define P_NUM 3
#define P_TIME 50
FILE *pFile;
enum state{            /*枚举类型*/
	ready,
	execute,
	block,
	finish
};

struct pcb{             /*进程控制块数据结构*/
	char name[4];
	int priority;
	int cputime;
	int needtime;
	int count;
	int round;
	enum state process;
	struct pcb * next;
};
struct pcb * get_process();    /*进程建立*/
struct pcb * get_process(){
	struct pcb *q;
	struct pcb *t;
	struct pcb *p;
	int i=0;
	fprintf(pFile,"input name and needtime, please input 3 processes\n");
	while (i<P_NUM){
		q=(struct pcb *)malloc(sizeof(struct pcb));
		scanf("%s", &(q->name) );
		scanf( "%d", &(q->needtime));
		q->cputime=0;
		q->priority=P_TIME-q->needtime;
		q->process=ready;
		q->next=NULL;
		if (i==0){
			p=q;
			t=q;
		}
		else{
			t->next=q;
			t=q;
		}
		i++;
	}/*while*/
	return p;
}
