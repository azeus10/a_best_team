/**
 * @file CAN_receive&send.c
 * @author sethome
 * @brief (CAN_motor & surper_cap) control & send_rev
 * @version 0.1
 * @date 2021-12-13
 *
 * @copyright Copyright (c) 2021 sethome
 *
 */
//#include "CAN_receive&send.h"
#include "IMU_updata.h"
#include "cap_ctl.h"

#include "math.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include "string.h"

//电机驱动
#include "LK_motor_process.h"
#include "HT_drive.h"
#include "dm4310_drv.h"
// 电机数据
motor_measure_t motor_data[22];

// CAN寄存器及控制器
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2; // 定义原型在can.c文件

/************************************CAN接收******************************************************/
// HAL库中断回调指针
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header; // CAN 数据指针
  uint8_t rx_data[8];            // 获取到的数据

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); // 取得信息
  CAN_ID_matching(rx_header.StdId,rx_data);
  // 超级电容
  if (rx_header.StdId == 0x307)  //0x307
  {
    cap_handle_message(rx_data);
    return;
  }
  //DM电机	

	//领控电机4015V3
  if(rx_header.StdId == 0x141)
	{
		
		LK_handle_message(rx_data);
		return;
	}
	
	if(rx_header.StdId > 0x1FE)//大疆电机数据接收
	{
		if (hcan == &hcan1) // CAN1/2判断
		{
			DJI_CAN1_handle_message(rx_header.StdId,rx_data);//DJI电机can1数据接收
		}
		else
		{
			DJI_CAN2_handle_message(rx_header.StdId,rx_data);//DJI电机can2数据接收
		}
  	}
	else if(rx_header.StdId < 0x100)//DM
	{
		DM_CAN_Callback((can_id)rx_header.StdId - 1,rx_data);//DM电机数据接收
	}
}

void canx_send_data(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t *data, uint32_t len)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t send_mail_box;
	
	TxHeader.StdId = id;
	TxHeader.IDE =  CAN_ID_STD ;        
	TxHeader.RTR = CAN_RTR_DATA;  

	TxHeader.DLC = len;

	 HAL_CAN_AddTxMessage(hcan, &TxHeader, data,&send_mail_box);
}
// 返回马达数据（还不够安全。。。）
motor_measure_t get_motor_data(can_id motorID) // 获取马达数据
{
  return motor_data[motorID];
}