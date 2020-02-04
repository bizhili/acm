#ifndef GIMBAL_TASK_NEW_H
#define GIMBAL_TASK_NEW_H
#include "main.h"
#include "CAN_Receive.h"
#include "pid.h"
#include "remote_control.h"
#include "uartTask.h"
#include "sensor.h"
#define Zero_Pitch 4300
#define Zero_Yaw 1320+4096
#define pitchMax 4800
#define pitchMin 3650
#define NOT_FOLLOW_KEY  KEY_PRESSED_OFFSET_CTRL
#define CONTROL_TO_ZERO KEY_PRESSED_OFFSET_F
#define shootSensor PI6
typedef enum
{
	 zero_force_mode=0,
   rc_control_mode,
	 camera_auto_mode,
	 follow_chassis_mode,
   not_follow_chassis_mode,
} gimbal_mode;
typedef struct
{
  const motor_measure_t *motor_measure;
  fp32 camera_control;
  int set_current;
  fp32 motorGyro;
} Gimbal_include;

typedef struct
{
  Gimbal_include yawMotor,pitchMotor;
  gimbal_mode mode;
  uint8_t last_gimbal_mode;
  uint8_t ctrlOn;
  int yawDelta;
  float insNow;
  messege *mes;
} Gimbal_Control_new;

extern  const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void);
extern  const motor_measure_t *get_Pitch_Gimbal_Motor_Measure_Point(void);
extern  void gimbal_motor_update(Gimbal_Control_new *gimbal);
extern  void gimbal_mode_set(Gimbal_Control_new *gimbal);
extern  void gimbal_current_set(Gimbal_Control_new *gimbal);
extern  void gimbal_mode_change(Gimbal_Control_new *gimbal);
extern  void gimbal_init(Gimbal_Control_new *gimbal);
extern  void gimbal_control_pid_old(int x,int y,Gimbal_Control_new *gimbal);
extern  int gimbal_control_pid1(int now,int control_d,int expect);
extern void gimbal_motor_init(Gimbal_Control_new *gimbal);
extern const Gimbal_Control_new *returnNewControlGimbal(void);
extern fp32 mouseControlYawS;
#endif














