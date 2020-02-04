/**
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  * @file       uartTask.c/h
  * @brief      ����2�봮��8���񣬴���2������λ�����ԣ�֧��printf����������8������TX2ͨ�ţ�CRC16���飬
  *             DMA��ʼ����ͨ��Э����д
  * @note       head+data+tail��
                head=0XAA,0X55
                data
                tail:CRC16 data
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020/1/9        BIZHI              ������
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  */
#include "uartTask.h"
#include "include.h"
#include "DJ_CRC.h"
#include "stdarg.h"
/**************************************
@����            ��stm32����ת��Ϊ��������
@���ù�ϵ        ����Append_CRC16_Check_Sum(SendBuffer,15)����CRC16У����
@�������              ��
@����ֵ                ��
@˵��                  ��
***************************************/
void transferToBuffer(void);
/**************************************
@����            ����������ת��Ϊstm32����
@���ù�ϵ           
@�������               ��
@����ֵ                ��
@˵��            
***************************************/
void transferToMydata(void);
static const motor_measure_t *gimbal_yaw_motor,*gimbal_pitch_motor,*gimbal1,*gimbal2,*gimbal3,*gimbal4,*motor5;
static const RC_ctrl_t *rc_ctrl;
static messege mes1;
messegeTx2 TX2messege;
union u16Tou8 u162u8;
union u16Tofloat u162float;

static const fp32 pidTest[]={20,0,0};
static PidTypeDef pidForTest;

/**************************************
@����            ������printfu2
@���ù�ϵ           
@�������         eg:("aaa:%d",2)
@����ֵ                ��
@˵��            ���η��Ͳ�����40���ֽ�
***************************************/
void printfu2(char *fmt,...)
{
  va_list arg;
  va_start(arg,fmt);
  vsprintf((char*)SendBuffer2,fmt,arg);
  va_end(arg);
  Dma_Send_Enable2(DMA1_Stream6,strlen((const char*)SendBuffer2));
 
}



static getss get1;
void uartTask(void *pvParameters)
{
  Init_Usart6(115200);
  Init_Usart2(115200);
  
  PID_Init(&pidForTest,PID_POSITION, pidTest,2000,1000);
  
  Init_Dma_Tx_Config(USART6, (u32)(&USART6->DR), DMA2_Stream7, DMA_Channel_5, (u32)SendBuffer, BufferLen, DMA2_Stream7_IRQn);
  //DMA ������������ʼ��
  Init_Dma_Rx_Config(USART6, (u32)(&USART6->DR), DMA2_Stream1, DMA_Channel_5, (u32)ReceiveBuffer, BufferLen);
  
  
  Init_Dma_Tx_Config2(USART2, (u32)(&USART2->DR), DMA1_Stream6, DMA_Channel_4, (u32)SendBuffer2, BufferLen, DMA1_Stream6_IRQn);
  //DMA ������������ʼ��
  Init_Dma_Rx_Config2(USART2, (u32)(&USART2->DR), DMA1_Stream5, DMA_Channel_4, (u32)ReceiveBuffer2, BufferLen);
  
  rc_ctrl=get_remote_control_point();
  while(1)
  {
    //���յ�һ֡����
    int control_test=0;

    led_green_toggle();
    control_test=-PID_Calc(&pidForTest,rc_ctrl->rc.ch[0]*2,motor5->speed_rpm/10);
    //CAN_CMD_CHASSIS2(control_test,0,0,0);
    
    
    
    TX2messege.send.time=TX2messege.receive.time;
    static u16 power=0;
    gimbal1=get_Chassis_Motor_Measure_Point(1);
    gimbal2=get_Chassis_Motor_Measure_Point(2);
    gimbal3=get_Chassis_Motor_Measure_Point(3);
    gimbal4=get_Chassis_Motor_Measure_Point(4);
    motor5=get_Chassis_Motor_Measure_Point(5);
    
    power=fabs(gimbal1->given_current)+fabs(gimbal2->given_current)+fabs(gimbal3->given_current)+fabs(gimbal4->given_current);
    mes1.power=power;
    gimbal_pitch_motor=get_Pitch_Gimbal_Motor_Measure_Point(); 
    gimbal_yaw_motor=get_Yaw_Gimbal_Motor_Measure_Point();
    vTaskDelay(uartDelayTime*2);
  }
}

void transferToMydata(void)
{
  TX2messege.receive.tx2Statu=ReceiveBuffer[2];
  TX2messege.receive.fps=ReceiveBuffer[3];
  TX2messege.receive.time=ReceiveBuffer[4];
  u162u8.u8data[0]=ReceiveBuffer[5];
  u162u8.u8data[1]=ReceiveBuffer[6];
  TX2messege.receive.distance=u162u8.u16data;
  u162float.u8data[0]=ReceiveBuffer[7];
  u162float.u8data[0]=ReceiveBuffer[8];
  u162float.u8data[0]=ReceiveBuffer[9];
  u162float.u8data[0]=ReceiveBuffer[10];
  TX2messege.receive.pitchAngle=u162float.fdata;
  u162float.u8data[0]=ReceiveBuffer[11];
  u162float.u8data[0]=ReceiveBuffer[12];
  u162float.u8data[0]=ReceiveBuffer[13];
  u162float.u8data[0]=ReceiveBuffer[14];
  TX2messege.receive.pitchAngle=u162float.fdata;
}
void transferToBuffer(void)
{
  //head bytes
  SendBuffer[0]=0XAA;
  SendBuffer[1]=0X55;
  //control bytes
  SendBuffer[2]=TX2messege.send.control;
  //ID bytes
  SendBuffer[3]=TX2messege.send.roboID;
  //time bytes
  SendBuffer[4]=TX2messege.send.time;
  //motorspeed bytes
  u162float.fdata=TX2messege.send.yawMotorSpeed;
  SendBuffer[5]=u162float.u8data[0];
  SendBuffer[6]=u162float.u8data[1];
  SendBuffer[7]=u162float.u8data[2];
  SendBuffer[8]=u162float.u8data[3];
  //absolutespeed bytes
  u162float.fdata=TX2messege.send.yawAbsoluteSpeed;
  SendBuffer[9]=u162float.u8data[0];
  SendBuffer[10]=u162float.u8data[1];
  SendBuffer[11]=u162float.u8data[2];
  SendBuffer[12]=u162float.u8data[3];
  //CRC16 bytes:
  Append_CRC16_Check_Sum(SendBuffer,15);
}

messege *getReceviedU6(void)
{
  return &mes1;
}

const getss *get_res(void)
{
  return &get1;
}


//void USART6_IRQHandler(void)			   //����6ȫ���жϷ�����//used to chat with TX2
//{
//  static u8 head,tail;
//  static u16 whole;
//	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//  {
//    led_red_toggle();
//  get1.res6=USART6->DR;
//  if(get1.res6>=64)
//  {
//    head=get1.res6-64;
//    mes1.received.flag=2;
//  }
//  else if(get1.res6<64&&mes1.received.flag==2)
//  {
//    tail=get1.res6;
//    whole=head*64+tail;
//    if(whole<330&&whole>0)
//    {
//      mes1.received.axis.y=((whole-165)/7.5+4.9)*8191/360;
//      mes1.received.flag=1;
//    }
//    else if(whole<830&&whole>0)
//    {
//      mes1.received.axis.x=(whole-218-218-165+30)/7.5;
//      mes1.received.flag=1;
//      
//    }
//    
//  }

//  
//  }
//}




