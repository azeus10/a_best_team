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
//预置标志位
uint8_t  flag_presets=0;
//预置量
uint32_t presets_ecd_cnt=21296;
static enum trigger_status_e last_status = SPEEDS;

// 子弹计数
static int BulletCnt = 0;
// 初始化
void shoot_init()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	pid_set(&shoot1_speed_pid, 25, 0, 0.0, 3000, 0.0); 
	pid_set(&shoot2_speed_pid, 25, 0, 0.0, 3000, 0.0);
	pid_set(&shoot3_speed_pid, 25, 0, 0.0, 3000, 0.0);
#endif
	
	pid_set(&trigger_speed_pid, 3, 0, 63, 15000, 0);	 //16000
	pid_set(&trigger_location_pid, 0.5, 0, 0.0, 16000, 0); 
  
	shoot.trigger_status = SPEEDS;
	shoot.last_status = SPEEDS;
	shoot.trigger_location.set = 0;
	shoot.trigger_location.now = 0;
	shoot.SetSpeedUp = 0;
	shoot.SetSpeedLeft = 0;
	shoot.SetSpeedRight = 0;
	shoot.compensate_angle=85;
	shoot.speedUpLevel   = 5300;
	shoot.speedRightLevel= 5300;
	shoot.speedLeftLevel = 5300;
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
#else
	// 适配其他拨弹电机
	PWM_snaill_set(PIN_2, (uint16_t)speed);
	PWM_snaill_set(PIN_3, (uint16_t)speed);
#endif
}
void shoot_pid_cal()
{
//正常模式 无电控预置部分
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
		shoot.trigger_speed = shoot.set_trigger_speed;
	}

	// 速度环
	if(Global.input.isOnForce==1)
	{
		set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.trigger_speed), TRIGGER_MOTOR);
		
	}
	//卸力模式
	else if(Global.input.isOnForce==0)
	{
		set_motor(0,TRIGGER_MOTOR);
	}
	
	last_status = shoot.trigger_status;	

////电控预置弹丸部分 需要测试
//	// 摩擦轮设定
//	decode_as_3508(TRIGGER_MOTOR);
//	shoot_set_shoot_Motor_speed((float)(shoot.SetSpeedUp), (float)(shoot.SetSpeedLeft), (float)(shoot.SetSpeedRight));

//	// 拨弹电机设定
//	//拨弹电机电控预置程序
//	if(flag_presets==0)
//	{
//		shoot.set_trigger_speed = +10000;
//		if(fabs(get_motor_data(TRIGGER_MOTOR).given_current)>15000&&(fabs(get_motor_data(TRIGGER_MOTOR).speed_rpm)<=0))
//		{
//			//获取当前位置
//			shoot.trigger_location.set += shoot.trigger_location.now;
//			//设定拨弹电机控制模式为位置控制
//			shoot.trigger_status =LOCATIONS;
//			//退出预置程序
//			flag_presets=1;
//		}
//	}
//	//PID串级
//	if (shoot.trigger_status == LOCATIONS) // 位置控制
//	{
//		if (shoot.last_status == SPEEDS)
//		{
//		  //设定预置量
//			shoot.trigger_location.set -= presets_ecd_cnt;
//			shoot.set_trigger_speed =0;
//			rampInit(&shootRamp,shoot.trigger_location.now,shoot.trigger_location.set,-600,1);
//			//自动退出
//			shoot.last_status = LOCATIONS;
//		}
//		//斜坡函数计算
//		rampIterate(&shootRamp);
//		shoot.trigger_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shootRamp.currentValue);
//	}
//	else if (shoot.trigger_status == SPEEDS) // 速度控制
//	{
//		shoot.trigger_speed = shoot.set_trigger_speed;
//	}

//	// 速度环
//	if(Global.input.isOnForce==1)
//	{
//		set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.trigger_speed), TRIGGER_MOTOR);
//	}
//	//卸力模式
//	else if(Global.input.isOnForce==0)
//	{
//		set_motor(0,TRIGGER_MOTOR);
//		
//	}
}
// 内部调用，射出子弹0'0
void shoot_set_trigger_location(int n)
{
	// 保证摩擦轮达到要求转速
	if( /*摩擦轮转速判断*/shoot.SpeedUp < -4500&&/*卸力模式判断*/Global.input.isOnForce==1/*拨弹盘卡弹判断*/)//&&(!(fabs(get_motor_data(TRIGGER_MOTOR).given_current)>=14800&&(fabs(get_motor_data(TRIGGER_MOTOR).speed_rpm)<=+0))))
	{
	
			// 补偿位置误差
			// if (BulletCnt % 2 == 0)
			// {
			// shoot.trigger_location.set -= shoot.compensate_angle * 19;
			// }
			//斜坡结构体初始化
			rampInit(&shootRamp,shoot.trigger_location.set,shoot.trigger_location.set+A_BULLET_ANGEL,-1,1);
			shoot.trigger_location.set += n * A_BULLET_ANGEL;//拨弹盘转动
			// 子弹计数
			BulletCnt++;
			shoot.trigger_location.last_now = shoot.trigger_location.now;
	}
}
// 强制停止射击
void shoot_stop()
{
	shoot.trigger_location.set = shoot.trigger_location.now;
}
// 发射N颗子弹
int shoot_Bullets(int n)
{
	shoot.trigger_status = LOCATIONS;
//	if(Global.input.isHeatLimit==0)
//	{
//		//如果剩余热量小于100 需要做UI显示
//	  if (REFEREE_DATA.Heat_Limit - REFEREE_DATA.Barrel_Heat <= 100)//&&REFEREE_DATA.Projectile_allowance_42mm<=0
//	  {
//		return 0;
//	  }
//	  else
//	 {
//		shoot_set_trigger_location(n);
//		return n;
//	 }
//	}
//	 else if(Global.input.isHeatLimit==1)
//	{
		shoot_set_trigger_location(n);
		return n;
//	}
}
// 重设子弹数目
void shoot_reload()
{
	shoot.remainingBullets = FULL_BULLETS;
}
// end of file
