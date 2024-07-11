#include "CAN_receive&send.h"
#include "LK_motor_process.h"

extern CAN_HandleTypeDef hcan1;


LK_can_send_message   LK_control;
LK_can_recive_message LK_motor_data;
int flag=0;

void LK_handle_message(uint8_t data[8])
{
	if(LK_control.mode==0xA3)                                       //单圈位置速度控制
	{
	  LK_motor_data.cmd=data[0];                                   //命令
	  LK_motor_data.temperature=data[1];                           //温度
	  LK_motor_data.power=(uint16_t)((data)[3] << 8 | (data)[2]);  //转矩电流
	  LK_motor_data.speed=(uint16_t)((data)[5] << 8 | (data)[4]);  //转速
	  LK_motor_data.encoder=(uint16_t)((data)[7] << 8 | (data)[6]);//编码器
	}
//	else if(LK_control.mode==0x94)                                  //获取当前单圈角度
//	{
//		*(uint8_t *)(&LK_motor_data.circleAngle)=data[4];
//		*(uint8_t *)((&LK_motor_data.circleAngle)+1)=data[5];
//	  *(uint8_t *)((&LK_motor_data.circleAngle)+2)=data[6];
//		*(uint8_t *)((&LK_motor_data.circleAngle)+3)=data[7];
//	}
//	
//	if(LK_control.mode==0xA5)
//	{
//		LK_control.mode=0x94;
//	}
//	else
//	{
//		LK_control.mode=0xA5;
//	}
}
//单圈位置速度闭环控制
void LK_send_message(void)
{
	uint8_t LK_can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;
	
	
  tx_message.StdId = 0x141;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;
  
  
  LK_control.mode=0xA3;
  LK_can_send_data[0] = LK_control.mode;
	
	
	if(LK_can_send_data[0] ==0xA3)
	{
	 LK_can_send_data[1] = 0;
	 LK_can_send_data[2] = 0;
   LK_can_send_data[3] = 0;
   LK_can_send_data[4] = *(uint8_t *) (&LK_control.angleControl);
   LK_can_send_data[5] = *((uint8_t *)(&LK_control.angleControl)+1); 
   LK_can_send_data[6] = *((uint8_t *)(&LK_control.angleControl)+2);
   LK_can_send_data[7] = *((uint8_t *)(&LK_control.angleControl)+3);
	}
	else if(LK_can_send_data[0] ==0x94)
	{
	 LK_can_send_data[1] =0;
	 LK_can_send_data[2] =0;
   LK_can_send_data[3] =0;
   LK_can_send_data[4] =0;
   LK_can_send_data[5] =0; 
   LK_can_send_data[6] =0;
   LK_can_send_data[7] =0;
	}
//  if(fabs(LK_motor_data.speed)<=0&&fabs(LK_motor_data.power)>=850)  //需要换向
//	{
//		if(LK_can_send_data[1] ==0x00)
//		{
//			LK_can_send_data[1] =0x01;
//		}
//		else
//		{
//			LK_can_send_data[1] =0x00;
//		}
//	}
//	LK_control.angleControl=0.0f;
  HAL_CAN_AddTxMessage(&hcan1, &tx_message, LK_can_send_data, &send_mail_box);
	


#ifdef USE_CAN_1_6020

  tx_message.StdId = CAN_6020_SIGN_ID;

  can_send_data[0] = (motor_data[CAN_1_6020_5].set >> 8);
  can_send_data[1] = motor_data[CAN_1_6020_5].set;

  can_send_data[2] = (motor_data[CAN_1_6020_6].set >> 8);
  can_send_data[3] = motor_data[CAN_1_6020_6].set;

  can_send_data[4] = (motor_data[CAN_1_6020_7].set >> 8);
  can_send_data[5] = motor_data[CAN_1_6020_7].set;

  can_send_data[6] = 0;
  can_send_data[7] = 0;

#ifdef USE_NOP_DELAY
  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
  {
  };
#endif
#ifdef USE_FREERTOS_DELAY
  osDelay(1); // 延时1ms
#endif

  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
#endif
}
