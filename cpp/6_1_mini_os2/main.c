#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "cv_stdlib.h"
int count1=0,count=0,count2=0;
FILE *pFile=0;
struct pcb *p=0,*thisP=0;//header
u8 os_end_flag=0,os_priority=1;
struct pcb * init_process(){
	struct pcb *q;
	struct pcb *t;
	struct pcb *p;
	int i=0;
	fprintf(pFile,"input name and needtime, please input %d processes,set of time: *ms\n",P_NUM);
	count=0;
	while (i<P_NUM){
		q=(struct pcb *)malloc(sizeof(struct pcb));
		scanf("%s", &(q->name) );
		scanf("%d", &(q->startTime) );
		scanf("%d", &(q->needtime));
		q->ID=count;
		count++;
		q->cputime=0;
		q->priority=0.0f;
		q->process=future;
		q->waitTime=0;
		q->leastTime=q->needtime;
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
	return p;//head process
}
void display_menu(){
	fprintf(pFile,"CHOOSE THE ALGORITHM:\n" );
	fprintf(pFile,"1 PRIORITY\n" );
	fprintf(pFile,"2 ROUNDROBIN\n" );
	fprintf(pFile,"3 EXIT\n" );
}
void  display(struct pcb *p){
	fprintf(pFile,"name    cputime    leasttime    priority     waittime        state    		\n");
	if(p){
		fprintf(pFile,"%s", p->name);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->cputime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->leastTime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%f", p->priority);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->waitTime);
		fprintf(pFile,"        ");
		switch(p->process){
			case ready:
				fprintf(pFile,"ready\n");
				break;
			case execute:
				fprintf(pFile,"execute\n");
				break;
			case block:
				fprintf(pFile,"block\n");
				break;
			case finish:
				fprintf(pFile,"finish\n");
				break;
		
		}
	}
}
void  display_all(struct pcb *p){
	fprintf(pFile,"name    cputime    needTime    priority	  waittime    startTime    endTime     power		state\n");
	if(p){
		fprintf(pFile,"%s", p->name);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->cputime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->needtime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%f", p->priority);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->waitTime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->startTime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->endTime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%f", p->power);
		fprintf(pFile,"        ");
		switch(p->process){
			case ready:
				fprintf(pFile,"ready\n");
				break;
			case execute:
				fprintf(pFile,"execute\n");
				break;
			case block:
				fprintf(pFile,"block\n");
				break;
			case finish:
				fprintf(pFile,"finish\n");
				break;
		}
	}
}
void  display_special(struct pcb *p){
	
	if(p){
		u16 temp=p->ID;
		for(;temp>0;temp--)
			fprintf(pFile,"\t");
		fprintf(pFile,"|\n");

	}
}
void os_process(int signo)
{
	static int lastTime=0,thisTime=0;
	struct pcb *temp,*temp2;
	float maxPriority=0.0;
	thisTime=(int)(clock()/1000);
	u16 os_count=0;
	temp2=thisP;
	thisP=0;
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
					fprintf(pFile, "\nProcess %s is booting\n",temp->name);
				}
			}
			else if(temp->process==ready||temp->process==execute)
			{
				if(temp->process==ready)
					temp->waitTime+=thisTime-lastTime;
				temp->priority=(1+(float)temp->waitTime/temp->needtime);
				if (temp->priority>maxPriority)
				{
					thisP=temp;
					maxPriority=temp->priority;
					//fprintf(pFile, "\nhaha\n");
				}
			}
			if(temp->process==finish)
				os_count++;
			if (os_count==P_NUM)
			{
				os_end_flag=1;
				fprintf(pFile, "os finished\n");
				printf("\nbye");
				return;
			}
			temp=temp->next;
		}
		if(temp2)
				temp2->process=ready;
		if (thisP)
			thisP->process=execute;
		
		os_priority=1;
		display_special(thisP);
	}
	else 
		return;
	lastTime=thisTime;
}
// 建立信号处理机制
void init_sigaction(void)
{
    struct sigaction tact;//
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
    value.it_value.tv_usec = 1000;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
    setitimer(ITIMER_REAL, &value, NULL);

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
		//q->priority=0;
		q->process=finish;
		q->power=(float)q->waitTime/q->needtime+1;
	}
}
void priority_init(){     /*优先数调度*/
	p=init_process();//create process
	init_sigaction();
	init_time();
	while (! os_end_flag)
	{
        if(thisP)
			    process_exe(thisP);
		else
		{
			os_priority=2;
			os_process(1);
			os_delay_ms(2);
		}	
	}
	while (p)
	{
		display_all(p);
		p=p->next;
	}
	
	fprintf(pFile,"All processes have finished,press any key to exit");
}
int main(){// 没有free
    int user_input;
    freopen("input.txt","r",stdin);//input file
    pFile = fopen ("output.txt","w");//output file
	display_menu();
	scanf("%d",&user_input);
	switch(user_input){
			case 1:priority_init();break;
			case 3:break;
			default:
			display_menu();
			scanf("%d",&user_input);
			break;
	}
	fclose (pFile);
	free_all();
	exit(0);
}
