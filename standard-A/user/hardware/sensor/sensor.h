
#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#include "stm32f4xx.h"
#define PI6     GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_6)//for shoot
extern void sensor_Init(void);


#endif
