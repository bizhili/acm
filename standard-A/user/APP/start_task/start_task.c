/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       start_task.c/h
  * @brief      �������񣬽�һ������������������Դ�������������ȼ�,
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "include.h"
#include "start_task.h"
static commonTime commontime1;
void uartTask(void *pvParameters);//ertern some functions
void timeTask(void *pvParameters);

#define INS_TASK_PRIO 20
#define INS_TASK_SIZE 512
static TaskHandle_t INSTask_Handler;

#define GIMBAL_TASK_PRIO 19
#define GIMBAL_STK_SIZE 768
TaskHandle_t GIMBALTask_Handler;

#define Chassis_TASK_PRIO 18
#define Chassis_STK_SIZE 512
TaskHandle_t ChassisTask_Handler;

#define User_TASK_PRIO 4
#define User_STK_SIZE 256
static TaskHandle_t UserTask_Handler;

#define START_TASK_PRIO 1
#define START_STK_SIZE 512
static TaskHandle_t StartTask_Handler;

#define Shoot_TASK_PRIO 9
#define Shoot_TASK_SIZE 256
static TaskHandle_t Shoot_Handler;

#define CALIBRATE_TASK_PRIO 5
#define CALIBRATE_STK_SIZE 512
static TaskHandle_t CalibrateTask_Handler;

#define Detect_TASK_PRIO 10
#define Detect_STK_SIZE 512
static TaskHandle_t DetectTask_Handler;

#define uart_TASK_PRIO 2
#define uart_STK_SIZE 512
static TaskHandle_t uartTaskHandler;

#define timeTaskPrio 6
#define timeStkSize 64
static TaskHandle_t timeTaskHandler;

#define judgeTaskPrio 10
#define judgeStkSize 256
static TaskHandle_t judgeTaskHandler;

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)INSTask,
                (const char *)"INSTask",
                (uint16_t)INS_TASK_SIZE,
                (void *)NULL,
                (UBaseType_t)INS_TASK_PRIO,
                (TaskHandle_t *)&INSTask_Handler);
          
    xTaskCreate((TaskFunction_t)GIMBAL_task,
                (const char *)"GIMBAL_task",
                (uint16_t)GIMBAL_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)GIMBAL_TASK_PRIO,
                (TaskHandle_t *)&GIMBALTask_Handler);

    xTaskCreate((TaskFunction_t)chassis_task,
                (const char *)"ChassisTask",
                (uint16_t)Chassis_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Chassis_TASK_PRIO,
                (TaskHandle_t *)&ChassisTask_Handler);

    xTaskCreate((TaskFunction_t)UserTask,
                (const char *)"UserTask",
                (uint16_t)User_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)User_TASK_PRIO,
                (TaskHandle_t *)&UserTask_Handler);

    xTaskCreate((TaskFunction_t)calibrate_task,
                (const char *)"CaliTask",
                (uint16_t)CALIBRATE_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)CALIBRATE_TASK_PRIO,
                (TaskHandle_t *)&CalibrateTask_Handler);

    xTaskCreate((TaskFunction_t)DetectTask,
                (const char *)"DetectTask",
                (uint16_t)Detect_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Detect_TASK_PRIO,
                (TaskHandle_t *)&DetectTask_Handler);
								
	xTaskCreate((TaskFunction_t)SHOOT_task,
                (const char *)"SHOOTTask",
                (uint16_t)Shoot_TASK_SIZE,
                (void *)NULL,
                (UBaseType_t)Shoot_TASK_PRIO,
                (TaskHandle_t *)&Shoot_Handler);
     
    xTaskCreate((TaskFunction_t)uartTask,
                (const char *)"uartTask",
                (uint16_t)uart_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)uart_TASK_PRIO,
                (TaskHandle_t *)&uartTaskHandler);
                
    xTaskCreate((TaskFunction_t)timeTask,
                (const char *)"DetectTask",
                (uint16_t)timeStkSize,
                (void *)NULL,
                (UBaseType_t)timeTaskPrio,
                (TaskHandle_t *)&timeTaskHandler);
                
    xTaskCreate((TaskFunction_t)judgeTask,
                (const char *)"judgeTask",
                (uint16_t)judgeStkSize,
                (void *)NULL,
                (UBaseType_t)judgeTaskPrio,
                (TaskHandle_t *)&judgeTaskHandler);
                

    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void timeTask(void *pvParameters)//generate common time 
{
  while(1)
  {    
    commontime1.doubleMicroSecond++;
    commontime1.longTime++;
    if(commontime1.doubleMicroSecond==500)
    {
      commontime1.second++;
      commontime1.doubleMicroSecond=0;
      if(commontime1.second==60)
      {
        commontime1.minute++;
        commontime1.second=0;
        if(commontime1.minute==60)
        {
          commontime1.hour++;
          commontime1.minute=0;
        }
      }
    }
    vTaskDelay(timeDelayTime);
  }
}
const commonTime *getCommonTime(void)
{
    return &commontime1;
}

void startTast(void)
{
								buzzer_off();
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}




