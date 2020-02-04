#include "sensor.h"
#include "include.h"
void sensor_Init(void)//pc2,4,0,5;pb1
{
GPIO_InitTypeDef GPIO_Struct1;	
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
GPIO_Struct1.GPIO_Pin=GPIO_Pin_6;
GPIO_Struct1.GPIO_Mode=GPIO_Mode_IN;
GPIO_Struct1.GPIO_Speed=GPIO_Speed_100MHz;
GPIO_Struct1.GPIO_PuPd=GPIO_PuPd_UP;	
GPIO_Init(GPIOI,&GPIO_Struct1);
GPIO_ResetBits(GPIOI,GPIO_Pin_6);
  
}

