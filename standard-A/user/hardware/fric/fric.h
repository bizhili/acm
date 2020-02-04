#ifndef FRIC_H
#define FRIC_H
#include "main.h"
#include "stm32f4xx.h"
#define Fric_UP 1400
#define Fric_DOWN 1300
#define Fric_OFF 1000
extern  void TIM4_PWM_Init(uint32_t arr,uint32_t psc,u32 date);//第一个pwm输出PD12//H
extern void fric_PWM_configuration(u16 arr,u16 psc);
extern void fric_off(void);
extern void fric1_on(uint16_t cmd);
extern void fric2_on(uint16_t cmd);
extern void steeringGear(uint16_t arr, uint16_t psc);//PI5,W
extern void steeringGearMax(void);
extern void steeringGearMin(void);
#endif
