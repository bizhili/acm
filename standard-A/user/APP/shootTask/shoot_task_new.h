#ifndef SHOOT_TASK_NEW_H
#define SHOOT_TASK_NEW_H
#include "main.h"
#define SHOOT_CONTROL_TIME2 3
#define sleepTime 10 //ms
extern int shoot_can;
typedef struct
{
  uint16_t fspeed;
  uint8_t shootReady;
  uint8_t shootFlag;
  uint8_t shootTimeFlag;
  uint16_t shootTimeCount;
  int rspeed;
}shoot_ty;
extern void SHOOT_task(void *pvParameters);
extern int shoot_can;
#endif
