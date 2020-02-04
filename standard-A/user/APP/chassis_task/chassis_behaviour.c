/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       chassis_behaviour.c/h
  * @brief      ��ɵ�����Ϊ����
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#include "chassis_behaviour.h"
#include "include.h"


static const fp32 *ins;
static const Gimbal_Control_new *getGimbalControl;
/**
  * @brief          ������������Ϊ״̬���£�����ģʽ��raw���ʶ��趨ֵ��ֱ�ӷ��͵�can�����Ϲʶ����趨ֵ������Ϊ0
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      vy_set���ҵ��ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      wz_set��ת���ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */
static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set, chassis_move_t *chassis_move_rc_to_vector);

/**
  * @brief          ���̲��ƶ�����Ϊ״̬���£�����ģʽ�ǲ�����Ƕȣ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set��ת���ٶȣ���ת�ٶ��ǿ��Ƶ��̵ĵ��̽��ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_no_move_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);

/**
  * @brief          ���̸�����̨����Ϊ״̬���£�����ģʽ�Ǹ�����̨�Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      angle_set��������̨���Ƶ�����ԽǶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);

/**
  * @brief          ���̸������yaw����Ϊ״̬���£�����ģʽ�Ǹ�����̽Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      angle_set�������õ�yaw����Χ -PI��PI
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_engineer_follow_chassis_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);

/**
  * @brief          ���̲�����Ƕȵ���Ϊ״̬���£�����ģʽ�ǲ�����Ƕȣ�������ת�ٶ��ɲ���ֱ���趨
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */
static void chassis_no_follow_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);

/**
  * @brief          ���̿�������Ϊ״̬���£�����ģʽ��rawԭ��״̬���ʶ��趨ֵ��ֱ�ӷ��͵�can������
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */
static void chassis_open_set_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);

//������Ϊ״̬��
static chassis_behaviour_e chassis_behaviour_mode = CHASSIS_ZERO_FORCE;

void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
    if (chassis_move_mode == NULL)
    {
        return;
    }

    //ң����������Ϊģʽ
    if (switch_is_mid(chassis_move_mode->chassis_RC->rc.s[MODE_CHANNEL]))
    {
        chassis_behaviour_mode = CHASSIS_NO_FOLLOW_YAW;
    }
    else if (switch_is_down(chassis_move_mode->chassis_RC->rc.s[MODE_CHANNEL]))
    {
        chassis_behaviour_mode = CHASSIS_NO_MOVE;
    }
    else if (switch_is_up(chassis_move_mode->chassis_RC->rc.s[MODE_CHANNEL]))
    {
        chassis_behaviour_mode =CHASSIS_NO_MOVE;//2019/11/8fixed,CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW
    }

    //��̨����ĳЩ״̬��ʱ�򣬵��̱��ֲ���
//    if (gimbal_cmd_to_chassis_stop())
//    {
//        chassis_behaviour_mode = CHASSIS_NO_MOVE;
//    }

    //������Ϊ״̬��ѡ�����״̬��
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RAW; //����Ϊ�ǵ��������������õ���״̬��Ϊ raw��ԭ��״̬����
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_MOVE)
    {

        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_NO_FOLLOW_YAW; //����Ϊ�ǵ��̲��ƶ��������õ���״̬��Ϊ ���̲�����Ƕ� ״̬����
    }
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {

        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW; //����Ϊ����������������̨�������õ���״̬��Ϊ ���̸�����̨�Ƕ� ״̬����
    }
    else if (chassis_behaviour_mode == CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW)
    {

        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW; //����Ϊ�ǹ��̸�����̽Ƕȣ������õ���״̬��Ϊ ���̸�����̽Ƕ� ״̬����
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_FOLLOW_YAW)
    {

        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_NO_FOLLOW_YAW; //����Ϊ�ǵ��̲�����Ƕȣ������õ���״̬��Ϊ ���̲�����Ƕ� ״̬����
    }
    else if (chassis_behaviour_mode == CHASSIS_OPEN)
    {

        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RAW; //����Ϊ�ǵ��̿����������õ���״̬��Ϊ ����ԭ��raw ״̬����
    }
}
void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector)
{

    if (vx_set == NULL || vy_set == NULL || angle_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_zero_force_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_MOVE)
    {
        chassis_no_move_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
        chassis_infantry_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW)
    {
        chassis_engineer_follow_chassis_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_FOLLOW_YAW)
    {
        chassis_no_follow_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_OPEN)
    {
        chassis_open_set_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
}
/**
  * @brief          ������������Ϊ״̬���£�����ģʽ��raw���ʶ��趨ֵ��ֱ�ӷ��͵�can�����Ϲʶ����趨ֵ������Ϊ0
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      vy_set���ҵ��ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      wz_set��ת���ٶ� �趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_can_set == NULL || vy_can_set == NULL || wz_can_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
    *vx_can_set = 0.0f;
    *vy_can_set = 0.0f;
    *wz_can_set = 0.0f;
}
/**
  * @brief          ���̲��ƶ�����Ϊ״̬���£�����ģʽ�ǲ�����Ƕȣ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set��ת���ٶȣ���ת�ٶ��ǿ��Ƶ��̵ĵ��̽��ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_no_move_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
    *vx_set = 0.0f;
    *vy_set = 0.0f;
    *wz_set = 0.0f;
}

/**
  * @brief          ���̸�����̨����Ϊ״̬���£�����ģʽ�Ǹ�����̨�Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      angle_set��������̨���Ƶ�����ԽǶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || angle_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set, vy_set, chassis_move_rc_to_vector);

    //ҡ�ڽǶ�������sin�������ɣ�swing_time ��sin����������ֵ
    static fp32 swing_time = 0.0f;
    //swing_time �Ǽ�������ĽǶ�
    static fp32 swing_angle = 0.0f;
    //max_angle ��sin�����ķ�ֵ
    static fp32 max_angle = SWING_NO_MOVE_ANGLE;
    //add_time ��ҡ�ڽǶȸı�Ŀ��������Խ��
    static fp32 const add_time = PI / 250.0f;
    //ʹ��ҡ�ڱ�־λ
    static uint8_t swing_flag = 0;

    //����ң������ԭʼ�����ź�

    //�ж��Ƿ�Ҫҡ��
    if (0)//chassis_move_rc_to_vector->chassis_RC->key.v & SWING_KEY
    {
        if (swing_flag == 0)
        {
            swing_flag = 1;
            swing_time = 2.0f;
        }
    }
    else
    {
        swing_flag = 0;
    }

    //�жϼ��������ǲ����ڿ��Ƶ����˶����������˶���Сҡ�ڽǶ�
    if (chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_FRONT_KEY || chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_BACK_KEY ||
        chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_LEFT_KEY || chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_RIGHT_KEY)
    {
        max_angle = SWING_MOVE_ANGLE;
    }
    else
    {
        max_angle = SWING_NO_MOVE_ANGLE;
    }
    //sin�������ɿ��ƽǶ�
    if (swing_flag)
    {
        swing_angle = max_angle * arm_sin_f32(swing_time);
        swing_time += add_time;
    }
    else
    {
        swing_angle = 0.0f;
    }
    //sin����������2pi
    if (swing_time > 2 * PI)
    {
        swing_time -= 2 * PI;
    }

    *angle_set = swing_angle;
}

/**
  * @brief          ���̸������yaw����Ϊ״̬���£�����ģʽ�Ǹ�����̽Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      angle_set�������õ�yaw����Χ -PI��PI
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_engineer_follow_chassis_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || angle_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set, vy_set, chassis_move_rc_to_vector);

    *angle_set = rad_format(chassis_move_rc_to_vector->chassis_yaw_set - CHASSIS_ANGLE_Z_RC_SEN * chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_WZ_CHANNEL]);
}

/**
  * @brief          ���̲�����Ƕȵ���Ϊ״̬���£�����ģʽ�ǲ�����Ƕȣ�������ת�ٶ��ɲ���ֱ���趨
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_no_follow_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
    static float controlWz,insTurn;
    static u8 flagTurn180;
    static fp32 flagTurnFollowGimbal;
    static messege *getMes;
    getMes=getReceviedU6();
    ins=get_INS_angle_point();
    *wz_set=0;
    getGimbalControl=returnNewControlGimbal();
    chassis_rc_to_control_vector(vx_set, vy_set, chassis_move_rc_to_vector);
    *wz_set = -CHASSIS_WZ_RC_SEN * chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_WZ_CHANNEL];//give rool of chassis 0.01*max 660
 
    if(getGimbalControl->mode==camera_auto_mode)
    {
      static float controlChassis=0;
      static fp32 mouseCountrolx,receivedLast;
      static u8 countForCam=0;
      mouseCountrolx+=chassis_move_rc_to_vector->chassis_RC->mouse.x*0.04;
     if(mouseCountrolx>=1)
     {
        *wz_set=-2.5;
        mouseCountrolx=0;
        return;
     }
     else if(mouseCountrolx<=-1)
     {

        *wz_set=2.5 ;
        mouseCountrolx=0;
        return;
     }
      countForCam++;
      if(getMes->received.flag==1)
      {
        controlChassis=(getMes->received.axis.x)/13;
        getMes->received.flag=0;
        countForCam=0;
      }
      receivedLast=getMes->received.axis.x;
      *wz_set=-controlChassis;
      if(countForCam>=15)
      {
        *wz_set=0;
        countForCam=15;
      }
    }
    
    if(((chassis_move_rc_to_vector->chassis_RC->key.v&CHASSIS_TURN_180)==0)&&flagTurn180==0)
    {
      if(ins[0]*180/3.1415*20<=0)
        insTurn=ins[0]*180/3.1415+180;
      else
        insTurn=ins[0]*180/3.1415-180;
    }
    if((chassis_move_rc_to_vector->chassis_RC->key.v&CHASSIS_TURN_180)&&flagTurn180==0)
      flagTurn180=1;
    if(flagTurn180==1)
    {
      if((ins[0]*180/3.1415-insTurn>4)||(ins[0]*180/3.1415-insTurn)<-4)
      {
        *wz_set=9;
        return;
      }
      else
        flagTurn180=0;
    }
    
      if(getGimbalControl->mode==follow_chassis_mode)//&&getGimbalControl->yawDelta>=100)
    {
     if(fabs(Zero_Yaw-getGimbalControl->yawMotor.motor_measure->ecd)>50)//keep no move
     {
       *wz_set=PID_Calc(&pidForChassis,Zero_Yaw,getGimbalControl->yawMotor.motor_measure->ecd);
        mouseControlYawS+=*wz_set*2.506;
     }
     if(chassis_move_rc_to_vector->chassis_RC->key.v&CHASSIS_RIGHT_ROLL_KEY)
    {
      mouseControlYawS+=18;
    }
    else if(chassis_move_rc_to_vector->chassis_RC->key.v&CHASSIS_LEFT_ROLL_KEY)
    {
      mouseControlYawS-=18;
    }
     
    }
    

      
}

/**
  * @brief          ���̿�������Ϊ״̬���£�����ģʽ��rawԭ��״̬���ʶ��趨ֵ��ֱ�ӷ��͵�can������
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
  */

static void chassis_open_set_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    *vx_set = chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_X_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    *vy_set = -chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_Y_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    *wz_set = -chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_WZ_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    return;
}
