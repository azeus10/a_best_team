#include "usbd_cdc_if.h"
#include "stdlib.h"
#include "CAN_receive&send.h"
#include "DBUS_remote_control.h"
#include "LED_control.h"
#include "IMU_updata.h"
#include "PWM_control.h"
#include "Stm32_time.h"
#include "CMSIS_OS2.H"

#include "chassis_move.h"
#include "gimbal.h"
#include "shoot.h"
#include "math.h"
#include "USB_VirCom.h"

#include "Global_status.h"
#include "Error_detect.h"
#include "NUC_communication.h"
#include "control_setting.h"
#include "RM_Cilent_UI.h"
#include "cap_ctl.h"
#include "referee_handle_pack.h"
#include "referee_usart_task.h"
#include "fifo.h"
#include "AHRS_MiddleWare.h"
#include "CAN_receive&send.h"
#include "LK_motor_process.h"

// 消抖
uint32_t Time_delay_LEAN = 0;
int Time_delay_mouse1 = 0;
uint32_t Time_delay_LOB = 0;
uint32_t Time_delay_LOB_OUT = 0;
uint32_t Time_delay_friction_wheel = 0;
uint32_t Time_delay_switch = 0;
uint32_t Time_delay_press_Z = 0;
uint32_t Time_delay_press_C_Ctrl = 0;
uint32_t Time_delay_press_C = 0;
uint32_t Time_delay_press_B = 0;
uint32_t Time_delay_press_G_CTRL = 0;
uint32_t Time_delay_press_G = 0;
uint32_t Time_delay_press_E = 0;
uint32_t Time_delay_press_V = 0;
uint32_t Time_delay_press_X = 0;
uint32_t Time_delay_press_Q = 0;
uint32_t Time_delay_press_F_CTRL = 0;
uint32_t Time_delay_press_F = 0;
uint32_t Time_delay_press_R = 0;
uint32_t Time_delay_tick_left = 0;
uint32_t Time_delay_press_X_CTRL = 0;
// 定义一个函数用来消抖
bool key_delay_ms(uint16_t time, int key, bool if_pess)
{
	if (if_pess)
	{
		static uint32_t delay_time[30] = {0};
		if (delay_time[key] - Get_sys_time_ms() > time)
		{
			delay_time[key] = Get_sys_time_ms();
			return true;
		}
		else
			return false;
	}
}
// 微调量
float tweaks_value = 0.01f;
uint32_t last_shoot_time = 0;
// 步长
float pressW_step = 0.001f;
float pressS_step = 0.005f;
float pressA_step = 0.001f;
float pressD_step = 0.001f;
float pressShift_step = 0.01f;
// 灵敏度
// 遥控器
float PitchCofficientFromRC = 2000000.0f;
float YawCofficientFromRC = 600.0f;
// 自瞄
float PitchCofficientFromNUC = -1500.0f;
float YawCofficientFromNUC = 20.0f;
// 客户端
float PitchCofficientFromPC = -12000.0f;
float YawCofficientFromPC = 120.0f;
float PitchLobCofficientFromPC = -4600000.0f;
float max_cap_speed;

// 全局状态机初始化
void Global_status_init()
{
	Global.cap = STOP;
	Global.mode = LOCK;
	Global.input.ctl = RC;
	Global.input.isHeatLimit = 0;
	Global.input.isOnForce = 1;
	Global.input.pitch = 0;
	Global.input.yaw = 0;
	Global.input.x = 0;
	Global.input.y = 0;
	Global.input.r = 0;
	Global.input.shooter_status = 0;
	Global.input.shoot_fire = 0;
	Global.input.vision_status = 0;
	Global.input.Subgimbalisfollow = 1; // 1默认跟随
}
void remove_control_task()
{
	/*按等级提升开超电的最大速度*/

	if (REFEREE_DATA.Chassis_Power_Limit <= 55)
	{
		max_cap_speed = 5.0f;
	}
	else if (REFEREE_DATA.Chassis_Power_Limit > 55 && REFEREE_DATA.Chassis_Power_Limit <= 85)
	{
		max_cap_speed = 5.5f;
	}
	else if (REFEREE_DATA.Chassis_Power_Limit > 85 && REFEREE_DATA.Chassis_Power_Limit <= 100)
	{
		max_cap_speed = 6.0f;
	}
	else if (REFEREE_DATA.Chassis_Power_Limit > 100)
	{
		max_cap_speed = 7.0f;
	}

	// 模式切换区
	// 左上右上 键鼠模式
	if (switch_is_up(RC_L_SW) && switch_is_up(RC_R_SW))
	{
		Global.input.ctl = PC;
	}
	// 遥控器模式
	else
		Global.input.ctl = RC;
	// 左中右中 跟随模式
	if (switch_is_mid(RC_L_SW) && switch_is_mid(RC_R_SW))
		Global.mode = FLOW;
	// 左中右上 顺时针陀螺模式
	if (switch_is_mid(RC_L_SW) && switch_is_down(RC_R_SW))
		Global.mode = SPIN;
	// 左下右中 逆时针陀螺模式
	if (switch_is_down(RC_L_SW) && switch_is_mid(RC_R_SW))
		Global.mode = SPIN;
	// 左下右下 锁定模式
	if (switch_is_down(RC_L_SW) && switch_is_down(RC_R_SW))
		Global.mode = LOCK;
	// 功能执行区
	// 开火

	if (Global.input.shoot_fire)
	{
		if (Global.input.isHeatLimit == 1)
		{
			if (Get_sys_time_ms() - last_shoot_time > 100)
			{
				shoot_Bullets(1);
				last_shoot_time = Get_sys_time_ms();
			}
		}
		else
		{
			if (Get_sys_time_ms() - last_shoot_time > 200)
			{
				shoot_Bullets(1);
				last_shoot_time = Get_sys_time_ms();
			}
		}
	}
	// 瞄准镜开关
	if (Global.input.ScopeisOpen == 0) // 开瞄准镜 副云台保持水平
	{
		gimbal.scope.set = (gimbal.scope.offset - 10000.0f);
	}
	else
	{
		gimbal.scope.set = gimbal.scope.offset + 10000.0f;
	}
	/****************************RC操作*************************************************/
	if (Global.input.ctl == RC)
	{
		/******************底盘行为控制******************/
		Global.input.x = RC_data.rc.ch[0] / 110.0f;
		Global.input.y = RC_data.rc.ch[1] / 110.0f;
		/******************云台行为控制******************/
		// 左上右中 自瞄切换
		if (switch_is_up(RC_L_SW) && switch_is_mid(RC_R_SW))
			Global.input.vision_status = 1;
		else
			Global.input.vision_status = 0;
		// 自瞄控制
		if (Global.input.vision_status == 1)
		{
			Global.input.shooter_status = 1;
			if (fabs(fromNUC.yaw) > 0.0f)
			{
				Global.input.yaw = fromNUC.yaw / YawCofficientFromNUC;
			}
			else
				Global.input.yaw = 0.0f;
			if (fabs(fromNUC.pitch) > 0.0f)
			{
				Global.input.pitch = fromNUC.pitch / PitchCofficientFromNUC;
			}
			else
				Global.input.pitch = 0.0f;
		}
		// 正常控制
		else
		{
			// 角度制
			Global.input.yaw = (RC_data.rc.ch[2] / YawCofficientFromRC);
			Global.input.pitch = (RC_data.rc.ch[3] / PitchCofficientFromRC) * 57.3f;
		}
		/******************射击行为控制******************/
		// 左中右上 开启摩擦轮
		if (switch_is_mid(RC_L_SW) && switch_is_up(RC_R_SW))
			Global.input.shooter_status = 1;

		else
			Global.input.shooter_status = 0;
		// 波轮向上拨 拨弹开关
		if (RC_data.rc.ch[4] == -660)
			Global.input.shoot_fire = 1;
		else
			Global.input.shoot_fire = 0;
	}
	/****************************PC操作*************************************************/
	else if (Global.input.ctl == PC)
	{
		/******************云台行为控制******************/
		/*按下鼠标右键进入自瞄程序*/
		Global.input.vision_status = RC_data.mouse.press_r;
		if (Global.input.vision_status == 1)
		{
			if (fabs(fromNUC.yaw) > 0.0f)
				Global.input.yaw = fromNUC.yaw / YawCofficientFromNUC;
			else
				Global.input.yaw = 0.0f;

			if (fabs(fromNUC.pitch) > 0.0f)
				Global.input.pitch = fromNUC.pitch / PitchCofficientFromNUC;
			else
				Global.input.pitch = 0.0f;
		}
		/*松开鼠标右键进入正常云台控制*/
		else
		{

			// 正常移动云台+微调操作
			if (MOUSE_Z_MOVE_SPEED || MOUSE_Y_MOVE_SPEED)
			{
				if (MOUSE_Z_MOVE_SPEED != 0)
				{
					Global.input.pitch += MOUSE_Z_MOVE_SPEED / PitchLobCofficientFromPC;

					// if(MOUSE_Z_MOVE_SPEED > 0)
					// else if(MOUSE_Z_MOVE_SPEED < 0)
					// 	Global.input.pitch = -0.000000005;					// 	Global.input.pitch = 0.000000005;

					// else
					// 	Global.input.pitch = 0;
				}

				else if (MOUSE_Y_MOVE_SPEED != 0)
				{
					Global.input.pitch = MOUSE_Y_MOVE_SPEED / PitchCofficientFromPC;
				}
			}
			else
			{
				Global.input.pitch = 0;
			}

			if (MOUSE_X_MOVE_SPEED != 0)
			{
				Global.input.yaw = MOUSE_X_MOVE_SPEED / YawCofficientFromPC;
			}
			else if (IF_KEY_PRESSED_C && IF_KEY_PRESSED_CTRL)
			{
				if (Get_sys_time_ms() - Time_delay_press_C_Ctrl > 200)
				{
					Global.input.yaw = -tweaks_value;
					Time_delay_press_C_Ctrl = Get_sys_time_ms();
				}
			}
			else if (IF_KEY_PRESSED_V && IF_KEY_PRESSED_CTRL)
			{
				if (Get_sys_time_ms() - Time_delay_press_V > 200)
				{
					Global.input.yaw = +tweaks_value;
					Time_delay_press_V = Get_sys_time_ms();
				}
			}
			else
			{
				Global.input.yaw = 0;
			}
		}
		/*按下X键瞄准镜开关切换*/
		if (IF_KEY_PRESSED_X && !(IF_KEY_PRESSED_CTRL))
		{
			if (Get_sys_time_ms() - Time_delay_press_X > 350)
			{
				if (Global.input.ScopeisOpen == 1)
					Global.input.ScopeisOpen = 0;
				else if (Global.input.ScopeisOpen == 0)
					Global.input.ScopeisOpen = 1;

				gimbal.scope.offset -= 2.0f;
				Time_delay_press_X = Get_sys_time_ms();
			}
		}
		/******************底盘行为控制******************/
		/*按下Q键进入陀螺模式*/
		if (IF_KEY_PRESSED_Q)
		{
			if (Get_sys_time_ms() - Time_delay_press_Q > 350)
			{
				if (Global.mode == SPIN)
					Global.mode = FLOW;
				else if (Global.mode != SPIN)
				{
					Global.mode = SPIN;
				}
				Time_delay_press_Q = Get_sys_time_ms();
			}
		}
		/*按下C键进入尖角模式*/
		if (IF_KEY_PRESSED_C && !(IF_KEY_PRESSED_CTRL))
		{
			if (Get_sys_time_ms() - Time_delay_press_C > 350)
			{
				if (Global.mode == LEAN)
					Global.mode = FLOW;
				else if (Global.mode != LEAN)
					Global.mode = LEAN;
				Time_delay_press_C = Get_sys_time_ms();
			}
		}

		/*按下G键一键掉头*/
		if (IF_KEY_PRESSED_G && !(IF_KEY_PRESSED_CTRL))
		{
			if (Get_sys_time_ms() - Time_delay_press_G > 350)
			{
				//			gimbal.yaw.set += 180.0f;
				if (Global.input.fly == 0)
				{
					Global.input.fly = 1;
				}
				else if (Global.input.fly != 0)
				{
					Global.input.fly = 0;
				}

				Time_delay_press_G = Get_sys_time_ms();
			}
		}
		/*按下W键前进*/
		if (IF_KEY_PRESSED_W)
		{
			if (Global.input.y < 0.0f)
				Global.input.y = 0.0f;
			if (Global.cap == FULL)
			{
				if (Global.input.y < max_cap_speed)
					Global.input.y += pressShift_step;
				else
					Global.input.y = max_cap_speed;
			}
			else
			{

				if (Global.input.y < 3.0f)
					Global.input.y += pressW_step;
				else
					Global.input.y = 3.0f;
			}
		}
		/*按下S键后退*/
		else if (IF_KEY_PRESSED_S)
		{
			if (Global.input.y > 0.0f)
				Global.input.y = 0.0f;
			if (Global.cap == FULL)
			{
				if (Global.input.y > -8.0f)
					Global.input.y -= 0.01f;
			}
			else
			{
				if (Global.input.y > -1.5f)
					Global.input.y -= pressS_step;
				else
					Global.input.y = -1.5f;
			}
		}
		/*停止*/
		else
		{
			Global.input.y = 0;
		}
		/*按下A键左移*/
		if (IF_KEY_PRESSED_A)
		{
			if (Global.input.x > 0.0f)
				Global.input.x = 0.0f;
			if (Global.cap == FULL)
			{
				if (Global.input.x > -4.0f)
					Global.input.x -= 0.005f;
			}
			else
			{
				if (Global.input.x > -1.5f)
					Global.input.x -= pressA_step;
			}
		}
		/*按下D键右移*/
		else if (IF_KEY_PRESSED_D)
		{
			if (Global.input.x < 0.0f)
				Global.input.x = 0.0f;
			if (Global.cap == FULL)
			{
				if (Global.input.x < 4.5f)
					Global.input.x += 0.005f;
			}
			else
			{
				if (Global.input.x < 1.5f)
					Global.input.x += pressD_step;
			}
		}
		/*静止*/
		else
		{
			Global.input.x = 0;
		}
		/*按下Shift键超级电容开关切换*/
		if (IF_KEY_PRESSED_SHIFT)
			Global.cap = FULL;
		else
			Global.cap = STOP;
		/******************射击行为控制******************/
		if (RC_data.mouse.press_l)
		{
			Time_delay_mouse1++;
		}
		else
		{
			Time_delay_mouse1 = 0;
		}
		if (Time_delay_mouse1 > 100)
			Global.input.shoot_fire = 1;
		else
			Global.input.shoot_fire = 0;
		/*按下F键进入自爆模式*/
		if (IF_KEY_PRESSED_F && !(IF_KEY_PRESSED_CTRL))
		{
			if (Get_sys_time_ms() - Time_delay_press_F > 350)
			{
				if (Global.input.isHeatLimit == 0)
				{
					Global.input.isHeatLimit = 1;
				}
				else
				{
					Global.input.isHeatLimit = 0;
				}
				Time_delay_press_F = Get_sys_time_ms();
			}
		}
		/*按下E键进入拨盘卸力模式*/
		if (IF_KEY_PRESSED_E)
		{
			if (Get_sys_time_ms() - Time_delay_press_E > 350)
			{
				if (Global.input.isOnForce == 1)
				{
					Global.input.isOnForce = 0;
				}
				else
				{
					Global.input.isOnForce = 1;
				}
				Time_delay_press_E = Get_sys_time_ms();
			}
		}
		/*按下R键摩擦轮开关切换*/
		if (IF_KEY_PRESSED_R)
		{
			if (Get_sys_time_ms() - Time_delay_press_R > 350)
			{
				if (Global.input.shooter_status == 0)
					Global.input.shooter_status = 1;
				else
					Global.input.shooter_status = 0;
				Time_delay_press_R = Get_sys_time_ms();
			}
		}
		/*按下F键+CTRL键 提高转速*/
		if (IF_KEY_PRESSED_F && IF_KEY_PRESSED_CTRL)
		{
			if (Get_sys_time_ms() - Time_delay_press_F_CTRL > 350)
			{
				shoot.speedUpLevel += 10;
				shoot.speedLeftLevel += 10;
				shoot.speedRightLevel += 10;
				Time_delay_press_F_CTRL = Get_sys_time_ms();
			}
		}
		/*按下G键+CTRL键 降低转速*/
		if (IF_KEY_PRESSED_G && IF_KEY_PRESSED_CTRL)
		{
			if (Get_sys_time_ms() - Time_delay_press_G_CTRL > 350)
			{
				shoot.speedUpLevel -= 10;
				shoot.speedLeftLevel -= 10;
				shoot.speedRightLevel -= 10;
				Time_delay_press_G_CTRL = Get_sys_time_ms();
			}
		}
		/******************UI刷新控制******************/
		/*按下V键UI结构体初始化*/
		if (IF_KEY_PRESSED_V && !(IF_KEY_PRESSED_CTRL))
		{
			// UI_task_init();
			ui_init();
		}
		/*按下B键刷新瞄准线*/
		if (IF_KEY_PRESSED_B)
		{
			press_refrsh();
		}
		/******************吊射模式行为控制******************/
		/*按Z键进入吊射模式*/
		if (IF_KEY_PRESSED_Z)
		{
			if (Get_sys_time_ms() - Time_delay_press_Z > 350)
			{
				if (Global.mode != LEAN_LOB)
					Global.mode = LEAN_LOB;
				else if (Global.mode == LEAN_LOB)
					Global.mode = FLOW;
				if (Global.mode == LEAN_LOB)
				{
					gimbal.small_pitch.target == 2;
				}
				else if (Global.mode != LEAN_LOB)
				{
					gimbal.small_pitch.target == 0;
				}
				Time_delay_press_Z = Get_sys_time_ms();
			}
		}
		/*按WASD键推出吊射模式*/
		if (IF_KEY_PRESSED_W || IF_KEY_PRESSED_S || IF_KEY_PRESSED_A || IF_KEY_PRESSED_D)
		{

			if (Global.mode == LEAN_LOB)
				Global.mode = FLOW;
		}
		if (IF_KEY_PRESSED_X && IF_KEY_PRESSED_CTRL)
		{
			if (Get_sys_time_ms() - Time_delay_press_X_CTRL > 350)
			{
				if (fly_mode == 0)
					fly_mode = 1;
				else if (fly_mode == 1)
					fly_mode = 0;
				Time_delay_press_X_CTRL = Get_sys_time_ms();
			}
		}
	}
}
