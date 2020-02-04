/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       start_task.c/h
  * @brief      启动任务，将一个个任务开启，分配资源，给定任务优先级,
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef START_TASK_H
#define START_TASK_H
#include "main.h"
#include "stm32f4xx.h"

#define timeDelayTime 2
void startTast(void);
typedef __packed struct//caution every 39 hours, longtime will reset once
{
  u32 longTime;
  u16 doubleMicroSecond;
  u8 second;
  u8 minute;
  u8 hour; 
}commonTime;
const commonTime *getCommonTime(void);
#endif
