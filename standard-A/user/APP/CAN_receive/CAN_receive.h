#ifndef CANTASK_H
#define CANTASK_H
#include "main.h"

#define CHASSIS_CAN CAN1
#define GIMBAL_CAN CAN1

/* CAN send and receive ID */

typedef enum
{
  //can1,receive1:
  CAN_CHASSIS_ALL_ID = 0x200,
  CAN_3508_M1_ID = 0x201,
  CAN_3508_M2_ID = 0x202,
  CAN_3508_M3_ID = 0x203,
  CAN_3508_M4_ID = 0x204,
  
  //can1,receive2:
  CAN_CHASSIS_ALL_ID2 = 0x1FF,
  CAN_3508_M5_ID = 0x205,
  CAN_3508_M6_ID = 0x206,
  CAN_3508_M7_ID = 0x207,
  CAN_3508_M8_ID = 0x208,
  
  //can2,receive1:
  CAN_YAW_MOTOR_ID = 0x205,
  CAN_PIT_MOTOR_ID = 0x206,
  CAN_TRIGGER_MOTOR_ID = 0x207,
  CAN_GIMBAL_MOTOR_S1_ID= 0x208,
  CAN_GIMBAL_ALL_ID = 0x1FF,
  
  //can2,receive2:
  CAN_GIMBAL_MOTOR_S2_ID = 0x209,
  CAN_GIMBAL_MOTOR_S3_ID = 0x20A,
  CAN_GIMBAL_MOTOR_S4_ID = 0x20B,
  CAN_GIMBAL_ALL_ID2 = 0x2FF,
  
  CAN_JUDGE=0x104,

} can_msg_id_e;

//rm���ͳһ���ݽṹ��
typedef struct
{
    uint16_t ecd;  //�Ƕ�  0-8191
    int16_t speed_rpm;     //תÿ��
    int16_t given_current;     //����
    uint8_t temperate;
    int16_t last_ecd;
} motor_measure_t;
extern void CAN_CMD_CHASSIS_RESET_ID(void);

//������̨�����������revΪ�����ֽ�
extern void CAN_CMD_GIMBAL(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);
//���͵��̵����������
extern void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);


//������̨�����������revΪ�����ֽ�
extern void CAN_CMD_GIMBAL2(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);//�ڶ���������
//���͵��̵����������
extern void CAN_CMD_CHASSIS2(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);



//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S1_Measure_Point(void);
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S2_Measure_Point(void);
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S3_Measure_Point(void);
//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Gimbal_Motor_S4_Measure_Point(void);

//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void);
//����pitch���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Pitch_Gimbal_Motor_Measure_Point(void);

//����trigger���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Trigger_Motor_Measure_Point(void);

//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����,i�ķ�Χ��1~7����Ӧ0x201-0x207,
extern const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i);

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
extern void GIMBAL_lose_slove(void);
#endif

#endif
