#ifndef UARTTASK_H
#define UARTTASK_H
#include "main.h"
#include "sys.h"
#include "stm32f4xx.h"
#define uartDelayTime 2
#define maxPower 32000
union u16Tofloat 
{ 
  u8 u8data[4];
  float fdata;
}; 

union u16Tou8
{ 
  u8 u8data[2];
  u16 u16data;
}; 
typedef __packed struct//struct for get messege
{
  u8 tx2Statu;//0x01:sleep,0x02:normal
  u8 fps;
  u8 time;//used to ensure delay time
  u16 distance;//unit:cm
  float pitchAngle;//unit:degree
  float yawAngle;
}tx2Received;

typedef __packed struct//struct for send messege
{
  u8 control;//0x11:control to sleep,0x12:control to normal
  u8 roboID;//?
  u8 time;//used to ensure delay time
  float yawMotorSpeed;//unit:degrees pre second
  float yawAbsoluteSpeed;//unit:degrees pre second
}sendToTx2;

typedef __packed struct
{
  tx2Received receive;
  sendToTx2 send;
}messegeTx2;

typedef __packed struct//struct for connect with tx2
{
  __packed struct
  {
          u8 sta;
          u8 flag;
          __packed struct
        {
          double x;
          double y;
        } axis;              
  }received;
  __packed struct
  {
    u8 sta;
  }send;
  u16 power;
}messege;

typedef __packed struct//struct for get messege
{
  u8 res2;
  u8 res6;
}getss;
extern void uartTask(void *pvParameters);
extern const getss *get_res(void);
extern messege *getReceviedU6(void);
  
#endif