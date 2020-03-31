//利用系统55ms中断，显示时间
//作者：nuaazdh
//时间：2011年11月30日
#include <stdio.h>
#include <bios.h>
#include <ctype.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

unsigned long count=0;//每55ms中断，count+1
int hour=0,minute=0,second=0,millisecond=0;
char time[10];
struct time t;

void interrupt get();
void interrupt (*old)();

//中断程序
void interrupt get()
{
	count+=1;
}

//获取时间
void Clock()
{

	millisecond=(count*55)%1000;
	second=(count*55/1000)%60;
	minute=(count*55/60000%60)%60;
	hour=(count*55/3600000%24)%24;

}

//显示时间
void PrintClock()
{
	clrscr();
	printf("\n\n\n\n");
	printf("\t\tThe beginning time:  ");
	printf("%02d:%02d:%02d\n\n",t.ti_hour,t.ti_min,t.ti_sec);
	printf("\t\tThe current time is: \n\n");
	printf("\t\tInterrupt counts:   \n\n");
	printf("\t\tPress esc to exit!");

}

//更新显示
void UpdateClock()
{
	gotoxy(38,7);//定位屏幕指定位置
	printf("%02d:%02d:%02d\n\n",hour,minute,second);//更新显示
	gotoxy(37,9);//定位中断数目位置
	printf("%06d",count);
}

//获取用户输入，检测Esc退出
void End()
{
	long key;

	while(bioskey(1)!=0)
	{
		key=bioskey(0);
		key=key&0x00FF;
		if(key==27)
		{
			setvect(0x1C,old);
			exit(0);
		}
	}
}

void main()
{
	old=getvect(0x1C);//得到中断向量
	setvect(0x1C,get);//设置中断向量
	gettime(&t);//获取时间
	PrintClock();//显示时间
	while(1)
	{
		Clock();
		UpdateClock();//更新显示
		End();
	}

}
