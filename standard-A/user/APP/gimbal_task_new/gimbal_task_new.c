#include "gimbal_task_new.h"
#include "include.h"
void gimbal_motor_update(Gimbal_Control_new *gimbal);
void gimbal_mode_set(Gimbal_Control_new *gimbal);
void gimbal_current_set(Gimbal_Control_new *gimbal);
void gimbal_mode_change(Gimbal_Control_new *gimbal);
void gimbal_init(Gimbal_Control_new *gimbal);
fp32 mouseControlYawS=0;
Gimbal_Control_new *gimbal_control_new;
static const RC_ctrl_t *gimbal_rc_ctrl;//rc return
static const fp32 pidYaw[]={62,0,60};//60,0,30
static const fp32 pidYawOut[]={25,0,30};//,25,0,30

static const fp32 pidPitch[]={55,0,50};
static const fp32 pidPitchOut[]={60,0,50};

static const float pidYawCam[]={42,0,666};
static const float pidPitchCam[]={820,0.05,900};

static PidTypeDef pidForYaw,pidForYawCam,pidForYawOut,pidForPitch,pidForPitchOut,pidForPitchCam;

static const fp32 *ins;
static fp32 control_yaw1,control_pitch1;
#define ECD_Format(ecd)         \
{                           \
    if ((ecd) > ecd_range)  \
       (ecd) -= ecd_range; \
    else if ((ecd) < 0)     \
       (ecd) += ecd_range; \
}
void gimbal_motor_init(Gimbal_Control_new *gimbal)
{
  PID_Init(&pidForYaw, PID_POSITION,pidYaw,YAW_SPEED_PID_MAX_OUT, 4000);
  PID_Init(&pidForYawOut, PID_POSITION,pidYawOut,YAW_SPEED_PID_MAX_OUT,4000);
  
  PID_Init(&pidForPitch, PID_POSITION,pidPitch,YAW_SPEED_PID_MAX_OUT, 4000);
  PID_Init(&pidForPitchOut, PID_POSITION,pidPitchOut,YAW_SPEED_PID_MAX_OUT,4000);
  PID_Init(&pidForYawCam,PID_POSITION, pidYawCam,30000,2000);
  PID_Init(&pidForPitchCam,PID_POSITION, pidPitchCam,30000,2000);
  
  gimbal->ctrlOn=0;
}

void gimbal_motor_update(Gimbal_Control_new *gimbal)//get motor return
{
  static u8 flagForCtrl=0;
  ins=get_INS_angle_point();
  gimbal->pitchMotor.motor_measure=get_Pitch_Gimbal_Motor_Measure_Point();
  gimbal->yawMotor.motor_measure=get_Yaw_Gimbal_Motor_Measure_Point();
  gimbal->mes=getReceviedU6();
  if((gimbal_rc_ctrl->key.v&NOT_FOLLOW_KEY)&&(flagForCtrl==0))
  {
    if (gimbal->ctrlOn==0)
      gimbal->ctrlOn=1;
    else if(gimbal->ctrlOn==1)
      gimbal->ctrlOn=0;
    
    flagForCtrl=1;
  }
  else if(((gimbal_rc_ctrl->key.v&NOT_FOLLOW_KEY)==0)&&(flagForCtrl==1))
    flagForCtrl=0;
  gimbal->pitchMotor.motorGyro=gimbal->pitchMotor.motor_measure->speed_rpm/60;//rps;
  gimbal->yawMotor.motorGyro=gimbal->yawMotor.motor_measure->speed_rpm/60;//rps
}
void gimbal_mode_set(Gimbal_Control_new *gimbal)
{
    gimbal->mode=zero_force_mode;
    if(toe_is_error(YawGimbalMotorTOE) || toe_is_error(PitchGimbalMotorTOE) || toe_is_error(TriggerMotorTOE))//check the motor online or not
    return;
    else if(gimbal->ctrlOn)
    gimbal->mode=not_follow_chassis_mode;
    else if(gimbal_rc_ctrl->rc.s[0]==1&&gimbal_rc_ctrl->rc.s[1]==1)//left up,right up
    gimbal->mode=rc_control_mode;
    else if(gimbal_rc_ctrl->rc.s[1]==3)//left mid
    gimbal->mode=follow_chassis_mode;
    else if(gimbal_rc_ctrl->rc.s[1]==2)//left down,right up
    gimbal->mode=camera_auto_mode;    
}
void gimbal_mode_change(Gimbal_Control_new *gimbal)//to change some value while changing mode
{
    if(gimbal->mode==gimbal->last_gimbal_mode)
    {
        if(gimbal->mode!=not_follow_chassis_mode)
          gimbal->insNow=ins[0]*180/3.1415f*20;
        return;
    }
    else
    {
      control_yaw1=0,control_pitch1=0;
      if(gimbal->last_gimbal_mode==not_follow_chassis_mode)
        if(gimbal->yawMotor.motor_measure->ecd>Zero_Yaw+300||gimbal->yawMotor.motor_measure->ecd<Zero_Yaw-300)
        {
          gimbal->mode=not_follow_chassis_mode;
          return;
        }
      gimbal->last_gimbal_mode=gimbal->mode;
      
    }
}
void gimbal_init(Gimbal_Control_new *gimbal)
{
    gimbal->last_gimbal_mode=zero_force_mode;
    gimbal_rc_ctrl=get_remote_control_point();
    
}
void gimbal_current_set(Gimbal_Control_new *gimbal)
{
    static float control_yaw=0,control_pitch=0;//to smooth the control value
    static fp32 motor_gyro_set;
    static fp32 mouseControlYawC=0,mouseControlPitchC=0;
    if(gimbal->mode==zero_force_mode)
    {
        gimbal->pitchMotor.set_current=0;
        gimbal->yawMotor.set_current=0;
    }
    else if(gimbal->mode==rc_control_mode)
    {
        if(gimbal_rc_ctrl->rc.ch[1]-control_pitch>100)//to smooth control
          control_pitch+=2.5f;
        else if(gimbal_rc_ctrl->rc.ch[1]-control_pitch<-100)
          control_pitch-=2.5f;
        else if(gimbal_rc_ctrl->rc.ch[1]-control_pitch>2)
          control_pitch+=2.5f;
        else if(gimbal_rc_ctrl->rc.ch[1]-control_pitch<-2)
          control_pitch-=2.5f;
        else
          control_pitch=gimbal_rc_ctrl->rc.ch[1];
        
         if(gimbal_rc_ctrl->rc.ch[0]-control_yaw>2)
          control_yaw+=4;
        else if(gimbal_rc_ctrl->rc.ch[0]-control_yaw<-2)
          control_yaw-=4;
        else
          control_yaw=gimbal_rc_ctrl->rc.ch[0];
        
        //for pitch motor
        motor_gyro_set =-PID_Calc(&pidForPitchOut,gimbal->pitchMotor.motor_measure->ecd,-control_pitch+Zero_Pitch);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        gimbal->pitchMotor.set_current=-PID_Calc(&pidForPitch,gimbal->pitchMotor.motorGyro,motor_gyro_set);
        //for yaw motor
        motor_gyro_set =-PID_Calc(&pidForYawOut,gimbal->yawMotor.motor_measure->ecd,-control_yaw*2.07f+Zero_Yaw);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        gimbal->yawMotor.set_current=-PID_Calc(&pidForYaw,gimbal->yawMotor.motorGyro,motor_gyro_set);
    }
    
    else if(gimbal->mode==follow_chassis_mode)
    {
         static fp32 mouseControlPitch=0;
         if(gimbal_rc_ctrl->key.v&CONTROL_TO_ZERO)
         mouseControlYawS=0,mouseControlPitch=0;
         mouseControlYawS+=gimbal_rc_ctrl->mouse.x*0.02;
         mouseControlPitch+=gimbal_rc_ctrl->mouse.y*0.03;
         if(mouseControlYawS>0)
           gimbal->yawDelta=mouseControlYawS;
         else
           gimbal->yawDelta=-mouseControlYawS;
         
         
         if(mouseControlYawS>1000)//limit value ,in case beyond max value
            mouseControlYawS=1000;
         else if(mouseControlYawS<-1000)//limit value ,in case beyond min value
            mouseControlYawS=-1000;
    
         if(mouseControlPitch>660)//limit value ,in case beyond max value
            mouseControlPitch=660;
         else if(mouseControlPitch<-300)//limit value ,in case beyond min value
            mouseControlPitch=-300;
         
         if(mouseControlYawS-control_yaw1>6)
            control_yaw1+=6;
         else if(mouseControlYawS-control_yaw1<-6)
          control_yaw1-=6;
         else
          control_yaw1=mouseControlYawS;
         if(mouseControlPitch-control_pitch1>10)
          control_pitch1+=10;
         else if(mouseControlPitch-control_pitch1<-10)
          control_pitch1-=10;
         else
          control_pitch1=mouseControlPitch;
                 //for pitch motor
        motor_gyro_set =-PID_Calc(&pidForPitchOut,gimbal->pitchMotor.motor_measure->ecd,-control_pitch1+Zero_Pitch);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        gimbal->pitchMotor.set_current=-PID_Calc(&pidForPitch,gimbal->pitchMotor.motorGyro,motor_gyro_set);
         
         motor_gyro_set =-PID_Calc(&pidForYawOut,gimbal->yawMotor.motor_measure->ecd,-control_yaw1+Zero_Yaw);//10000,50
         motor_gyro_set=motor_gyro_set/240;//50
         gimbal->yawMotor.set_current=-PID_Calc(&pidForYaw,gimbal->yawMotor.motorGyro,motor_gyro_set);
        
    }
    
    
    else if(gimbal->mode==not_follow_chassis_mode)
    {
         static fp32 setControl;
         static fp32 insNowConrtol;
         if(gimbal_rc_ctrl->key.v&CONTROL_TO_ZERO)
         mouseControlYawC=0,mouseControlPitchC=0;
         mouseControlYawC+=gimbal_rc_ctrl->mouse.x*0.02;
         mouseControlPitchC+=gimbal_rc_ctrl->mouse.y*0.02;
         if(mouseControlYawC>7200)//limit value ,in case beyond max value
            mouseControlYawC=0;
         else if(mouseControlYawC<-7200)//limit value ,in case beyond min value
            mouseControlYawC=-0;
         if(mouseControlPitchC>660)//limit value ,in case beyond max value
            mouseControlPitchC=660;
         else if(mouseControlPitchC<-300)//limit value ,in case beyond min value
            mouseControlPitchC=-300;
         if(mouseControlYawC-control_yaw1>4)
            control_yaw1+=4;
         else if(mouseControlYawC-control_yaw1<-4)
          control_yaw1-=4;
         else
          control_yaw1=mouseControlYawC;
         if(mouseControlPitchC-control_pitch1>7)
          control_pitch1+=7;
         else if(mouseControlPitchC-control_pitch1<-7)
          control_pitch1-=7;
         else
          control_pitch1=mouseControlPitchC;
                 //for pitch motor
        motor_gyro_set =-PID_Calc(&pidForPitchOut,gimbal->pitchMotor.motor_measure->ecd,-control_pitch1+Zero_Pitch);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        gimbal->pitchMotor.set_current=-PID_Calc(&pidForPitch,gimbal->pitchMotor.motorGyro,motor_gyro_set);
        insNowConrtol=ins[0]*180/3.1415f;
        setControl=gimbal->insNow-mouseControlYawC;
        if(setControl>180*20)//to convert the value between -180~180 degrees
            setControl=setControl-360*20;
        else if(setControl<-180*20)
            setControl=setControl+360*20;
        if(setControl>90*20)//Dynamic mapping
        {
            if(insNowConrtol<-90)
              insNowConrtol+=360;
        }
        else if(setControl<-90*20)
        {
            if(insNowConrtol>90)
              insNowConrtol-=360;
        }
        gimbal->yawMotor.set_current=-PID_Calc(&pidForYawCam,setControl,insNowConrtol*20); 
    }
    
    else if(gimbal->mode==camera_auto_mode)   //相机自动模式  3，1
    {
        static fp32 cam_control_pitch=0;//to smooth the control value
        static fp32 mouseControly=0,controlAll;
        mouseControly-=gimbal_rc_ctrl->mouse.y*0.01;
        controlAll=mouseControly+gimbal->mes->received.axis.y;
        if(gimbal_rc_ctrl->key.v&CONTROL_TO_ZERO)
          mouseControly=0;
        if(controlAll>660)//limit value ,in case beyond max value
          controlAll=660;
        else if(controlAll<-660)//limit value ,in case beyond min value
            controlAll=-660;
        if(controlAll>660)//limit value ,in case beyond max value
            controlAll=660;
        if(mouseControly>0.2f)
          mouseControly-=0.1f;
        else if(mouseControly<-0.2f)
          mouseControly+=0.1f;
        gimbal->pitchMotor.camera_control=(controlAll);
        if(gimbal->pitchMotor.camera_control-cam_control_pitch>2)
          cam_control_pitch+=2;
        else if(gimbal->pitchMotor.camera_control-cam_control_pitch<-2)
          cam_control_pitch-=2;
        else
          cam_control_pitch=gimbal->pitchMotor.camera_control;
                //for pitch motor
        motor_gyro_set =-PID_Calc(&pidForPitchOut,gimbal->pitchMotor.motor_measure->ecd,cam_control_pitch+Zero_Pitch);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        gimbal->pitchMotor.set_current=-PID_Calc(&pidForPitch,gimbal->pitchMotor.motorGyro,motor_gyro_set);
        //for yaw motor
        motor_gyro_set =-PID_Calc(&pidForYawOut,gimbal->yawMotor.motor_measure->ecd,Zero_Yaw);//10000,50
        motor_gyro_set=motor_gyro_set/240;//50
        if(gimbal->mode==camera_auto_mode)
        gimbal->yawMotor.set_current=-PID_Calc(&pidForYaw,gimbal->yawMotor.motorGyro,motor_gyro_set);
    }
    
}


