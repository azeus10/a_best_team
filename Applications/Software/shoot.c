/**
 * @file shoot.c
 * @author sethome
 * @brief 发射模块
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CAN_ID_Library.h"


#include "Global_status.h"
#include "CAN_receive&send.h"
#include "shoot.h"
#include "pid.h"
#include "referee_handle_pack.h"
#include "RampFunc.h"
#include "nloop_middlewares.h"
#include "Stm32_time.h"
#include "vofa.h"

pid_t trigger_speed_pid;
pid_t trigger_location_pid;
pid_t shoot1_speed_pid;
pid_t shoot2_speed_pid;
pid_t shoot3_speed_pid;
shoot_t shoot;
RampGenerator shootRamp;
static enum trigger_status_e last_status = SPEEDS;

// 子弹计数
static int BulletCnt = 0;
// 初始化
void shoot_init()
{
	#define USE_3508_AS_SHOOT_MOTOR
	
	
	
#ifdef USE_3508_AS_SHOOT_MOTOR
	pid_set(&shoot1_speed_pid, 20, 0, 0.0, 5000, 0.0);
	pid_set(&shoot2_speed_pid, 20, 0, 0.0, 5000, 0.0);
	pid_set(&shoot3_speed_pid, 20, 0, 0.0, 5000, 0.0);
#endif
	
//	pid_set(&trigger_speed_pid, 2.20000005, 0, 50, 16000, 0); // 16000
//	pid_set(&trigger_location_pid, 0.75, 0, 1.20000005, 15000, 0);//0.5
	pid_set(&trigger_speed_pid, 2.5,0,67, 16000, 0); // 16000
	pid_set(&trigger_location_pid, 0.75, 0, 1.2, 15000, 0);//0.5
	
	shoot.trigger_status = SPEEDS;
//	shoot.trigger_status = LOCATIONS;
	shoot.last_status = SPEEDS;
//	shoot.last_status = LOCATIONS;
	shoot.trigger_location.set = 0;
	shoot.trigger_location.now = 0;
	shoot.SetSpeedUp = 0;
	shoot.SetSpeedLeft = 0;
	shoot.SetSpeedRight = 0;
	shoot.compensate_angle = 85;		///compensate补偿角度
	shoot.speedUpLevel = 5100;//6000;
	shoot.speedRightLevel = -5300;//-6150;
	shoot.speedLeftLevel = -5300;//-6150;
	
}
// 更新拨弹电机数据
void shoot_update()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	// 如果使用3508作为摩擦轮电机的话
	decode_as_3508(SHOOT_MOTOR1);
	decode_as_3508(SHOOT_MOTOR2);
	decode_as_3508(SHOOT_MOTOR3);

	shoot.SpeedUp = get_motor_data(SHOOT_MOTOR1).speed_rpm;
	shoot.SpeedRight = get_motor_data(SHOOT_MOTOR2).speed_rpm;
	shoot.SpeedLeft = get_motor_data(SHOOT_MOTOR3).speed_rpm;
#endif

	decode_as_3508(TRIGGER_MOTOR);
	shoot.trigger_location.now = get_motor_data(TRIGGER_MOTOR).ecd_cnt;
	shoot.trigger_speed = get_motor_data(TRIGGER_MOTOR).speed_rpm;
}

void shoot_set_shoot_Motor_speed(float SpeedUp, float SpeedLeft, float SpeedRight)
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	set_motor(pid_cal(&shoot1_speed_pid, get_motor_data(SHOOT_MOTOR1).speed_rpm, -SpeedUp), SHOOT_MOTOR1);
	set_motor(pid_cal(&shoot2_speed_pid, get_motor_data(SHOOT_MOTOR2).speed_rpm, -SpeedRight), SHOOT_MOTOR2);
	set_motor(pid_cal(&shoot3_speed_pid, get_motor_data(SHOOT_MOTOR3).speed_rpm, SpeedLeft), SHOOT_MOTOR3);
	
	
	UploadData_vofa(trigger_location_pid.err * 60 / 26219,trigger_location_pid.set,trigger_location_pid.total_out,0);	///vofa+调试吗
#else
	// 适配其他拨弹电机
	PWM_snaill_set(PIN_2, (uint16_t)speed);
	PWM_snaill_set(PIN_3, (uint16_t)speed);
#endif
}
void shoot_pid_cal()
{
	// 摩擦轮转速赋值
	if (Global.input.shooter_status)
	{
		shoot.SetSpeedUp = shoot.speedUpLevel;
		shoot.SetSpeedLeft = shoot.speedLeftLevel;
		shoot.SetSpeedRight = shoot.speedRightLevel;
	}
	else
	{
		shoot.SetSpeedUp = 0;
		shoot.SetSpeedLeft = 0;
		shoot.SetSpeedRight = 0;
	}
	
//		/*************测试*************/	
//		shoot.SetSpeedUp = 200;
//		shoot.SetSpeedLeft = 200;
//		shoot.SetSpeedRight = 200;
	
	// 摩擦轮设定
	decode_as_3508(TRIGGER_MOTOR);
	shoot_set_shoot_Motor_speed((float)(shoot.SetSpeedUp), (float)(shoot.SetSpeedLeft), (float)(shoot.SetSpeedRight));

	// 拨弹电机设定
	if (shoot.trigger_status == LOCATIONS) // 位置控制
	{
		if (last_status == SPEEDS)
		{
			shoot.trigger_location.set += shoot.trigger_location.now;
		}
		rampIterate(&shootRamp);
		shoot.trigger_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shootRamp.currentValue);
		//		shoot.trigger_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shoot.trigger_location.set);
	}
	else if (shoot.trigger_status == SPEEDS) // 速度控制
	{
		shoot.trigger_speed = shoot.set_trigger_speed;	///shoot.set_trigger_speed这个在哪定义的？
	}

	// 速度环
	if (Global.input.isOnForce == OPEN)
	{
		set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.trigger_speed), TRIGGER_MOTOR);
	}
	// 卸力模式
	else if (Global.input.isOnForce == CLOSE)
	{
		set_motor(0, TRIGGER_MOTOR);
//		rampInit(&shootRamp, shoot.trigger_location.now, shoot.trigger_location.now, -1, 1);//以屎克屎的办法，有时间找找原因
		shootRamp.currentValue = shoot.trigger_location.now;	//currentValue 是当前值
		shootRamp.isBusy = 0;
		shoot.shoot_flag = 0;
	}
	last_status = shoot.trigger_status;
}
// 内部调用，射出子弹0'0
void shoot_set_trigger_location(int n)
{
	// 保证摩擦轮达到要求转速
	if (/*摩擦轮转速判断*/ shoot.SpeedUp < -4500 && /*卸力模式判断*/ Global.input.isOnForce == 1 /*拨弹盘卡弹判断*/) 
	{
		if(shoot.shoot_flag == 1)
		{
			rampInit(&shootRamp, shoot.trigger_location.set, shoot.trigger_location.set + A_BULLET_ANGEL, -1, 1);
			shoot.trigger_location.set += n * A_BULLET_ANGEL; // 拨弹盘转动
			shoot.trigger_location.last_now = shoot.trigger_location.now;
		}
		else if(shoot.shoot_flag == 0)
		{
			rampInit(&shootRamp, shoot.trigger_location.set, shoot.trigger_location.set + A_BULLET_ANGEL/30, -1, 1);
			shoot.trigger_location.set += n * A_BULLET_ANGEL/60; // 拨弹盘转动
			shoot.trigger_location.last_now = shoot.trigger_location.now;
			shoot.shoot_flag = 1;
		}
	}
}
int shoot_Bullets(int n)
{
	shoot.trigger_status = LOCATIONS;
	if(abs(get_motor_data(TRIGGER_MOTOR).given_current) < 2000)//防止卡弹多发
	{
		shoot_set_trigger_location(n);
	}
	return n;
	//	}
}		///在control_setting中使用
// end of file
