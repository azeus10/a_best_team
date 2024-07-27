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

#include "Global_status.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "gimbal.h"
#include "RampFunc.h"
#include "control_setting.h"
#include <stdio.h>
struct gimbal_status gimbal;

/*编码器*/
pid_t pitch_ecd_speed_pid;
pid_t pitch_ecd_location_pid;

pid_t yaw_ecd_speed_pid;
pid_t yaw_ecd_location_pid;
/*陀螺仪*/
pid_t pitch_imu_speed_pid;
pid_t pitch_imu_location_pid;

pid_t yaw_imu_speed_pid;
pid_t yaw_imu_location_pid;
/*编码器+陀螺仪*/
pid_t pitch_ecd_cross_imu_speed_pid;
pid_t pitch_ecd_cross_imu_location_pid;

pid_t yaw_ecd_cross_imu_speed_pid;
pid_t yaw_ecd_cross_imu_location_pid;

pid_t scope_speed_pid;
pid_t scope_location_pid;

pid_t SAMALL_speed_pid;
pid_t SAMALL_location_pid;
int flag_Scope_init=0;
int small_pitch_time_delay;
float imu2ecd_zero = 0.0f;
float imu2ecd_err = 0.0f;
// 云台初始化
void gimbal_init()
{

	/*主云台*/
	/*编码器控制参数*/
	pid_set(&yaw_ecd_speed_pid, 300, 0.0f, 400.0f, 25000.0f, 0.0f);
	pid_set(&yaw_ecd_location_pid, 1800.0f, 0.0f, 7400, 500.0f, 0.0f);

	pid_set(&pitch_ecd_speed_pid, 10000.0f,10.0f, 1400.0f, 30000.0f, 0.0f);
	pid_set(&pitch_ecd_location_pid, 17.0f, 0.0f, 2.0f, 100.0, 0.0f);
	/*陀螺仪控制参数*/
	pid_set(&yaw_imu_speed_pid, 500, 0.00f, 0.0f, 29000.0f, 0.0f);  //吊射 400 0  0 29000
	pid_set(&yaw_imu_location_pid,14.0f, 0.0f, 0.0f,29000.0f, 0.0f);//30 0 0 0 29000

	// pid_set(&pitch_imu_speed_pid,800.0f, 0.0f,2500.0f,29000.0f, 0.0f);     //可以吊射用
	// pid_set(&pitch_imu_location_pid, 1500.0f, 0.0f, 20000.0f, 100.0, 0.0f);//
	pid_set(&pitch_imu_speed_pid,800.0f, 0.0f,2500.0f,29000.0f, 0.0f);     //平时用
	pid_set(&pitch_imu_location_pid, 300.0f, 0.0f, 35000.0f, 100.0, 0.0f);//
	/*编码器+陀螺仪控制参数*/
	pid_set(&yaw_ecd_cross_imu_speed_pid,240, 0.00f, 1700.0f, 0.0f, 0.0f);
	pid_set(&yaw_ecd_cross_imu_location_pid, 18.0f, 0.0f, 0, 30000.0f, 0.0f);

	pid_set(&pitch_ecd_cross_imu_speed_pid, 10000.0f, 0.0f, 1400.0f, 30000.0f, 10.0f);
	pid_set(&pitch_ecd_cross_imu_location_pid, 17.0f, 0.000f, 2.0f, 100.0, 0.0f);
	/*副云台*/
	//用于初始化的PID
	pid_set(&scope_speed_pid, 40.0f, 0.0f, 0.0f, 3000.0f, 0.0f);
	pid_set(&scope_location_pid, 15.0f, 0.000f, 0.0f, 1000.0, 0.0f);

	pid_set(&SAMALL_speed_pid, 7.0f, 0.0f, 1.0f, 10000.0f, 0.0f);
	pid_set(&SAMALL_location_pid, 22.0f, 0.0f, 18.0f, 1000.0, 0.0f);

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

	gimbal.scope.now = 0;
	gimbal.scope.set = 0;
	gimbal.scope.offset = 0;
	gimbal.scope.stable = 0;
	gimbal.set_scope_speed = 0;

	gimbal.small_pitch.state = 0;
	gimbal.small_pitch.speed = 0;
	gimbal.small_pitch.set = 0;
	gimbal.small_pitch.now = 0;
	gimbal.small_pitch.offset = 0;

	gimbal.scope_pitch_ecd = 0;

	gimbal.yaw_status = gimbal.pitch_status = LOCATION; // 默认为位置控制模式
	gimbal.image_status = LOCATION;
	// 云台数据来源默认由编码器和陀螺仪共同提供
	gimbal.gimbal_source = IMU;
}

void gimbal_set_offset(float pitch, float yaw)
{
	gimbal.pitch.offset = pitch;
	gimbal.yaw.offset = yaw;
}
void gimbal_updata()
{
	/*编码器*/
	if (gimbal.gimbal_source == ECD)
	{
		// yaw轴更新
		decode_as_6020(YAW_MOTOR);
    	gimbal.yaw_speed =  (get_motor_data(YAW_MOTOR).speed_rpm * 0.10472f);
		gimbal.yaw.now   =  degree2rad(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset);

		// pitch轴更新
		decode_as_6020(PITCH_MOTOR);
		gimbal.pitch_speed = -IMU_data.gyro[1];
		gimbal.pitch.now = degree2rad(get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.offset);

	}
	
	/*陀螺仪*/
	else if (gimbal.gimbal_source == IMU)
	{
		// yaw轴更新
		//角度制
		if(Global.mode==LEAN_LOB)
		{
			decode_as_6020(YAW_MOTOR);
			gimbal.yaw_speed =  (get_motor_data(YAW_MOTOR).speed_rpm * 0.10472f);
			gimbal.yaw.now   =  degree2rad(get_motor_data(YAW_MOTOR).angle_cnt - imu2ecd_err);
		}
		else
		{
			gimbal.yaw_speed =   (-cos(IMU_data.AHRS.pitch) * IMU_data.gyro[2] +sin(IMU_data.AHRS.pitch) * IMU_data.gyro[0])*360.0f/(2*3.1415926f);
			gimbal.yaw.now   =   -rad2degree(IMU_data.AHRS.yaw_rad_cnt);
		}
		
		
		//pitch轴更新
		decode_as_6020(PITCH_MOTOR);
		//角度制
	    // gimbal.pitch_speed = (-IMU_data.gyro[1]*360.0f/(2*3.1415926f));
		//  gimbal.pitch.now   =get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.offset;
	    //gimbal.pitch.now   = (-IMU_data.AHRS.pitch)*360.0f/(2*3.1415926f);
		//弧度值
		gimbal.pitch_speed = -IMU_data.gyro[1];
		gimbal.pitch.now   = -IMU_data.AHRS.pitch;
		// gimbal.pitch.now   =degree2rad(get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.offset);
		
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
	// 副云台更新  前哨站变化910   基地变化1820
	decode_as_3508_rotor(SMALL_PITCH);
	gimbal.small_pitch.now = get_motor_data(SMALL_PITCH).ecd_cnt;
	gimbal.small_pitch.speed = get_motor_data(SMALL_PITCH).speed_rpm;

	//瞄准镜更新
	decode_as_2006(SCOPE_MOTOR);
	gimbal.scope.now=get_motor_data(SCOPE_MOTOR).angle_cnt;
	gimbal.scope_speed=get_motor_data(SCOPE_MOTOR).round_speed*360.0f;
	
}

// 设定角度
void gimbal_set(float pitch, float yaw)
{
	gimbal.yaw_status = gimbal.pitch_status = LOCATION; // 以位置模式控制

	// 范围限定 防止超出机械限位
	if (fabs(pitch) < 1.0f)
		gimbal.pitch.set = pitch;
	gimbal.yaw.set = yaw;
}
void gimbal_set_pitch(float pitch,float up_angle,float down_angle)
{
  //角度值
  if (pitch >up_angle)
  {
	gimbal.pitch.set = up_angle;
  }
  else if(pitch< -down_angle)
  {
	gimbal.pitch.set = -down_angle;
  }
}

// 设定速度
void gimbal_set_speed(float pitch, float yaw)
{
	gimbal.yaw_status = gimbal.pitch_status = SPEED; // 以速度模式控制

	gimbal.set_pitch_speed = pitch;
	gimbal.set_yaw_speed = yaw;
}
void gimbal_set_yaw_speed(float yaw)
{
	gimbal.yaw_status = SPEED; // 以速度模式控制

	gimbal.set_yaw_speed = yaw;
}
static float last_set;
void gimbal_pid_cal()
{
//	if(flag_Scope_init==0)
//	{
//		gimbal.scope.set=+655535;
//		if(fabs(get_motor_data(SCOPE_MOTOR).given_current)>=2000&&fabs(get_motor_data(SCOPE_MOTOR).speed_rpm)<=0)
//		{
//			//获取当前零点
//			gimbal.scope.offset=get_motor_data(SCOPE_MOTOR).angle_cnt;
//			gimbal.scope.set=gimbal.scope.offset;
//			flag_Scope_init=1;
//		}
//	}

	if(Global.mode!=LEAN_LOB&&Global.input.vision_status==0)
	 {
 	pid_set(&yaw_imu_speed_pid, 500, 0.00f, 0.0f, 29000.0f, 0.0f);  //吊射 400 0  0 29000
                                                               //30 0 0 0 29000
	pid_set(&yaw_imu_location_pid,14.0f, 0.0f, 0.0f,29000.0f, 0.0f);

	pid_set(&pitch_imu_speed_pid,400.0f, 0.0f, 15000.0f,29000.0f, 0.0f); //200 0 0 29000
	pid_set(&pitch_imu_location_pid, 900.0f, 0.0f, 5000.0f, 100.0, 0.0f);//75 0 0 600
	 }
	// else if(Global.mode!=LEAN_LOB&&Global.input.vision_status==1)
	// {
	// pid_set(&yaw_imu_speed_pid, 500, 0.00f, 0.0f, 29000.0f, 0.0f);  //吊射 400 0  0 29000
    //                                                            //30 0 0 0 29000
	// pid_set(&yaw_imu_location_pid,20.0f, 0.0f, 0.0f,29000.0f, 0.0f);

	//  pid_set(&pitch_imu_speed_pid,400.0f, 0.0f, 15000.0f,29000.0f, 0.0f); //200 0 0 29000
	// pid_set(&pitch_imu_location_pid, 900.0f, 0.0f, 5000.0f, 100.0, 0.0f);//75 0 
	
	//  }
	 else if(Global.mode==LEAN_LOB)
	 {
	 pid_set(&yaw_imu_speed_pid, 800, 0.00f, 2500.0f, 29000.0f, 0.0f);                                                                
	pid_set(&yaw_imu_location_pid,30.0f, 0.0f, 0.0f,29000.0f, 0.0f);

	pid_set(&pitch_imu_speed_pid,800.0f, 0,2500.0f,29000.0f, 20000.0f);     //可以吊射用
	pid_set(&pitch_imu_location_pid, 1700.0f, 0.0f, 20000.0f, 100.0, 0.0f);//
	 }
	gimbal_set_pitch(gimbal.pitch.set,0.27555f,0.7655f);//限位,英雄反过来的
	//副云台设置零点    前哨站变化910   基地变化1820
	if(gimbal.small_pitch.state==0)
	{
		gimbal.small_pitch.set -=100;
		if(fabs(fabs(get_motor_data(SMALL_PITCH).speed_rpm) <= 0))
		{
			small_pitch_time_delay ++;
		}
		if(small_pitch_time_delay > 10 && fabs(get_motor_data(SMALL_PITCH).speed_rpm) <= 0)
		{
			gimbal.small_pitch.offset=get_motor_data(SMALL_PITCH).ecd_cnt + 446;
			gimbal.small_pitch.set=gimbal.small_pitch.offset;
			gimbal.small_pitch.state=1;
		}
		

	}
	// 副云台pid计算
	if(gimbal.small_pitch.state == 1)
	{
		if(Global.mode==LEAN_LOB && Global.input.ScopeisOpen==1 && IMU_data.AHRS.pitch*57.3f < 30)//打前哨站
		{
			gimbal.small_pitch.set += 5; 
			if(gimbal.small_pitch.set > (gimbal.small_pitch.offset + 850))
				gimbal.small_pitch.set = gimbal.small_pitch.offset + 850;
		}
		else if(Global.mode==LEAN_LOB && Global.input.ScopeisOpen==1 && IMU_data.AHRS.pitch*57.3f >= 30)//打基地
		{
			gimbal.small_pitch.set += 5; 
			if(gimbal.small_pitch.set > (gimbal.small_pitch.offset + 1820))
				gimbal.small_pitch.set = gimbal.small_pitch.offset + 1820;
		}
		else//正常看前边
		{
			gimbal.small_pitch.set = gimbal.small_pitch.offset;
		}
	}
	gimbal.small_pitch.set_speed = pid_cal(&SAMALL_location_pid, gimbal.small_pitch.now , gimbal.small_pitch.set);
	set_motor(pid_cal(&SAMALL_speed_pid, gimbal.small_pitch.speed, gimbal.small_pitch.set_speed), SMALL_PITCH);
	if (gimbal.gimbal_source == ECD)
	{
		//位置环
		if (gimbal.yaw_status == LOCATION)
		{   
			gimbal.set_yaw_speed = pid_cal(&yaw_ecd_location_pid, gimbal.yaw.now, gimbal.yaw.set);
		}
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;

		if (gimbal.pitch_status == LOCATION)
		{
			gimbal.set_pitch_speed = pid_cal(&pitch_ecd_location_pid, gimbal.pitch.now, gimbal.pitch.set);
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;
			gimbal.set_scope_speed = gimbal.scope_speed;
		}
    //速度环
		set_motor(pid_cal(&yaw_ecd_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
		set_motor(pid_cal(&pitch_ecd_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);

	}
	if (gimbal.gimbal_source == IMU)
	{
		
		if (gimbal.yaw_status == LOCATION)
		{
			if(Global.mode==LEAN_LOB)
			{
				gimbal.set_yaw_speed = pid_cal(&yaw_ecd_location_pid, gimbal.yaw.now, degree2rad(gimbal.yaw.set - imu2ecd_zero));
			}
			else
			{
				gimbal.set_yaw_speed = pid_cal(&yaw_imu_location_pid, gimbal.yaw.now, gimbal.yaw.set);
				imu2ecd_zero = gimbal.yaw.set;
				imu2ecd_err = get_motor_data(YAW_MOTOR).angle_cnt;
			}
			
		}
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;

		if (gimbal.pitch_status == LOCATION)
		{
			/*对gimbal.pitch.set进行低通滤波  尝试减少微调的抖动*/
			gimbal.pitch.fist_set = lowPassFilter(gimbal.pitch.set,gimbal.pitch.last_low,0.8f);
			gimbal.set_pitch_speed = pid_cal(&pitch_imu_location_pid, gimbal.pitch.now,gimbal.pitch.fist_set);
			gimbal.pitch.last_low = lowPassFilter(gimbal.pitch.set,gimbal.pitch.last_low,0.8f);
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;
			gimbal.set_scope_speed = gimbal.scope_speed;
		}

		// // 速度环
		if(Global.mode==LEAN_LOB)
		{
			set_motor(pid_cal(&yaw_ecd_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
		}
		else
		{
			set_motor(pid_cal(&yaw_imu_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
		}
		set_motor(pid_cal(&pitch_imu_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);
	}
	if (gimbal.gimbal_source == ECD_Cross_IMU)
	{
		if (gimbal.yaw_status == LOCATION)
		{
			gimbal.set_yaw_speed = pid_cal(&yaw_ecd_cross_imu_location_pid, gimbal.yaw.now, gimbal.yaw.set);
		}
		else
			gimbal.set_yaw_speed = gimbal.yaw_speed;

		if (gimbal.pitch_status == LOCATION)
		{
			gimbal.set_pitch_speed = pid_cal(&pitch_ecd_cross_imu_location_pid, gimbal.pitch.now, gimbal.pitch.set);
			gimbal.set_scope_speed = pid_cal(&scope_location_pid, gimbal.scope.now, gimbal.scope.set);
		}
		else
		{
			gimbal.set_pitch_speed = gimbal.pitch_speed;
			gimbal.set_scope_speed = gimbal.scope_speed;
		}

		// 速度环
		set_motor(pid_cal(&yaw_ecd_cross_imu_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed), YAW_MOTOR);
		set_motor(pid_cal(&pitch_ecd_cross_imu_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		set_motor(pid_cal(&scope_speed_pid, gimbal.scope_speed, gimbal.set_scope_speed), SCOPE_MOTOR);
	}

	last_set = gimbal.pitch.set;
 }

void gimbal_clear_cnt(void)
{
	clear_motor_cnt(YAW_MOTOR);
}