/**
 * @file Error_detect.c
 * @author sethome
 * @brief 错误检查
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "global_status.h" 
#include "Error_detect.h"
#include "CAN_receive&send.h"
#include "chassis_move.h"
#include "gimbal.h"
#include "shoot.h"
#include "CAN_ID_Library.h"

struct Error_detect_t Error_detect;

void Error_detect_flush(void)
{
    // 遥控器检测
    static uint32_t last_remote = 0;		//遥控器检测应该就是DBUS了
    if (last_remote == Error_detect.remote.last_time)
    {
        Global.err[REMOTE_ERR] = 1;
        Error_detect.remote.flag = 1;
    }
    else
    {
        Global.err[REMOTE_ERR] = 0;
        Error_detect.remote.flag = 0;
    }
    last_remote = Error_detect.remote.last_time;

    // 电机检测
    Error_detect_motor(chassis_FR);
    Error_detect_motor(chassis_FL);
    Error_detect_motor(chassis_BL);
    Error_detect_motor(chassis_BR);
    if (Error_detect.motor.flag[chassis_FR] ||
        Error_detect.motor.flag[chassis_FL] ||
        Error_detect.motor.flag[chassis_BL] ||
        Error_detect.motor.flag[chassis_BR])
        Global.err[CHASSIS_ERR] = 1;	//如果都出错，直接给 Global.err[CHASSIS_ERR] = 1，下同
    else
        Global.err[CHASSIS_ERR] = 0;

    Error_detect_motor(YAW_MOTOR);
    Error_detect_motor(PITCH_MOTOR);
    if (Error_detect.motor.flag[YAW_MOTOR] ||
        Error_detect.motor.flag[PITCH_MOTOR])
        Global.err[GIMBAL_ERR] = 1;
    else
        Global.err[GIMBAL_ERR] = 0;

#ifdef USE_3508_AS_SHOOT_MOTOR
    Error_detect_motor(SHOOT_MOTOR1);
    Error_detect_motor(SHOOT_MOTOR2);
#endif

    Error_detect_motor(TRIGGER_MOTOR);
    if (Error_detect.motor.flag[TRIGGER_MOTOR] ||
        Error_detect.motor.flag[SHOOT_MOTOR1] ||
        Error_detect.motor.flag[SHOOT_MOTOR2])
        Global.err[SHOOT_ERR] = 1;
    else
        Global.err[SHOOT_ERR] = 0;
}

/**
 * @brief 电机错误检测，在FreeRTOS中调用
 *
 * @param ID
 */
void Error_detect_motor(can_id ID)
{
    uint16_t tmp1 = get_motor_data(ID).given_current;
    uint16_t tmp2 = get_motor_data(ID).ecd;
    if (Error_detect.motor.last_given_current[ID] == tmp1 &&
        Error_detect.motor.last_ecd[ID] == tmp2)
        Error_detect.motor.err_cnt[ID]++;            //这个是上次电流和编码器等于现在电流和编码器错误就加一吗
    else
    {
        Error_detect.motor.err_cnt[ID] = 0;	//否则无错误
        Error_detect.motor.flag[ID] = 0;
    }

    Error_detect.motor.last_given_current[ID] = tmp1;
    Error_detect.motor.last_ecd[ID] = tmp2;

    if (Error_detect.motor.err_cnt[ID] > 3)
    {
        Error_detect.motor.err_cnt[ID] = 0;
        Error_detect.motor.flag[ID] = 1;	//标志位直接放置1
    }
}

void Error_detect_remote(void)
{
    Error_detect.remote.last_time++;	//错误去除？
}														//这段函数仅仅在DBUS_remote中使用过
