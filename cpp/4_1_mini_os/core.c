#include "core.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h> 
#include <time.h>

struct pcb *p,*thisP;//header
u8 os_end_flag=0,os_priority=1;
void os_process(int signo)
{
	static int lastTime=0,thisTime=0;
	struct pcb *temp;
	float maxPriority=0;
	thisTime=(int)(clock()/1000);
	u16 os_count=0;
	if(os_priority==1||os_priority==2)//timer mode or function mode
	{
		if(os_priority==2)
			os_priority=3;
		temp=p;
		while (temp)
		{
			if(temp->process==future)
			{
				if(clock()>=temp->startTime*1000)
				{
					temp->process=ready;
					temp->priority=1;
					fprintf(pFile, "Process %s is booting\n",temp->name);
				}
			}
			else if(temp->process==ready||temp->process==execute)
			{
				if(temp->process==ready)
					temp->waitTime+=lastTime-thisTime;
				temp->priority=1+(float)temp->waitTime/temp->needtime;
				if (maxPriority>temp->priority)
					thisP=temp;
			}
			if(temp->process==finish)
				os_count++;
			if (os_count==P_NUM)
			{
				os_end_flag=1;
				fprintf(pFile, "os finished");
				printf("\n bye ");
				exit(0);
			}
			temp=temp->next;
		}
		thisP->process==execute;
		display(p);
	}
	else 
		return;
	
	lastTime=(int)(clock()/1000);
}

// 建立信号处理机制
void init_sigaction(void)
{
	struct sigaction tact;
	/*信号到了要执行的任务处理函数为prompt_info*/
	tact.sa_handler = os_process;
	tact.sa_flags = 0;
	/*初始化信号集*/
	sigemptyset(&tact.sa_mask);
	/*建立信号处理机制*/
	sigaction(SIGALRM, &tact, NULL);

}

void init_time()
{
    struct itimerval value;
    /*设定执行任务的时间间隔为2秒0微秒*/
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 20;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
	
    setitimer(ITIMER_REAL, &value, NULL);
	exit(0);
	
}

bool_t os_delay_ms(u32 delay_time)
{
    clock_t time1;
    u32 delay_clock=delay_time*1000;
    time1=clock();
    
    while(clock()-time1<delay_clock);

    return 1;
}

void process_exe(struct pcb *q)
{
	if (q->leastTime)
	{
		os_delay_ms(1);
		q->leastTime--;
		q->cputime++;
	}
	else
	{
		q->endTime=(int)(clock()/1000);
		q->priority=0;
		q->process=finish;
	}
}

int process_finish(struct pcb *q){         /*进程是否结束，当还有进程没有结束时返回0*/
	int bl=1;
	while(bl&&q){
		bl=bl&&q->needtime==0;
		q=q->next;
	}
}

void priority_init(){     /*优先数调度*/
	int cpu =0;
	
	p=init_process();//create process
	
	init_sigaction();
	
	init_time();
	
	while (! os_end_flag)
	{
		if(thisP->process==execute)
			process_exe(thisP);
		else
		{
			os_priority=2;
			os_process(1);
		}
			
	}
	exit(0);

	fprintf(pFile,"All processes have finished,press any key to exit");
}
struct pcb * get_process_round(){       /*轮转进程建立*/
	struct pcb *q;//用于修改当前值
	struct pcb *t;//用于链表中间替换
	struct pcb *p;//用于保存链尾
	int i=0;
	fprintf(pFile,"input name and time,please input 3 processes\n");

	while (i<P_NUM){
		q=(struct pcb *)my_malloc(sizeof(struct pcb));
		scanf( "%s", &(q->name));
		scanf( "%d", &(q->needtime));
		q->cputime=0;
		q->round=0;
		q->count=0;
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
	}  /*while*/
	return p;
}

void cpu_round(struct pcb *q){   /*处理机轮转*/
	q->cputime+=2;
	q->needtime-=2;
	if(q->needtime<0) {
		q->needtime=0;
	}
	q->count++;
	q->round++;
	q->process=execute;

}

struct pcb * get_next(struct pcb * k, struct pcb * head){    /*返回没有结束的进程*/
	struct pcb * t;
	t=k;
	do{
	 t=t->next;
	}while (t && t->process==finish);
	if(t==NULL){
		t=head;
		while (t->next!=k && t->process==finish){
			t=t->next;
		}
	}
	return t;
}

void set_state(struct pcb *p){
	while(p){
		if (p->needtime==0){
			p->process=finish;

		}
		if (p->process==execute){
			p->process=ready;
		}
		p=p->next;
	}
}


void round_cal(){         /*循环轮转调度*/
	struct pcb * p;
	struct pcb * r;
	int cpu=0;
	p=get_process_round();
	r=p;
	while(!process_finish(p)){
		cpu+=2;
		cpu_round(r);
		r=get_next(r,p);
		fprintf(pFile,"cpu %d\n", cpu );
		display_round(p);
		set_state(p);
		os_delay_ms(5);
	}
fprintf(pFile,"\nAll processes have finished,press any key to exit");
}