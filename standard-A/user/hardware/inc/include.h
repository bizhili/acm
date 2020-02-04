#ifndef INCLUDE_H
#define INCLUDE_H

//arm
#include "arm_math.h"
#include "main.h"
#include "stm32f4xx.h"
#include "AHRS.h"
//standard libray
#include  "sys.h"
#include  "stdio.h"
#include  "string.h"
#include  "ctype.h"
#include  "stdlib.h"
#include  "stdarg.h"
#include  "math.h"
//hardware
#include "adc.h"
#include "buzzer.h"
#include "can.h"
#include "delay.h"
#include "exit_init.h"
#include "flash.h"
#include "fric.h"
#include "laser.h"
#include "led.h"
#include "power_ctrl.h"
#include "rc.h"
#include "rng.h"
#include "spi.h"
#include "sys.h"
#include "timer.h"
#include "sensor.h"
#include "usart2.h"
#include "usart6_dma.h"
//os
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

//app
#include "calibrate_task.h"
#include "CAN_receive.h"
#include "chassis_task.h"
#include "chassis_behaviour.h"
#include "detect_task.h"
#include "FreeRTOS_middleware.h"
#include "gimbal_behaviour.h"
#include "gimbal_task.h"
#include "INS_task.h"
#include "pid.h"
#include "remote_control.h"
#include "shoot.h"
#include "start_task.h"
#include "user_task.h"
#include "shoot_task_new.h"
#include "gimbal_task_new.h"
#include "uartTask.h"
#include "gimbal_task_new.h"
#include "judge.h"
#endif


