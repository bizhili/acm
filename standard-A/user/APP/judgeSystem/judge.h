#ifndef JUDGE_H
#define JUDGE_H
#include "include.h"
#define judgeDelayTime 3
extern u8 point;
/*
cmdNum length of data(byte) fuctions
0x0001 3                    比赛状态数据，1Hz 周期发送
0x0002 1                    比赛结果数据，比赛结束后发送
0x0003 2                    比赛机器人存活数据，1Hz 周期发送
0x0101 4                    场地事件数据，事件改变后发送
0x0102 3                    场地补给站动作标识数据，动作改变后发送
0x0103 2                    请求补给站补弹数据，由参赛队发送，上限 10Hz。（RM 对抗赛尚未开放）
0x0201 15                   机器人状态数据，10Hz 周期发送
0x0202 14                   实时功率热量数据，50Hz 周期发送
0x0203 16                   机器人位置数据，10Hz 发送
0x0204 1                    机器人增益数据，增益状态改变后发送
0x0205 3                    空中机器人能量状态数据，10Hz 周期发送，只有空中机器人主控发送
0x0206 1                    伤害状态数据，伤害发生后发送
0x0207 6                    实时射击数据，子弹发射后发送
0x0301 n                    机器人间交互数据，发送方触发发送，上限 10Hz
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
 uint8_t game_type : 4;// 1：RoboMaster 机甲大师赛；2：RoboMaster 机甲大师单项赛；3：ICRA RoboMaster 人工智能挑战赛
 uint8_t game_progress : 4;//0：未开始比赛；1：准备阶段；2：自检阶段；3：5s 倒计时；4：对战中；5：比赛结算中
 uint16_t stage_remain_time;//当前阶段剩余时间，单位 s
} ext_game_state_t;
typedef __packed struct//0x0002
{
 uint8_t winner;//0 平局 1 红方胜利 2 蓝方胜利
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
