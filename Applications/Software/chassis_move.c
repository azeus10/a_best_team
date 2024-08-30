/**
 * @file chassis_move.c
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "DBUS_remote_control.h"
#include "vofa.h"
#include "math.h"
#include "pid.h"
#include "CAN_receive&send.h"
#include "chassis_move.h"
#include "referee_handle_pack.h"
#include "cap_ctl.h"
#include "Global_status.h"

// wheel conf
#define WHEEL_RADIUS 0.0875f // m
#define PI 3.1415926f

// car conf
#define ROLLER_DISTANCE 415 // mm  轴距
#define WHEELS_DISTANCE 300 // mm  轮距

float max_p;
float max_c;
double percentage;
float sssd = 9000;
struct chassis_status chassis;
struct cap cap_chassis; // 电容组
// mm/s
#define FR 0
#define FL 1
#define BL 2
#define BR 3
float wheel_mps[4];	 // 底盘速度数组
float Plimit = 1.0f; // 等比系数
int fly_mode = CLOSE;	 // 飞坡开关
float Power;
float CHASSIS_WZ_SET_SCALE = 0.0f; // 底盘转角速度设置比例
// 马达速度环PID
pid_t motor_speed[4];

// 初始化底盘
void chassis_move_init()
{
	// 左手坐标系
	// 前后 y
	// 左右 x
	chassis.speed.max_x = 2.0f;	   // m/s
	chassis.speed.max_y = 1000.0f; // m/s
	chassis.speed.max_r = 6.0f; //

	chassis.acc.max_x = 0.1f;  // 1m/^2  1
	chassis.acc.max_y = 0.1f;  //  m/^2   1
	chassis.acc.max_r = 10.0f; //

	pid_set(&motor_speed[FR], 8000, 0, 200, MAX_CURRENT, 3000); // 16000 1000
	pid_set(&motor_speed[FL], 8000, 0, 200, MAX_CURRENT, 3000);
	pid_set(&motor_speed[BL], 8000, 0, 200, MAX_CURRENT, 3000);
	pid_set(&motor_speed[BR], 8000, 0, 200, MAX_CURRENT, 3000);
}
// 限制值
inline void val_limit(float *val, float MAX)
{
	if (fabs(*val) > MAX)
	{
		if (*val > 0)
			*val = MAX;
		else
			*val = -MAX;
	}
}
// 限制变化量
inline void change_limit(float last, float *now, float limit)
{
	float change = *now - last;
	if (fabs(change) > limit)
	{
		if (change > 0)
			*now = last + limit;
		else
			*now = last - limit;
	}
}

// 软件功率控制函数
float now_p = 0.0f;
float b = 0.001f;
float e = 0.85f;
float a = 2.53999826e-07;  // 1.23e-07;	// k1
float k2 = 5.25299993e-06; // 1.453e-07; // k2
float power_limit(int16_t current[4])
{
	float max_p;
	if (cap.remain_vol <= 5)
		max_p = REFEREE_DATA.Chassis_Power_Limit - 2.0f; // 2w余量
	else if (cap.remain_vol > 5)
	{

		max_p = REFEREE_DATA.Chassis_Power_Limit + 14.0f * cap.remain_vol; // 超电最大功率 = 超电电压 * 14A 线圈最大电流
																		   //		}
																		   //	else
	}

	if (max_p >= REFEREE_DATA.Chassis_Power_Limit + 14.0f * cap.remain_vol)
		max_p = REFEREE_DATA.Chassis_Power_Limit + 14.0f * cap.remain_vol;
	//	}

	now_p = 0;

	const float constant = 4.081f;
	const float toque_coefficient = (20 / 16384) * (0.3) * (187 / 3591) / 9.55;

	for (int i = 0; i < 4; i++)
	{
		// 估算功率
		// 西交利物浦：https://github.com/MaxwellDemonLin/Motor-modeling-and-power-control/blob/master/chassis_power_control.c#L89
		now_p += (fabs(current[i] * toque_coefficient * get_motor_data(i).speed_rpm) +
				  fabs(k2 * get_motor_data(i).speed_rpm * get_motor_data(i).speed_rpm) +
				  fabs(a * current[i] * current[i] + constant)) /
				 e;
	}
	float percentage = max_p / now_p;

	if (percentage > 1.0f)
		return 1.0f;
	return percentage - b;
}
// 计算底盘马达速度
void chassis_moto_speed_calc()
{
	// 吊射模式下 底盘PD拉高一点达到锁死的效果
	if (fly_mode == OPEN)
	{
		if (Global.mode == LEAN_LOB)
			Global.mode = FLOW;//防止飞坡和吊射同时开启
		pid_set(&motor_speed[FR], 2, 0.1, 1.5, MAX_CURRENT, 3000); // 16000 1000
		pid_set(&motor_speed[FL], 1.5, 0.1, 1.5, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BL], 1.5, 0.1, 1.5, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BR], 2, 0.1, 1.5, MAX_CURRENT, 3000);
	}
	else if (Global.mode == LEAN_LOB)
	{
		if (fly_mode == OPEN)
			fly_mode == CLOSE;//防止飞坡和吊射同时开启
		pid_set(&motor_speed[FR], 16000, 0, 1000, MAX_CURRENT, 3000); // 16000 1000
		pid_set(&motor_speed[FL], 16000, 0, 1000, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BL], 16000, 0, 1000, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BR], 16000, 0, 1000, MAX_CURRENT, 3000);
	}
	else
	{
		pid_set(&motor_speed[FR], 8000, 0, 200, MAX_CURRENT, 3000); // 16000 1000
		pid_set(&motor_speed[FL], 8000, 0, 200, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BL], 8000, 0, 200, MAX_CURRENT, 3000);
		pid_set(&motor_speed[BR], 8000, 0, 200, MAX_CURRENT, 3000);
	}

	// 最大速度限制
	val_limit(&chassis.speed.x, chassis.speed.max_x);
	val_limit(&chassis.speed.y, chassis.speed.max_y);
	val_limit(&chassis.speed.r, chassis.speed.max_r);

	decode_as_3508(CAN_1_1);
	decode_as_3508(CAN_1_2);
	decode_as_3508(CAN_1_3);
	decode_as_3508(CAN_1_4);

	// 计算速度分量
	wheel_mps[FR] = +chassis.speed.x - chassis.speed.y + (1 + CHASSIS_WZ_SET_SCALE) * chassis.speed.r;
	wheel_mps[FL] = +chassis.speed.x + chassis.speed.y + (1 + CHASSIS_WZ_SET_SCALE) * chassis.speed.r;
	wheel_mps[BL] = -chassis.speed.x + chassis.speed.y + (1 - CHASSIS_WZ_SET_SCALE) * chassis.speed.r;
	wheel_mps[BR] = -chassis.speed.x - chassis.speed.y + (1 - CHASSIS_WZ_SET_SCALE) * chassis.speed.r;

	// 当前速度
	chassis.speed.now_x = wheel_mps[FL] / 2.0f - wheel_mps[BL] / 2.0f;
	chassis.speed.now_y = wheel_mps[FL] / 2.0f - wheel_mps[FR] / 2.0f;
	chassis.speed.now_r = wheel_mps[FR] / 2.0f + wheel_mps[BL] / 2.0f;

	if (fly_mode == 1)
	{
		chassis.wheel_current[FR] = pid_cal(&motor_speed[FR], (get_motor_data(chassis_FR).speed_rpm), 1727 * wheel_mps[FR] / (2 * PI * WHEEL_RADIUS)); //
		chassis.wheel_current[BR] = pid_cal(&motor_speed[BR], (get_motor_data(chassis_BR).speed_rpm), 1727 * wheel_mps[BR] / (2 * PI * WHEEL_RADIUS));
		chassis.wheel_current[FL] = pid_cal(&motor_speed[FL], (get_motor_data(chassis_FL).speed_rpm), 1727 * wheel_mps[FL] / (2 * PI * WHEEL_RADIUS));
		chassis.wheel_current[BL] = pid_cal(&motor_speed[BL], (get_motor_data(chassis_BL).speed_rpm), 1727 * wheel_mps[BL] / (2 * PI * WHEEL_RADIUS));
	}
	else
	{
		chassis.wheel_current[FR] = pid_cal(&motor_speed[FR], (get_motor_data(chassis_FR).speed_rpm) / 19.0f * 0.104719755 * WHEEL_RADIUS, wheel_mps[FR]);
		chassis.wheel_current[BR] = pid_cal(&motor_speed[BR], (get_motor_data(chassis_BR).speed_rpm) / 19.0f * 0.104719755 * WHEEL_RADIUS, wheel_mps[BR]);
		chassis.wheel_current[FL] = pid_cal(&motor_speed[FL], (get_motor_data(chassis_FL).speed_rpm) / 19.0f * 0.104719755 * WHEEL_RADIUS, wheel_mps[FL]);
		chassis.wheel_current[BL] = pid_cal(&motor_speed[BL], (get_motor_data(chassis_BL).speed_rpm) / 19.0f * 0.104719755 * WHEEL_RADIUS, wheel_mps[BL]);
	}
	Plimit = power_limit(chassis.wheel_current);

	// 	// 设定马达电流 （在freeRTOS中发送）
	// set_motor((chassis.wheel_current[BR]), chassis_BR);
	// set_motor((chassis.wheel_current[FL]), chassis_FL);
	// set_motor((chassis.wheel_current[FR]), chassis_FR);
	// set_motor((chassis.wheel_current[BL]), chassis_BL);
	if (fly_mode == 1)
	{
		set_motor((chassis.wheel_current[BR]), chassis_BR);
		set_motor((chassis.wheel_current[FL]), chassis_FL);
		set_motor((chassis.wheel_current[FR]), chassis_FR);
		set_motor((chassis.wheel_current[BL]), chassis_BL);
	}
	else
	{
		set_motor((Plimit * chassis.wheel_current[BR]), chassis_BR);
		set_motor((Plimit * chassis.wheel_current[FL]), chassis_FL);
		set_motor((Plimit * chassis.wheel_current[FR]), chassis_FR);
		set_motor((Plimit * chassis.wheel_current[BL]), chassis_BL);
	}

	chassis.speed.last_x = chassis.speed.now_x;
	chassis.speed.last_y = chassis.speed.now_y;
	chassis.speed.last_r = chassis.speed.now_r;
}

/*此函数用来按等级提升小陀螺转速*/
float chassis_spin_speed_level_up()
{
	float r_s = 0.0f;
	if (Global.cap == FULL)
	{
		if (REFEREE_DATA.Chassis_Power_Limit <= 55)
		{
			r_s = 2.0f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 55 && REFEREE_DATA.Chassis_Power_Limit <= 85)
		{
			r_s = 3.0f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 85 && REFEREE_DATA.Chassis_Power_Limit <= 100)
		{
			r_s = 4.0f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 100)
		{
			r_s = 5.5f;
		}
	}
	else
	{
		if (REFEREE_DATA.Chassis_Power_Limit <= 55)
		{
			r_s = 1.0f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 55 && REFEREE_DATA.Chassis_Power_Limit <= 85)
		{
			r_s = 1.5f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 85 && REFEREE_DATA.Chassis_Power_Limit <= 100)
		{
			r_s = 2.0f;
		}
		else if (REFEREE_DATA.Chassis_Power_Limit > 100)
		{
			r_s = 2.5f;
		}
	} //
	// 通信质量测试
	if (switch_is_down(RC_L_SW) && switch_is_mid(RC_R_SW))
	{
		r_s = -r_s;
	}
	return r_s;
}
