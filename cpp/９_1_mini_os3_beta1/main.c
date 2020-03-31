/* main.c */
#include "main.h"

struct init_pcb *pro1,*pro2,*pro3;

void func1(void)
{
	int i = 1;
	while (i<10) {
		i++;
		printf("thread 1 :%d\n",i);
		os_delay_ms(10);
		printf("xixi\n");
		os_delay_ms(10);
		printf("qiqi\n");
		os_delay_ms(10);
		printf("tata\n");
		os_delay_ms(100);
		printf("gaag\n");
	}
	os_exe_end();
}
void func2(void)
{
	int i = 1;
	while (i<9) {
		i++;
		printf("2thread 2 :%d\n", i);
		os_delay_ms(10);
		printf("2xixi\n");
		os_delay_ms(10);
		printf("2qiqi\n");
		os_delay_ms(10);
		printf("2tata\n");
		os_delay_ms(10);
		printf("2gaag\n");
	}
	os_exe_end();
}

void func3(void)
{
	int i = 1;
	while (i<10) {
		i++;
		printf("3thread 3 :%d\n", i);
		os_delay_ms(10);
		printf("3xixi\n");
		os_delay_ms(10);
		printf("3qiqi\n");
		os_delay_ms(10);
		printf("3tata\n");
		os_delay_ms(10);
		printf("3gaag\n");
	}
	os_exe_end();
}


int main()
{
	
	pro1=my_malloc(sizeof(struct init_pcb));
	pro2=my_malloc(sizeof(struct init_pcb));
	pro3=my_malloc(sizeof(struct init_pcb));
	pro1->p=func1;
	pro1->startTime=18;
	pro1->time_priority=90;
	pro1->stackSpace=8000;
	pro2->p=func2;
	pro2->startTime=13;
	pro2->time_priority=100;
	pro2->stackSpace=8000;
	pro3->p=func3;
	pro3->startTime=13;
	pro3->time_priority=100;
	pro3->stackSpace=8000;
	os_init_process(pro1);
	os_init_process(pro2);
	os_init_process(pro3);
	os_start();
	
}

/*

stack space
processing function
start time
time priority

*/

