#ifndef JUDGE_H
#define JUDGE_H
#include "include.h"
#define judgeDelayTime 3
extern u8 point;
/*
cmdNum length of data(byte) fuctions
0x0001 3                    ����״̬���ݣ�1Hz ���ڷ���
0x0002 1                    ����������ݣ�������������
0x0003 2                    ���������˴�����ݣ�1Hz ���ڷ���
0x0101 4                    �����¼����ݣ��¼��ı����
0x0102 3                    ���ز���վ������ʶ���ݣ������ı����
0x0103 2                    ���󲹸�վ�������ݣ��ɲ����ӷ��ͣ����� 10Hz����RM �Կ�����δ���ţ�
0x0201 15                   ������״̬���ݣ�10Hz ���ڷ���
0x0202 14                   ʵʱ�����������ݣ�50Hz ���ڷ���
0x0203 16                   ������λ�����ݣ�10Hz ����
0x0204 1                    �������������ݣ�����״̬�ı����
0x0205 3                    ���л���������״̬���ݣ�10Hz ���ڷ��ͣ�ֻ�п��л��������ط���
0x0206 1                    �˺�״̬���ݣ��˺���������
0x0207 6                    ʵʱ������ݣ��ӵ��������
0x0301 n                    �����˼佻�����ݣ����ͷ��������ͣ����� 10Hz
*/
typedef __packed struct
{
  u8 sof;
  u16 data_length;
  u8 seq;
  u8 crc8;
}frame_header;  
typedef __packed struct//0x0001
{
 uint8_t game_type : 4;// 1��RoboMaster ���״�ʦ����2��RoboMaster ���״�ʦ��������3��ICRA RoboMaster �˹�������ս��
 uint8_t game_progress : 4;//0��δ��ʼ������1��׼���׶Σ�2���Լ�׶Σ�3��5s ����ʱ��4����ս�У�5������������
 uint16_t stage_remain_time;//��ǰ�׶�ʣ��ʱ�䣬��λ s
} ext_game_state_t;
typedef __packed struct//0x0002
{
 uint8_t winner;//0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
} ext_game_result_t;
typedef __packed struct//0x0003
{
 uint16_t robot_legion;
} ext_game_robot_survivors_t;
typedef __packed struct//0x0101
{
 uint32_t event_type;
} ext_event_data_t;
typedef __packed struct//0x0102
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
 uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;
typedef __packed struct//0x0103
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
uint8_t supply_num;
} ext_supply_projectile_booking_t;
typedef __packed struct//0x0201
{
 uint8_t robot_id;
 uint8_t robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint8_t mains_power_gimbal_output : 1;
 uint8_t mains_power_chassis_output : 1;
 uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;
typedef __packed struct//0x0202
{
 uint16_t chassis_volt;
 uint16_t chassis_current;
 float chassis_power;
 uint16_t chassis_power_buffer;
 uint16_t shooter_heat0;
 uint16_t shooter_heat1;
} ext_power_heat_data_t;
typedef __packed struct//0x0203
{
 float x;
 float y;
 float z;
 float yaw;
} ext_game_robot_pos_t;
typedef __packed struct//0x0204
{
 uint8_t power_rune_buff;
}ext_buff_musk_t;
typedef __packed struct//0x0205
{
 uint8_t energy_point;
 uint8_t attack_time;
} aerial_robot_energy_t;
typedef __packed struct//0x0206
{
 uint8_t armor_id : 4;
 uint8_t hurt_type : 4;
} ext_robot_hurt_t;
typedef __packed struct//0x0207
{
 uint8_t bullet_type;
 uint8_t bullet_freq;
 float bullet_speed;
} ext_shoot_data_t;
typedef __packed struct//0x0301
{
 uint16_t data_cmd_id;
 uint16_t send_ID;
 uint16_t receiver_ID;
}ext_student_interactive_header_data_t;

extern void judgeTask(void *pvParameters);
#endif
