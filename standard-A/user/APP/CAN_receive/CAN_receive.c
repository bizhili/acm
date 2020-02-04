/**
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  * @file       can_receive.c/h
  * @brief      ���can�豸�����շ����������ļ���ͨ��can�ж���ɽ���,֧�����7��3508��7��6020���ܹ�15�����ʹ��,ע��6��3508������
  * @note       ���ļ�����freeRTOS����
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020/1/12       RM&BIZHI             1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  */
#include "include.h"
#include "CAN_receive.h"

//���̵�����ݶ�ȡ
#define get_motor_measure(ptr, rx_message)                                                     \
    {                                                                                          \
        (ptr)->last_ecd = (ptr)->ecd;                                                          \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);     \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]); \
        (ptr)->temperate = (rx_message)->Data[6];                                              \
    }

//��̨������ݶ�ȡ
#define get_gimbal_motor_measuer(ptr, rx_message)                                              \
    {                                                                                          \
        (ptr)->last_ecd = (ptr)->ecd;                                                          \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]); \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);     \
        (ptr)->temperate = (rx_message)->Data[6];                                              \
    }

//ͳһ����can���պ���
static void CAN1_hook(CanRxMsg *rx_message);
//ͳһ����can���պ���
static void CAN2_hook(CanRxMsg *rx_message);
//�����������
static motor_measure_t motor_yaw, motor_pit, motor_trigger,motor_s1,
motor_s2,motor_s3,motor_s4,motor_chassis[8];

static CanTxMsg GIMBAL_TxMessage;
#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
static uint8_t delay_time = 100;
#endif

//can1�ж�
void CAN1_RX0_IRQHandler(void)
{
    static CanRxMsg rx1_message;

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);
        CAN1_hook(&rx1_message);
    }
}

//can2�ж�
void CAN2_RX0_IRQHandler(void)
{
    static CanRxMsg rx2_message;
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx2_message);
        CAN2_hook(&rx2_message);
    }
}

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
void GIMBAL_lose_slove(void)
{
        delay_time = RNG_get_random_range(13,239);
}
#endif
//������̨�����������revΪ�����ֽ�
void CAN_CMD_GIMBAL(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev)//0x205~0x208
{
    GIMBAL_TxMessage.StdId = CAN_GIMBAL_ALL_ID;
    GIMBAL_TxMessage.IDE = CAN_ID_STD;
    GIMBAL_TxMessage.RTR = CAN_RTR_DATA;
    GIMBAL_TxMessage.DLC = 0x08;
    GIMBAL_TxMessage.Data[0] = (yaw >> 8);
    GIMBAL_TxMessage.Data[1] = yaw;
    GIMBAL_TxMessage.Data[2] = (pitch >> 8);
    GIMBAL_TxMessage.Data[3] = pitch;
    GIMBAL_TxMessage.Data[4] = (shoot >> 8);
    GIMBAL_TxMessage.Data[5] = shoot;
    GIMBAL_TxMessage.Data[6] = (rev >> 8);
    GIMBAL_TxMessage.Data[7] = rev;

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE

    TIM6->CNT = 0;
    TIM6->ARR = delay_time ;

    TIM_Cmd(TIM6,ENABLE);
#else
    CAN_Transmit( GIMBAL_CAN,  &GIMBAL_TxMessage );
#endif

}

//������̨�����������revΪ�����ֽ�
void CAN_CMD_GIMBAL2(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev)//0x209~0x20B
{
    GIMBAL_TxMessage.StdId = CAN_GIMBAL_ALL_ID2;
    GIMBAL_TxMessage.IDE = CAN_ID_STD;
    GIMBAL_TxMessage.RTR = CAN_RTR_DATA;
    GIMBAL_TxMessage.DLC = 0x08;
    GIMBAL_TxMessage.Data[0] = (yaw >> 8);
    GIMBAL_TxMessage.Data[1] = yaw;
    GIMBAL_TxMessage.Data[2] = (pitch >> 8);
    GIMBAL_TxMessage.Data[3] = pitch;
    GIMBAL_TxMessage.Data[4] = (shoot >> 8);
    GIMBAL_TxMessage.Data[5] = shoot;
    GIMBAL_TxMessage.Data[6] = (rev >> 8);
    GIMBAL_TxMessage.Data[7] = rev;

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE

    TIM6->CNT = 0;
    TIM6->ARR = delay_time ;

    TIM_Cmd(TIM6,ENABLE);
#else
    CAN_Transmit( GIMBAL_CAN,  &GIMBAL_TxMessage );
#endif

}

void TIM6_DAC_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM6, TIM_IT_Update )!= RESET )
    {

        TIM_ClearFlag( TIM6, TIM_IT_Update );
#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
        CAN_Transmit( GIMBAL_CAN,  &GIMBAL_TxMessage );
#endif
        TIM_Cmd(TIM6,DISABLE);
    }
}
//CAN ���� 0x700��ID�����ݣ�������M3508�����������IDģʽ
void CAN_CMD_CHASSIS_RESET_ID(void)
{

    CanTxMsg TxMessage;
    TxMessage.StdId = 0x700;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = 0;
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0;
    TxMessage.Data[4] = 0;
    TxMessage.Data[5] = 0;
    TxMessage.Data[6] = 0;
    TxMessage.Data[7] = 0;
    CAN_Transmit(CAN2, &TxMessage);
}

//���͵��̵����������
void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)//0x201~0x204
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_CHASSIS_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CHASSIS_CAN, &TxMessage);
}

//���͵��̵����������
void CAN_CMD_CHASSIS2(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)//0x205~0x208
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_CHASSIS_ALL_ID2;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CHASSIS_CAN, &TxMessage);
}

//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void)
{
    return &motor_yaw;
}

//����pitch���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Pitch_Gimbal_Motor_Measure_Point(void)
{
    return &motor_pit;
}

//����trigger���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Trigger_Motor_Measure_Point(void)
{
    return &motor_trigger;
}
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S1_Measure_Point(void)
{
    return &motor_s1;
}
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S2_Measure_Point(void)
{
    return &motor_s2;
}

//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S3_Measure_Point(void)
{
    return &motor_s3;
}
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S4_Measure_Point(void)
{
    return &motor_s4;
}

//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis[i-1];
}

//ͳһ����can1�жϺ��������Ҽ�¼�������ݵ�ʱ�䣬��Ϊ�����ж�����
static void CAN1_hook(CanRxMsg *rx_message)
{
    switch (rx_message->StdId)
    {

    case CAN_3508_M1_ID:
    case CAN_3508_M2_ID:
    case CAN_3508_M3_ID:
    case CAN_3508_M4_ID:
    case CAN_3508_M5_ID:
    case CAN_3508_M6_ID:
    case CAN_3508_M7_ID:
    case CAN_3508_M8_ID:
    {
        static uint8_t i = 0;
        //������ID��
        i = rx_message->StdId - CAN_3508_M1_ID;
      
        //���������ݺ꺯��
        get_motor_measure(&motor_chassis[i], rx_message);
        //��¼ʱ��
        DetectHook(ChassisMotor1TOE + i);
        break;
    }

    default:
    {
        break;
    }
    }
}

//ͳһ����can2�жϺ��������Ҽ�¼�������ݵ�ʱ�䣬��Ϊ�����ж�����
static void CAN2_hook(CanRxMsg *rx_message)
{
    switch (rx_message->StdId)
    {
    case CAN_YAW_MOTOR_ID:
    {
        //���������ݺ꺯��
        get_gimbal_motor_measuer(&motor_yaw, rx_message);
        //��¼ʱ��
        DetectHook(YawGimbalMotorTOE);
        break;
    }
    case CAN_PIT_MOTOR_ID:
    {
        //���������ݺ꺯��
        get_gimbal_motor_measuer(&motor_pit, rx_message);
        DetectHook(PitchGimbalMotorTOE);
        break;
    }
    case CAN_TRIGGER_MOTOR_ID:
    {
        //���������ݺ꺯��
        get_motor_measure(&motor_trigger, rx_message);
        //��¼ʱ��
        DetectHook(TriggerMotorTOE);
        break;
    }
    case CAN_GIMBAL_MOTOR_S1_ID:
    {
        //���������ݺ꺯��
        get_gimbal_motor_measuer(&motor_s1, rx_message);
        //��¼ʱ��
        DetectHook(YawGimbalMotorTOE);
        break;
    }
    case CAN_GIMBAL_MOTOR_S2_ID:
    {
        //���������ݺ꺯��
        get_gimbal_motor_measuer(&motor_s2, rx_message);
        DetectHook(PitchGimbalMotorTOE);
        break;
    }
    case CAN_GIMBAL_MOTOR_S3_ID:
    {
        //���������ݺ꺯��
        get_gimbal_motor_measuer(&motor_s3, rx_message);
        //��¼ʱ��
        DetectHook(TriggerMotorTOE);
        break;
    }
    case CAN_GIMBAL_MOTOR_S4_ID:
    {
        //���������ݺ꺯��
        get_motor_measure(&motor_s4, rx_message);
        //��¼ʱ��
        DetectHook(TriggerMotorTOE);
        break;
    }
    default:
    {
        break;
    }
    }
}
