/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "math.h"
#include "fifo.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "DBUS_remote_control.h"

#include "Global_status.h"
#include "Error_detect.h"
#include "chassis_move.h"
#include "gimbal.h"
#include "shoot.h"
#include "LK_motor_process.h"
#include "nloop_middlewares.h"

#include "NUC_communication.h"
#include "USB_VirCom.h"
#include "vofa.h"
#include "cap_ctl.h"
#include "LED_control.h"
#include "RM_Cilent_UI.h"
#include "referee_handle_pack.h"
#include "referee_usart_task.h"

#include "AHRS_MiddleWare.h"
#include "control_setting.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern struct IMU_t IMU_data;
float relative_angle = 0;
float spin_forward_sen = -7.9f;

/* USER CODE END Variables */
/* Definitions for Flash_LED_Task */
osThreadId_t Flash_LED_TaskHandle;
const osThreadAttr_t Flash_LED_Task_attributes = {
	.name = "Flash_LED_Task",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for CAN_sendTask */
osThreadId_t CAN_sendTaskHandle;
const osThreadAttr_t CAN_sendTask_attributes = {
	.name = "CAN_sendTask",
	.stack_size = 256 * 4,
	.priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for RemoteTask */
osThreadId_t RemoteTaskHandle;
const osThreadAttr_t RemoteTask_attributes = {
	.name = "RemoteTask",
	.stack_size = 256 * 4,
	.priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for ChassisTask */
osThreadId_t ChassisTaskHandle;
const osThreadAttr_t ChassisTask_attributes = {
	.name = "ChassisTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for GimbalTask */
osThreadId_t GimbalTaskHandle;
const osThreadAttr_t GimbalTask_attributes = {
	.name = "GimbalTask",
	.stack_size = 512 * 4,
	.priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for NUCcontrolTask */
osThreadId_t NUCcontrolTaskHandle;
const osThreadAttr_t NUCcontrolTask_attributes = {
	.name = "NUCcontrolTask",
	.stack_size = 512 * 4,
	.priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for ErrorDetectTask */
osThreadId_t ErrorDetectTaskHandle;
const osThreadAttr_t ErrorDetectTask_attributes = {
	.name = "ErrorDetectTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow,
};
/* Definitions for RefereeTask */
osThreadId_t RefereeTaskHandle;
const osThreadAttr_t RefereeTask_attributes = {
	.name = "RefereeTask",
	.stack_size = 512 * 4,
	.priority = (osPriority_t)osPriorityLow,
};
/* Definitions for ModechangeTask */
osThreadId_t ModechangeTaskHandle;
const osThreadAttr_t ModechangeTask_attributes = {
	.name = "ModechangeTask",
	.stack_size = 512 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for RM_UI_refresh_t */
osThreadId_t RM_UI_refresh_tHandle;
const osThreadAttr_t RM_UI_refresh_t_attributes = {
	.name = "RM_UI_refresh_t",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Flash_LED_Task_callback(void *argument);
void CAN_sendTask_callback(void *argument);
void RemoteTask_callback(void *argument);
void ChassisTask_callback(void *argument);
void GimbalTask_callback(void *argument);
void NUCcontrolTask_callback(void *argument);
void ErrorDetectTask_callback(void *argument);
void RefereeTask_callback(void *argument);
void ModechangeTask_callback(void *argument);
void RM_UI_refresh_task_callback(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of Flash_LED_Task */
	Flash_LED_TaskHandle = osThreadNew(Flash_LED_Task_callback, NULL, &Flash_LED_Task_attributes);

	/* creation of CAN_sendTask */
	CAN_sendTaskHandle = osThreadNew(CAN_sendTask_callback, NULL, &CAN_sendTask_attributes);

	/* creation of RemoteTask */
	RemoteTaskHandle = osThreadNew(RemoteTask_callback, NULL, &RemoteTask_attributes);

	/* creation of ChassisTask */
	ChassisTaskHandle = osThreadNew(ChassisTask_callback, NULL, &ChassisTask_attributes);

	/* creation of GimbalTask */
	GimbalTaskHandle = osThreadNew(GimbalTask_callback, NULL, &GimbalTask_attributes);

	/* creation of NUCcontrolTask */
	NUCcontrolTaskHandle = osThreadNew(NUCcontrolTask_callback, NULL, &NUCcontrolTask_attributes);

	/* creation of ErrorDetectTask */
	ErrorDetectTaskHandle = osThreadNew(ErrorDetectTask_callback, NULL, &ErrorDetectTask_attributes);

	/* creation of RefereeTask */
	RefereeTaskHandle = osThreadNew(RefereeTask_callback, NULL, &RefereeTask_attributes);

	/* creation of ModechangeTask */
	ModechangeTaskHandle = osThreadNew(ModechangeTask_callback, NULL, &ModechangeTask_attributes);

	/* creation of RM_UI_refresh_t */
	RM_UI_refresh_tHandle = osThreadNew(RM_UI_refresh_task_callback, NULL, &RM_UI_refresh_t_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_Flash_LED_Task_callback */
/**
 * @brief  Function implementing the Flash_LED_Task thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Flash_LED_Task_callback */
void Flash_LED_Task_callback(void *argument)
{
	/* init code for USB_DEVICE */
	MX_USB_DEVICE_Init();
	/* USER CODE BEGIN Flash_LED_Task_callback */
	/* Infinite loop */
	for (;;)
	{
		led_show(BLUE);
		osDelay(500);
		led_show(RED);
		osDelay(500);
		led_show(GREEN);
		osDelay(500);
	}
	/* USER CODE END Flash_LED_Task_callback */
}

/* USER CODE BEGIN Header_CAN_sendTask_callback */
/**
 * @brief Function implementing the CAN_sendTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_CAN_sendTask_callback */
void CAN_sendTask_callback(void *argument)
{
	/* USER CODE BEGIN CAN_sendTask_callback */
	/* Infinite loop */
	for (;;)
	{
		CAN1_send_current();
		CAN2_send_current();
		osDelay(5);
	}
	/* USER CODE END CAN_sendTask_callback */
}

/* USER CODE BEGIN Header_RemoteTask_callback */
/**
 * @brief Function implementing the RemoteTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RemoteTask_callback */
void RemoteTask_callback(void *argument)
{
	/* USER CODE BEGIN RemoteTask_callback */
	/* Infinite loop */
	for (;;)
	{
		remove_control_task();
		osDelay(1);
	}
	/* USER CODE END RemoteTask_callback */
}

/* USER CODE BEGIN Header_ChassisTask_callback */
/**
 * @brief Function implementing the ChassisTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ChassisTask_callback */
void ChassisTask_callback(void *argument)
{
	/* USER CODE BEGIN ChassisTask_callback */

	/* Infinite loop */
	for (;;)
	{
		chassis_moto_speed_calc();
		osDelay(5);
	}
	/* USER CODE END ChassisTask_callback */
}

/* USER CODE BEGIN Header_GimbalTask_callback */
/**
 * @brief Function implementing the GimbalTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_GimbalTask_callback */
void GimbalTask_callback(void *argument)
{
	/* USER CODE BEGIN GimbalTask_callback */
	/* Infinite loop */

	for (;;)
	{
		gimbal_pid_cal();
		gimbal_updata();
		shoot_pid_cal();
		shoot_update();
		osDelay(1);
	}
	/* USER CODE END GimbalTask_callback */
}

/* USER CODE BEGIN Header_NUCcontrolTask_callback */
/**
 * @brief Function implementing the NUCcontrolTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_NUCcontrolTask_callback */
void NUCcontrolTask_callback(void *argument)
{
	/* USER CODE BEGIN NUCcontrolTask_callback */
	/* Infinite loop */
	// 视觉通信
	char data[128];
	// 串口显示波形（Vofa+）
	ext_robot_status_t robot_status;
	for (;;)
	{
		// 视觉通信部分
		if (get_robot_id() > 100) // 敌方颜色
			toNUC.enemy = 1;	  // 1        //红色  自瞄识别
		else
			toNUC.enemy = 0; // 0      //蓝色
		toNUC.b_speed = 15.5f;
		toNUC.mode = 1;
		toNUC.yaw = rad2degree(IMU_data.AHRS.yaw);
		toNUC.pitch = rad2degree(IMU_data.AHRS.pitch);

		encodeSTM32(&toNUC, data, sizeof(STM32_data_t));
		VirCom_send(data, sizeof(STM32_data_t));
		// 虚拟串口显示波形部分
		// 格式如下
		//   char matlab[128];

		//  unsigned char len = 0;
		//  len = sprintf(matlab,"%f,%f,%f\r\n",get_motor_data(SHOOT_MOTOR1).speed_rpm/1.0,
		//  										(get_motor_data(SHOOT_MOTOR2).speed_rpm)/1.0,
		//  										-(get_motor_data(SHOOT_MOTOR3).speed_rpm)/1.0);
		//  VirCom_send(matlab,len);

		osDelay(1);
	}

	/* USER CODE END NUCcontrolTask_callback */
}

/* USER CODE BEGIN Header_ErrorDetectTask_callback */
/**
 * @brief Function implementing the ErrorDetectTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ErrorDetectTask_callback */
void ErrorDetectTask_callback(void *argument)
{
	/* USER CODE BEGIN ErrorDetectTask_callback */
	/* Infinite loop */
	for (;;)
	{
		Error_detect_flush();
		osDelay(100);
	}
	/* USER CODE END ErrorDetectTask_callback */
}

/* USER CODE BEGIN Header_RefereeTask_callback */
/**
 * @brief Function implementing the RefereeTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RefereeTask_callback */
void RefereeTask_callback(void *argument)
{
	/* USER CODE BEGIN RefereeTask_callback */

	/* Infinite loop */
	for (;;)
	{
		referee_usart_task();
		Required_Data();
		Cap_Required_Data();
		cap_update();
		osDelay(5);
	}
	/* USER CODE END RefereeTask_callback */
}

/* USER CODE BEGIN Header_ModechangeTask_callback */
/**
 * @brief Function implementing the ModechangeTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ModechangeTask_callback */
void ModechangeTask_callback(void *argument)
{
	/* USER CODE BEGIN ModechangeTask_callback */
	/* Infinite loop */
	float x_s, y_s, r_s;
	float sin_beta, cos_beta;
	uint32_t mul = 0;
	pid_t chassis_follow;
	// 底盘跟随PID
	pid_set(&chassis_follow, 2, 0, 10, 50, 0);
	for (;;)
	{
		decode_as_6020(YAW_MOTOR);
		decode_as_6020(PITCH_MOTOR);
		if (Global.mode == FLOW)
		{
			if(fly_mode == OPEN)
			pid_set(&chassis_follow,5, 0, 30, 50, 0);
		else
			pid_set(&chassis_follow,3, 0, 26, 50, 0);
			relative_angle = -(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset);
			if (RC_data.rc.ch[2] == 0 && Global.input.yaw == 0)
				relative_angle = 0; // 防止走不直
		}
		else if (Global.mode == SPIN)
		{
			relative_angle = -(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset - get_motor_data(YAW_MOTOR).round_speed * spin_forward_sen);
			r_s = chassis_spin_speed_level_up(); // 小陀螺转速按等级提升
		}
		else if (Global.mode == LOCK)
		{
			x_s = 0;
			y_s = 0;
			r_s = 0;
			chassis.speed.x = 0;
			chassis.speed.y = 0;
			chassis.speed.r = 0;
			Global.input.pitch = 0;
			Global.input.yaw = 0;
			Global.input.x = 0;
			Global.input.y = 0;
			Global.input.r = 0;
			Global.input.isHeatLimit = 0;
			Global.input.shooter_status = 0;
			Global.input.shoot_fire = 0;
		}
		else if (Global.mode == LEAN_LOB)
		{
			relative_angle = 0;
		}
		else if (Global.mode == LEAN)
		{
			relative_angle = -(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset - 45.0f);
		}
		sin_beta = sinf(degree2rad(relative_angle));
		cos_beta = cosf(degree2rad(relative_angle));
		x_s = Global.input.x * cos_beta - sin_beta * Global.input.y; // 运动分解
		y_s = Global.input.x * sin_beta + Global.input.y * cos_beta;
		mul = abs((int)relative_angle) / 180;
		if (relative_angle > 180.0f)
		{
			if (mul % 2 == 1) // 处于-180度
				relative_angle -= (mul + 1) * 180;
			else // 处于180度
				relative_angle -= mul * 180;
		}
		if (relative_angle < -180.0f)
		{
			if (mul % 2 == 1) // 处于180度
				relative_angle += (mul + 1) * 180;
			else // 处于-180度
				relative_angle += mul * 180;
		}
		if (fabs(relative_angle) > 0.3f)									  //
			r_s = pid_cal(&chassis_follow, degree2rad(relative_angle), 0.0f); // degree2rad(relative_angle)
		else
			r_s = 0.0;

		if (fabs(Global.input.yaw) > 0.00f)
		{
			gimbal.yaw.set += Global.input.yaw;
		}
		if (fabs(Global.input.pitch) > 0.0f)
		{
			gimbal.pitch.set -= Global.input.pitch;
		}
		chassis.speed.x = x_s;
		chassis.speed.y = y_s;
		chassis.speed.r = r_s;

		osDelay(5);
	}
	/* USER CODE END ModechangeTask_callback */
}

/* USER CODE BEGIN Header_RM_UI_refresh_task_callback */
/**
 * @brief Function implementing the RM_UI_refresh_t thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RM_UI_refresh_task_callback */
void RM_UI_refresh_task_callback(void *argument)
{
	/* USER CODE BEGIN RM_UI_refresh_task_callback */
	/* Infinite loop */
	for (;;)
	{
		// UI刷新
		ui_shoot_speed((int)((shoot.speedUpLevel - 5350) * 1.0f), (int)((shoot.speedLeftLevel - 5350) * 1.0)); // 摩擦轮转速差更新
		ui_pitch_angle(IMU_data.AHRS.pitch * 57.3f * 1000);													   // pitch角度更新
		ui_supercap(cap.remain_vol);																		   // 超点能量
		char_change();																						   // 字符更新
		ui_chassis(relative_angle);																			   // 地盘方向指示
		osDelay(2);
		ui_updata();
		osDelay(3);
		UI_id_check(); // 检查机器人id，实现动态更新
		osDelay(1);
	}
	/* USER CODE END RM_UI_refresh_task_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
