/* uthread.c */
#include "main.h"

struct context *next=0;
int count=0,flag1=0,flag2=0,flag11=0,flag12=0,flag21=0;
void schedule();
void schedule(void)
{
	
	struct context *prev=0;	
	printf("count %d\n",count);
		if(flag11==0)
		{
			flag11=1;
			save(ctx1);
			printf("a\n");
		}
		if(flag21==1)
		{printf("b\n");
			flag11=0;
			flag21=0;
			restore(ctx1);
			
			
		}
		if(flag11)
			flag21=1;
		
	printf("okk\n");
}


void func1()
{
	int i = 1;
	if(!flag1)
		save(ctx1);
	if(flag1)
	{
		while (i++) {
		printf("thread 1 :%d\n", i);
		sleep(1);
		printf("xixi\n");
		sleep(1);
		printf("qiqi");
		sleep(2);
		printf("tata");
		sleep(2);
		printf("gaag");
	}
	}
	flag1=1;
	
}

void func2()
{
	int i = 0;
	if(!flag2)
		save(ctx2);
	if(flag2)
	{
		while (i++) {
		printf("thread 2 :%d\n", i);
		sleep(1);
	}
	}
	flag2=1;

}

void init_sigaction(void)
{
struct sigaction tact;
/*信号到了要执行的任务处理函数为prompt_info*/
tact.sa_handler = schedule;
tact.sa_flags = SA_NODEFER;
/*初始化信号集*/
sigemptyset(&tact.sa_mask);
/*建立信号处理机制*/
sigaction(SIGALRM, &tact, NULL);

}
void init_time()
{
    struct itimerval value;
    /*设定执行任务的时间间隔为2秒0微秒*/
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
    setitimer(ITIMER_REAL, &value, NULL);
}

int main(int unused1, char **unused2)
{
	int i, j, k;

	ctx1 = (struct context *)malloc(sizeof(struct context));
	ctx2 = (struct context *)malloc(sizeof(struct context));

	stack1 = (unsigned char *)malloc(4096);
	stack2 = (unsigned char *)malloc(4096);

	memset(ctx1, 0, sizeof(struct context));
	memset(ctx2, 0, sizeof(struct context));

	// 以下的4行是关键，用glibc的setjmp/longjmp很难做到！
	ctx1->rip = &func1;
	// 因为stack是向下生长的，所以要从高地址开始！这点很容易出错。
	ctx1->rsp = ctx1->rbp = stack1+4000;

	ctx2->rip = &func2;
	ctx2->rsp = ctx2->rbp = stack2+4000;

	init_sigaction();

	init_time();
	//i =save(ctx1);
	//j =save(ctx2);
	// 切换到thread1的func1，内部切换了stack，并且由于修改了ctx的rip，即修改了save的返回地址，将直接进入func1的逻辑！
	
	func1();
	func2();
	func1();
	

	return 0;
}
/*
as --64 -o os_core.o os_core.s
gcc -c main.c -o main.o
gcc -o a.out os_core.o main.o
 ./a.out
*/

