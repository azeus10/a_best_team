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
#include "cap_ctl.h"//

#include "math.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include "string.h"

//�������
#include "LK_motor_process.h"
#include "HT_drive.h"
#include "dm4310_drv.h"
// �������
motor_measure_t motor_data[22];

// CAN�Ĵ�����������
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2; // ����ԭ����can.c�ļ�



/************************************CAN����******************************************************/
// HAL���жϻص�ָ��
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header; // CAN ����ָ��
  uint8_t rx_data[8];            // ��ȡ��������

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); // ȡ����Ϣ
  CAN_ID_matching(rx_header.StdId,rx_data);
  // ��������
  if (rx_header.StdId == 0x307)  //0x307
  {
    cap_handle_message(rx_data);
    return;
  }
  //DM���	

	//��ص��4015V3
  if(rx_header.StdId == 0x141)
	{
		
		LK_handle_message(rx_data);
		return;
	}
	
	if(rx_header.StdId > 0x1FE)//�󽮵�����ݽ���
	{
		if (hcan == &hcan1) // CAN1/2�ж�
		{
			DJI_CAN1_handle_message(rx_header.StdId,rx_data);//DJI���can1���ݽ���
		}
		else
		{
			DJI_CAN2_handle_message(rx_header.StdId,rx_data);//DJI���can2���ݽ���
		}
  	}
	else if(rx_header.StdId < 0x100)//DM
	{
		DM_CAN_Callback((can_id)rx_header.StdId - 1,rx_data);//DM������ݽ���
		return;
	}
//	cap.remain_vol = 0;//������
//	if(rx_header.StdId == 0x309)//double
//	{
//		data_receive(rx_data);
//		return;
//	}   
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
// �����������ݣ���������ȫ��������
motor_measure_t get_motor_data(can_id motorID) // ��ȡ��������
{
  return motor_data[motorID];
}
