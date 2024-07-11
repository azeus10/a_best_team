/**
 * @file gimbal.h
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#define __GIMBAL_H__
#ifdef __GIMBAL_H__

#include "pid.h"
#include "stdint.h"

//云台电机数据
#define PITCH_MOTOR CAN_2_6
#define YAW_MOTOR CAN_1_5
#define SMALL_PITCH CAN_2_4

#define SCOPE_MOTOR CAN_2_7


enum gimbal_status_e
{
    LOCATION = 0,
    SPEED,
};
enum gimbal_ctrl_mode
{
	ECD=0,
	IMU,
	ECD_Cross_IMU,
};
struct gimbal_status
{
    //设定云台控制模式
    enum gimbal_status_e pitch_status;
    enum gimbal_status_e yaw_status;
	  //设定云台数据源
	  enum gimbal_ctrl_mode gimbal_source;
	  //设定图传控制模式
	  enum gimbal_status_e image_status;

    struct
    {
        float set,last_low, now,fist_set, last, offset;
        float stable;
    } pitch;
    struct
    {
        float set,last_low, now,speed,set_speed, last, offset;
        float stable;
        int state;
        int target;
    } small_pitch;
    float pitch_speed;
    float set_pitch_speed;

    struct
    {
        float set, now, last, offset;
        float stable;
    } yaw;
    float yaw_speed;
    float set_yaw_speed;

		 struct
    {
        float set, now, last, offset;
        float stable;
    } scope;
    float scope_speed;
    float set_scope_speed;
 
		float scope_pitch_ecd;
		
		float  scope_view_location;
		float  scope_view_speed;
};

extern struct gimbal_status gimbal;
extern pid_t yaw_imu_location_pid;

//extern pid_t pitch_speed_pid;

//外部调用
void gimbal_init(void);                         //初始化云台
void gimbal_set_offset(float pitch, float yaw); //初始化零点
void image_set_offset(void);                    //初始化
void gimbal_set(float pitch, float yaw); //设置角度
void gimbal_set_pitch(float pitch,float up_angle,float down_angle);
void image_set(void);                    //设定图传模式
void gimbal_set_speed(float pitch, float yaw); //设定速度
void gimbal_set_yaw_speed(float yaw);          //设定yaw速度

void gimbal_pid_cal(void); //云台PID计算
void gimbal_updata(void);  //更新云台数据
void gimbal_clear_cnt(void);
// end of file

#endif
