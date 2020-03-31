#include "main.h"

/* 主程序 */

int main(){// 没有free
	printf("1");
    freopen("input.txt","r",stdin);//input file
    pFile = fopen ("output.txt","w");//output file
	
	int user_input;
	display_menu();
	scanf("%d",&user_input);
	
	switch(user_input){
			case 1:priority_init();break;
			case 2:round_cal();break;
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
