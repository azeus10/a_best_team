/**
 * @file gimbal.c
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#define USE_DJI60320_AS_PITCH_MOTOR//使用6020作为pitch电机

#include "Global_status.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "gimbal.h"
#include "RampFunc.h"
#include "control_setting.h"
#include <stdio.h>

#include "dm4310_drv.h"

#include "CAN_ID_Library.h"
struct gimbal_status gimbal;
int my_cnt = 0;
int Time_delay_duoji1 = 0;
// pitch轴
//pid_t pitch_speed_pid;
pid_t pitch_angle_pid;
// yaw
pid_t yaw_speed_pid;
pid_t yaw_location_pid;
// 瞄准镜的2006
pid_t scope_speed_pid;
pid_t scope_location_pid;
// 小云台的3508屁股
pid_t SAMALL_speed_pid;
pid_t SAMALL_location_pid;

float DM_Motor_PITCH_Set = 0;//云台与水平面的角度跟set的差值

int small_pitch_time_delay = 0; // 小云台延时用的

float imu2ecd_zero = 0.0f; // 因为吊射模式和普通模式切换时候，
float imu2ecd_err = 0.0f;  // 可能yaw有误差，通过这两个值补偿（现在陀螺仪漂的话也有误差）

// pitch轴限位函数
void gimbal_set_pitch(float up_angle, float down_angle)
{
	gimbal.pitch.up_angle = up_angle;
	gimbal.pitch.down_angle = down_angle;
}
// 云台初始化
void gimbal_init()
{
	gimbal_set_pitch(0.0880615711f, -0.928312063f); // 限位,英雄反过来的!!!,4310每次拆装后必须要改，不然会堵转卡死
	/*主云台*/
	pid_set(&yaw_speed_pid, 500, 0.00f, 0.0f, 29000.0f, 0.0f);
	pid_set(&yaw_location_pid, 14.0f, 0.0f, 0.0f, 29000.0f, 0.0f);
	
	pid_set(&pitch_angle_pid, 0.001f, 0.0f, 0.0f, 10.0f, 0.0f);
	
	gimbal.pitch.now = 0;
	gimbal.pitch.set = 0;
	gimbal.pitch.offset = 0;
	gimbal.pitch.stable = 0;
	gimbal.set_pitch_speed = 0;

	gimbal.yaw.now = 0;
	gimbal.yaw.set = 0;
	gimbal.yaw.offset = 0;
	gimbal.yaw.stable = 0;
	gimbal.set_yaw_speed = 0;

	/*副云台&瞄准镜*/
	pid_set(&scope_speed_pid, 40.0f, 0.0f, 0.0f, 3000.0f, 0.0f);
	pid_set(&scope_location_pid, 15.0f, 0.000f, 0.0f, 1000.0, 0.0f);

//	pid_set(&SAMALL_speed_pid, 7.0f, 0.0f, 1.0f, 10000.0f, 0.0f);  
//	pid_set(&SAMALL_location_pid, 22.0f, 0.0f, 18.0f, 1000.0, 0.0f);//先把小云台整个流程整完
//177
	pid_set(&SAMALL_speed_pid, 1.75f, 0.0f, 1.0f, 10000.0f, 0.0f);  //1.75
	pid_set(&SAMALL_location_pid, 1.8f ,0.0f, 18.0f, 1000.0, 0.0f);//先把小云台整个流程整完//1.8


	gimbal.scope.now = 0;
	gimbal.scope.set = 0;
	gimbal.scope.offset = 0;
	gimbal.scope.stable = 0;
	gimbal.scope_pitch_ecd = 0;
	gimbal.set_scope_speed = 0;

	gimbal.small_pitch.state = 1;//原先是0，我改为1
	gimbal.small_pitch.speed = 0;
	gimbal.small_pitch.set = 0;
	gimbal.small_pitch.now = 0;
	gimbal.small_pitch.offset = 0;

	gimbal.yaw_status = gimbal.pitch_status = LOCATION; // 默认为位置控制模式
 
	gimbal.gimbal_source = IMU;

}
// 用来设置yaw原点,pitch如果用纯陀螺仪就无所谓了，如果有编码器的话也需要这个零点
void gimbal_set_offset(float pitch, float yaw)
{
	gimbal.pitch.offset = pitch;
	gimbal.yaw.offset = yaw;
}
void gimbal_updata()
{	
	HT_small_pitch(); // 海泰电机小云台的控制都在这个函数里
	/*编码器*/
	if (gimbal.gimbal_source == ECD)
	{
		// yaw轴更新
		decode_as_6020(YAW_MOTOR);
		gimbal.yaw_speed = (get_motor_data(YAW_MOTOR).speed_rpm * 0.10472f);
		gimbal.yaw.now = degree2rad(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset);	///角度变弧度？

		// pitch轴更新
		decode_as_6020(PITCH_MOTOR);
		gimbal.pitch_speed = -IMU_data.gyro[1];
		gimbal.pitch.now = degree2rad(get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.offset);
	}
	/*陀螺仪*/
	else if (gimbal.gimbal_source == IMU)
	{
		// yaw轴更新
		// 角度制
		if (Global.mode == LEAN_LOB) // 这里特殊吊射模式下yaw是编码器控制
		{
			decode_as_6020(YAW_MOTOR);
			gimbal.yaw_speed = (get_motor_data(YAW_MOTOR).speed_rpm * 0.10472f);
			gimbal.yaw.now = degree2rad(get_motor_data(YAW_MOTOR).angle_cnt - imu2ecd_err);
		}
		else
		{
			gimbal.yaw_speed = (-cos(IMU_data.AHRS.pitch) * IMU_data.gyro[2] + sin(IMU_data.AHRS.pitch) * IMU_data.gyro[0]) * 360.0f / (2 * 3.1415926f);
			gimbal.yaw.now = -rad2degree(IMU_data.AHRS.yaw_rad_cnt);
		}

		// pitch轴更新
		// 弧度值
		decode_as_6020(PITCH_MOTOR);
		gimbal.pitch_speed = -IMU_data.gyro[1];
//		gimbal.pitch.now = -IMU_data.AHRS.pitch;
		gimbal.pitch.now = DM_motor_data[4].position;
	}
	/*编码器+陀螺仪*/
	else if (gimbal.gimbal_source == ECD_Cross_IMU)
	{
		// yaw轴更新
		decode_as_6020(YAW_MOTOR);
		gimbal.yaw_speed = (get_motor_data(YAW_MOTOR).ecd - get_motor_data(YAW_MOTOR).last_ecd) / ECD_MAX;
		gimbal.yaw.now = -rad2degree(IMU_data.AHRS.yaw_rad_cnt);
		// pitch轴更新
		decode_as_6020(PITCH_MOTOR);
		gimbal.pitch.now = degree2rad(get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.offset);
		gimbal.pitch_speed = -IMU_data.gyro[1];
	}
	// 副云台更新（3508屁股作为副云台）
	decode_as_3508_rotor(SMALL_PITCH);
	gimbal.small_pitch.now = get_motor_data(SMALL_PITCH).ecd_cnt;
	gimbal.small_pitch.speed = get_motor_data(SMALL_PITCH).speed_rpm;

	// 瞄准镜更新
	decode_as_2006(SCOPE_MOTOR);
	gimbal.scope.now = get_motor_data(SCOPE_MOTOR).angle_cnt;
	gimbal.scope_speed = get_motor_data(SCOPE_MOTOR).round_speed * 360.0f;
}

	
void gimbal_pid_cal()
{
	if(gimbal.pitch.set > gimbal.pitch.up_angle)//设置值限位
		gimbal.pitch.set = gimbal.pitch.up_angle;
	else if(gimbal.pitch.set < gimbal.pitch.down_angle)
		gimbal.pitch.set = gimbal.pitch.down_angle;

	/*副云台部分*/
	// 副云台设置零点
	//与之前主要的区别是之前是m3508的屁股，而现在是m3508的整个，加上了减速箱，就是从转子变成了输出轴，转子要很小的，而输出轴要很大
	//
	if (gimbal.small_pitch.state == OPEN) // 让这个函数只执行一次，当然写的挺呆的
	{
		gimbal.small_pitch.set -= 100000;			//10度是20000，10000	// 上电后让小云台往后转
		if (fabs(fabs(get_motor_data(SMALL_PITCH).speed_rpm) <= 0)) // 如果转不动了
		{
			small_pitch_time_delay++; // 伪逻辑延时，（不够规范）
		}
		if (small_pitch_time_delay > 100&& fabs(get_motor_data(SMALL_PITCH).speed_rpm) <= 0) // 延时后如果还是转不动说明到位了，
		{
			gimbal.small_pitch.offset = get_motor_data(SMALL_PITCH).ecd_cnt+32000 ; // 设置水平为零点，//应该调节大一点，让他保持水平30000，55度
			//从他自己的机械零点变成实际生活中的水平零点，再进行所需求的角度
			gimbal.small_pitch.set = gimbal.small_pitch.offset;					   // 设置当前为水平位置
			gimbal.small_pitch.state = CLOSE;									   // 保证函数只在上电时候执行一次
		}
	}
	
//	if (Get_sys_time_ms() -Time_delay_duoji1 <= 1000)
//	{
//		
//	}
//     else if(Get_sys_time_ms()-Time_delay_duoji1 > 1000) 
//	{
//		
//		if(Get_sys_time_ms() -Time_delay_duoji1> 2000) //135
//		{
//			Time_delay_duoji1 =Get_sys_time_ms();
//		}
//		else
//		{
//		
//		}
//	}
	if(Get_sys_time_ms() -Time_delay_duoji1> 2000)
	{
		Time_delay_duoji1 =Get_sys_time_ms();
	}
	
	
	
	if(HAL_GPIO_ReadPin(KEY_GPIO_Port ,KEY_Pin) == GPIO_PIN_RESET)
	{
		if((Get_sys_time_ms() -Time_delay_duoji1)>200)
		{
			my_cnt += 1;
			Time_delay_duoji1 =Get_sys_time_ms();
		}
		
	}
	if(my_cnt == 1)
	{
		gimbal.small_pitch.set = gimbal.small_pitch.offset - 32000/55*15;
	}
	if(my_cnt == 2)
	{
		gimbal.small_pitch.set = gimbal.small_pitch.offset - 32000/55*30;
	}
	if(my_cnt == 3)
	{
		gimbal.small_pitch.set = gimbal.small_pitch.offset - 32000/55*45;
	}
	if(my_cnt > 3)
	{
		my_cnt = 1;
	}
	// 副云台pid计算,主要用来设置副云台要的俯仰角
	if (gimbal.small_pitch.state == CLOSE)
	{
		if (Global.mode == LEAN_LOB && Global.input.ScopeisOpen == 1 && IMU_data.AHRS.pitch * 57.3f < 20)
		{
			gimbal.small_pitch.set += 5;//	///这个和下面参数都得重新调
			if (gimbal.small_pitch.set > (gimbal.small_pitch.offset + 500)) // 打前哨
				gimbal.small_pitch.set = gimbal.small_pitch.offset + 500;
		}
		else if (Global.mode == LEAN_LOB && Global.input.ScopeisOpen == 1 && IMU_data.AHRS.pitch * 57.3f < 28 && IMU_data.AHRS.pitch * 57.3f >= 20) // 打前哨站
		{
			gimbal.small_pitch.set += 5;
			if (gimbal.small_pitch.set > (gimbal.small_pitch.offset + 850))
				gimbal.small_pitch.set = gimbal.small_pitch.offset + 850;
		}
		else if (Global.mode == LEAN_LOB && Global.input.ScopeisOpen == 1 && IMU_data.AHRS.pitch * 57.3f >= 30) // 打基地
		{
			gimbal.small_pitch.set += 5;
			if (gimbal.small_pitch.set > (gimbal.small_pitch.offset + 1650))
				gimbal.small_pitch.set = gimbal.small_pitch.offset + 1650;
		}
		else // 正常水平看前边
		{
			gimbal.small_pitch.set = gimbal.small_pitch.offset;
		}
	}
	// 计算设置电流值并发给副云台电机
	gimbal.small_pitch.set_speed = pid_cal(&SAMALL_location_pid, gimbal.small_pitch.now, gimbal.small_pitch.set);
	set_motor(pid_cal(&SAMALL_speed_pid, gimbal.small_pitch.speed, gimbal.small_pitch.set_speed), SMALL_PITCH);
	//
	
	set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);

	/*主云台部分*/
	// 吊射和不吊射用两套pid
	if (Global.mode != LEAN_LOB)
	{
		pid_set(&yaw_speed_pid, 500, 0.00f, 0.0f, 27000.0f, 0.0f);
		pid_set(&yaw_location_pid, 14.0f, 0.0f, 0.0f, 27000.0f, 0.0f);

	}
	else if (Global.mode == LEAN_LOB)
	{
		pid_set(&yaw_speed_pid, 300, 0.0f, 400.0f, 25000.0f, 0.0f);
		pid_set(&yaw_location_pid, 1800.0f, 0.0f, 7400, 500.0f, 0.0f);

	}
	// 根据电机所依赖的不同数据源，进行不同的计算
	// 数据来源为编码器
	if (gimbal.gimbal_source == ECD)
	{
		// 位置环
		if (gimbal.yaw_status == LOCATION)
		{
			gimbal.set_yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);	///gimbal.yaw.set在control_setting.c中能找到
		}																						///gimbal.yaw.now在gimbal.c中找吧
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;	///119行定义了

		if (gimbal.pitch_status == LOCATION)
		{
//			gimbal.set_pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;	
			gimbal.set_scope_speed = gimbal.scope_speed;
		}
		// 速度环
		set_motor(pid_cal(&yaw_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
//		set_motor(pid_cal(&pitch_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);
	}
	// 数据来源为陀螺仪
	else if (gimbal.gimbal_source == IMU)
	{

		if (gimbal.yaw_status == LOCATION)
		{
			if (Global.mode == LEAN_LOB)
			{
				gimbal.set_yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, degree2rad(gimbal.yaw.set - imu2ecd_zero));
			}
			else
			{
				gimbal.set_yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
				imu2ecd_zero = gimbal.yaw.set;
				imu2ecd_err = get_motor_data(YAW_MOTOR).angle_cnt;	///误差补偿
			}
		}
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;	///

		if (gimbal.pitch_status == LOCATION)
		{
			/*对gimbal.pitch.set进行低通滤波  尝试减少微调的抖动*/
			gimbal.pitch.fist_set = lowPassFilter(gimbal.pitch.set, gimbal.pitch.last_low, 0.8f);
//			gimbal.set_pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.fist_set);
			gimbal.pitch.last_low = gimbal.set_pitch_speed;
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);		

//			DM_Motor_PITCH_Set = pid_cal(&pitch_angle_pid, IMU_data.AHRS.pitch, gimbal.pitch.set);
//			gimbal.pitch.set += pid_cal(&pitch_angle_pid, IMU_data.AHRS.pitch, gimbal.pitch.set);
			if(DM_Motor_PITCH_Set> gimbal.pitch.up_angle)//达妙电机pitch陀螺仪输出值限位
				DM_Motor_PITCH_Set = gimbal.pitch.up_angle;
			else if(DM_Motor_PITCH_Set < gimbal.pitch.down_angle)
				DM_Motor_PITCH_Set = gimbal.pitch.down_angle;
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;		///gimbal.pitch_speed数据来源于IMU
			gimbal.set_scope_speed = gimbal.scope_speed;		///gimbal.scope_speed数据就在gimbal.c中计算得到
		}

		// 速度环

		set_motor(pid_cal(&yaw_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
//		set_motor(0, YAW_MOTOR);
//		DM_Motor_set(PITCH_MOTOR,DM_Motor_PITCH_Set,20);
		DM_Motor_set(PITCH_MOTOR,gimbal.pitch.set,20);
		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);
	}
	// 数据来源为编码器+陀螺仪
	else if (gimbal.gimbal_source == ECD_Cross_IMU)
	{
		if (gimbal.yaw_status == LOCATION)
		{
			gimbal.set_yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
		}
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;

		if (gimbal.pitch_status == LOCATION)
		{
//			gimbal.set_pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;
			gimbal.set_scope_speed = gimbal.scope_speed;
		}
		// 速度环
		set_motor(pid_cal(&yaw_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);

		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);
	}

}
