#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
static char msg[] = "time is running out\n";
static int len;
int count1=0,count=0,count2=0;
struct sigaction tact,oldact;//
void prompt_info2(int signo);
int os_delay_ms(int delay_time)
{
    clock_t time1;
    int delay_clock=delay_time*1000;
    time1=clock();
    while(clock()-time1<delay_clock);
    return 1;
}
// 向标准错误输出信息，告诉用户时间到了
void prompt_info(int signo)
{
    tact.sa_handler = prompt_info2;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, &oldact);
    count2++;
    count1++;
    os_delay_ms(400);
    printf("2");
    count1+=2;
    os_delay_ms(400);
    printf("2");
    count1+=3;
    os_delay_ms(400);

    tact.sa_handler = prompt_info2;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, &oldact);

    printf("2");
    count1+=3;
    os_delay_ms(400);
    printf("2");
    count1+=3;
    os_delay_ms(400);
    printf("2");
    count1+=3;
    os_delay_ms(1400);
    count1-=15;
    printf("\nf%d,%d,%d",count1,count,count2);

}

void prompt_info2(int signo)
{
    tact.sa_handler = prompt_info;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
    count2++;
    count1++;
    os_delay_ms(400);
    printf("1");
    count1+=2;
    os_delay_ms(400);
    printf("1");
    count1+=3;
    os_delay_ms(400);
    printf("1");
    count1+=3;

    tact.sa_handler = prompt_info;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);

    os_delay_ms(400);
    printf("1");
    count1+=3;
    os_delay_ms(400);
    printf("1");
    count1+=3;
    os_delay_ms(1400);
    count1-=15;
    printf("\na%d,%d,%d",count1,count,count2);
}


// 建立信号处理机制
void init_sigaction(void)
{

/*信号到了要执行的任务处理函数为prompt_info*/
tact.sa_handler = prompt_info;
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
    value.it_value.tv_usec = 100;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
    setitimer(ITIMER_REAL, &value, NULL);
}
int main()
{
len = strlen(msg);
init_sigaction();
init_time();
while (1){
    count++;
    os_delay_ms(400);
    count+=2;
    os_delay_ms(400);
    count+=3;
    os_delay_ms(400);
    count+=3;
    os_delay_ms(400);
    count+=3;
    os_delay_ms(400);
    count+=3;
    os_delay_ms(400);
    count+=3;
    os_delay_ms(400);
    count-=18;
    printf("\nb%d",count);
    os_delay_ms(10);
}
}
