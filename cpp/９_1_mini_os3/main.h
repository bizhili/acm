#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "cv_stdlib.h"
#include <unistd.h>
#include "core_head.h"
//os declare
struct pcb *os_head=0,*os_init_temp=0,*os_now_processing=0;
u16 os_init_count=0,os_first_flag=0,os_end_count=0,os_sleep_count=0;
u8 os_end_flag=0;
void os_schedule(int a);

int os_delay_ms_old(int delay_time)
{
    clock_t time1;
    int delay_clock=delay_time*1000;
    time1=clock();
	
    while(clock()-time1<delay_clock);
    return 1;
}

int os_delay_ms(int delay_time)//need signal value
{
    os_now_processing->sleepTime=delay_time;
	os_now_processing->process_state=block;
	os_sleep_count++;
	os_schedule(1);
    return 1;
}

int os_init_process(struct init_pcb *p)
{
	struct pcb *temp;
	unsigned long *prip1;
	unsigned long *pst1,*pbp1;
	temp=(struct pcb *)my_malloc(sizeof(struct pcb));
	if(!os_init_count)
		os_head=temp;
	else
		os_init_temp->next=temp;
	os_init_temp=temp;
	temp->startTime=p->startTime;
	temp->priority=0.0f;
	temp->timePriority=p->time_priority;
	temp->stack=(unsigned char *)my_malloc(p->stackSpace+96);
	temp->cputime=0;
	temp->waitTime=0;
	temp->sleepTime=0;
	temp->process_state=future;
	memset(&temp->buf, 0, sizeof(jmp_buf));
	prip1 = ((unsigned long *)&temp->buf) + JB_PC;
	pbp1 = ((unsigned long *)&temp->buf) + JB_RBP;
	pst1 = ((unsigned long *)&temp->buf) + JB_RSP;
	// init process pointer
	*prip1 = PTR_MANGLE(p->p);
	*pst1 = *pbp1 = PTR_MANGLE(temp->stack+p->stackSpace);
	temp->p=p->p;
	temp->pid=os_init_count;
	os_init_count++;
	return 1;
}


void os_schedule(int a)
{
	struct pcb *temp=os_head,*lastTemp=0;
	float maxPriority=0.0f;
	u16 lastPid=-1;
	if(os_now_processing)
	{		
		lastTemp=os_now_processing;
		lastPid=os_now_processing->pid;
	}
	while (temp)
	{
		if(temp->process_state==future)//detect boot time
		{
			if(temp->startTime)
				temp->startTime--;
			else
			{
				temp->process_state=ready;
				temp->priority=1;
				if(maxPriority<1.0f)
					{
						os_now_processing=temp;
						maxPriority=temp->priority;
						os_first_flag++;
						if(os_first_flag==1)//first process
						{
							os_first_flag++;
							longjmp(os_now_processing->buf,1);
						}
					}
			}
		}
		else if(temp->process_state==ready)
		{
			temp->waitTime++;
			temp->priority=((float)temp->waitTime/temp->timePriority+1);
			if(maxPriority<temp->priority)
			{
				os_now_processing=temp;
				maxPriority=temp->priority;
			}
		}
		else if(temp->process_state==execute)
		{
			temp->cputime++;
			if(maxPriority<temp->priority)
			{
				os_now_processing=temp;
				maxPriority=temp->priority;
			}
			temp->process_state=ready;
		}
		else if(temp->process_state=block)//only sleep mode
		{
			if(!temp->sleepTime--)//sleep finished
			{
				temp->process_state=ready;
				if(maxPriority<temp->priority)
				{
					os_now_processing=temp;
					maxPriority=temp->priority;
					os_sleep_count--;
				}
			}
		}
		temp=temp->next;
	}
	if(!os_now_processing)//not any process running
	{
		return;
	}
	os_now_processing->process_state=execute;//change statu
	if (os_now_processing->pid==lastPid)//if last processing still max priority
		return;
	if(setjmp(lastTemp->buf))//save last process
	{
		return;	
	}
	else
	{
		if(os_sleep_count==os_init_count)//all process sleep
		{
			while (1);
		}
		else
		longjmp(os_now_processing->buf,1);
	}
		
}
void os_fprint_report()
{

}
void os_exe_end(void)//once a process end
{
	printf("end\n");
	os_end_flag=1;
	os_end_count++;//sign end process num
	if(os_end_count==os_init_count)//end process num ==setup process num
	{
		printf("bye\n");
		free_all();//free all my_malloc
		exit(0);
	}
	os_now_processing->process_state=finish;
	os_now_processing->endTime=(u32)clock()/1000;
	os_now_processing->priority=0;
	os_schedule(0);
	while (1);
}




void os_init_sigaction(void)//init signal
{
struct sigaction tact;
/*信号到了要执行的任务处理函数为prompt_info*/
tact.sa_handler =os_schedule;
tact.sa_flags = SA_NODEFER;
/*初始化信号集*/
sigemptyset(&tact.sa_mask);
/*建立信号处理机制*/
sigaction(SIGALRM, &tact, NULL);
}

void os_init_time()//init timer
{
    struct itimerval value;
    /*设定执行任务的时间间隔为2秒0微秒*/
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1*1000;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
    setitimer(ITIMER_REAL, &value, NULL);
}


int os_start()
{
	os_init_sigaction();
	os_init_time();
	while (1);
}


