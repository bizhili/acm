#include "include.h"
#include "shoot_task_new.h"
static const RC_ctrl_t *shoot_rc2;//to get rc control value 
static shoot_ty shoot1;
static const motor_measure_t *trigger_motor;
static int shoot_can_control(void);
int shoot_can;
void SHOOT_task(void *pvParameters)
{
	shoot1.rspeed=0;
  sensor_Init();
  steeringGear(2000, 1800);
  while(1)//init shoot motor
  {
    laser_on();
    static u16 count=0;
    vTaskDelay(SHOOT_CONTROL_TIME2);
    count+=2;
    TIM_SetCompare1(TIM4,500+count);
    if(count>=300)
    {
     vTaskDelay(1000);
     break;
    }
    laser_configuration();
  }
  
	while(1)
	{
    
    
    static u16 shootSpeed=0;
    shoot_rc2=get_remote_control_point();
    if(shoot_rc2->rc.s[0]==2)//bullet top open
    {
      steeringGearMax();
    }
    else
    {
      steeringGearMin();//bullet top close
    }
    
    TIM_SetCompare1(TIM4,1000-shootSpeed);//control shoot motor
    shoot_can=shoot1.rspeed;
    if(shoot_rc2->mouse.press_r!=0||shoot_rc2->rc.ch[3]<-450)//controled by rc and computer
    {
      shootSpeed+=1;
      if(shootSpeed>=420)
        shootSpeed=420;
    }
    else if(shoot_rc2->rc.ch[3]>=420)
    {
      shootSpeed-=1;
      if(shootSpeed<=2)
        shootSpeed=2;
    }
    else
    {
      shootSpeed-=1;
      if(shootSpeed<=420&&shootSpeed>=50)
        shootSpeed=420;
    }
    if(shoot1.shootTimeCount<sleepTime&&shoot1.shootTimeFlag==0)//wait for sleep time
      shoot1.shootTimeCount+=3;
    if(shoot1.shootTimeCount>=sleepTime&&shoot1.shootTimeFlag==0)
    {
      shoot1.shootTimeFlag=1;
      shoot1.shootTimeCount=0;
    }
    if(shoot_rc2->mouse.press_l!=0)
      shoot1.shootFlag=1;
    if(shootSensor==1)
    {
      shoot1.rspeed=shoot_can_control();
      shoot1.shootTimeFlag=0;
      shoot1.shootTimeCount=0;
      shoot1.shootFlag=0;
    }
    else
    {
    if((shoot1.shootFlag==1||shoot_rc2->rc.ch[3]<=-580)&&shoot1.shootTimeFlag==1&&shootSensor==0)
      shoot1.rspeed=shoot_can_control();
    else
      shoot1.rspeed=0;
    }
    
    vTaskDelay(SHOOT_CONTROL_TIME2);
    
	}
}

int shoot_can_control(void)
{
	int h;
	trigger_motor=get_Trigger_Motor_Measure_Point();
	h=(-1500-trigger_motor->speed_rpm)*100;//control trigger motor pid control//-1500
	if(h>9999)//limit the current of motor 
		h=9999;
	if(h<-9999)
		h=-9999;
	return h;
}



