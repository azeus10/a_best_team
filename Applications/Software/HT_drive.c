#include "HT_drive.h"
CAN_TxHeaderTypeDef HT_message;
uint8_t  HT_speed_data[2];
uint8_t  HT_location_data[4];
uint8_t  HT_location_speedset_data[3];
uint8_t  HT_power_data[2];
uint8_t  Offset_judge = -1;
uint16_t Get_Angle = 0;
int32_t GET_Total_Angle = 0;
int16_t GET_speed = 0;
int lock_power = 0;
int last_HT_time = 0;
int power_set = 800;
#define DEVADDR 1
//位置闭环控制，位置值发送
void HT_CAN_location_TX(uint32_t location)
{
    uint32_t send_mail_box;
    HT_message.StdId = (0x55<<4 | DEVADDR);
    HT_message.IDE = CAN_ID_STD;
    HT_message.RTR = CAN_RTR_DATA;
    HT_message.DLC = 0x04;
    HT_location_data[0] = (location & 0xFF);
    HT_location_data[1] = ((location >> 8) & 0xFF);
    HT_location_data[2] = ((location >> 16) & 0xFF);
    HT_location_data[3] = ((location >> 24) & 0xFF);

    HAL_CAN_AddTxMessage(&hcan2, &HT_message, HT_location_data, &send_mail_box);
}
//速度闭环控制，速度值发送
void HT_CAN_speed_TX( int16_t speed)
{
    uint32_t send_mail_box;
    HT_message.StdId = (0x54<<4 | DEVADDR);
    HT_message.IDE = CAN_ID_STD;
    HT_message.RTR = CAN_RTR_DATA;
    HT_message.DLC = 0x02;
    HT_speed_data[0] = (speed & 0xFF);
    HT_speed_data[1] = ((speed >> 8) & 0xFF);
   
    HAL_CAN_AddTxMessage(&hcan2, &HT_message, HT_speed_data, &send_mail_box);
}
//设置位置闭环时候都的速度，发送速度值
void HT_CAN_location_speed_set_TX(uint32_t speed_set)
{
    uint32_t send_mail_box;
    HT_message.StdId = (0x57<<4 | DEVADDR);
    HT_message.IDE = CAN_ID_STD;
    HT_message.RTR = CAN_RTR_DATA;
    HT_message.DLC = 0x03;
    HT_location_speedset_data[0] = 1;
    HT_location_speedset_data[1] = ((speed_set >> 8) & 0xFF);
    HT_location_speedset_data[2] = ((speed_set >> 16) & 0xFF);

    HAL_CAN_AddTxMessage(&hcan2, &HT_message, HT_location_speedset_data, &send_mail_box);
}
//电流值闭环控制,电流值发送
void HT_CAN_power_TX(uint32_t power)
{
    uint32_t send_mail_box;
    HT_message.StdId =  (0x53<<4 | DEVADDR);
    HT_message.IDE = CAN_ID_STD;
    HT_message.RTR = CAN_RTR_DATA;
    HT_message.DLC = 0x02;
    HT_power_data[0] = (power & 0xFF);
    HT_power_data[1] = ((power >> 8) & 0xFF);

    HAL_CAN_AddTxMessage(&hcan2, &HT_message, HT_power_data, &send_mail_box);
}

//接收电流值闭环控制的返回信息
void HT_CAN_power_RX(uint8_t aData[])
{
	Get_Angle = ((int32_t)aData[1] << 8)  |
				((int32_t)aData[0]);      
	GET_Total_Angle = ((int32_t)aData[5] << 24) | 
					  ((int32_t)aData[4] << 16) |
					  ((int32_t)aData[3] << 8)  |
					  ((int32_t)aData[2]);       
	GET_speed = ((int32_t)aData[7] << 8)  |
				((int32_t)aData[6]);  
	GET_speed = GET_speed*0.1f;
}

//设置当前位置为零点
void HT_CAN_offset_TX()
{
    uint32_t send_mail_box;
    HT_message.StdId =   (0x21<<4 | DEVADDR);
    HT_message.IDE = CAN_ID_STD;
    HT_message.RTR = CAN_RTR_DATA;
    HT_message.DLC = 0x00;
   
    HAL_CAN_AddTxMessage(&hcan2, &HT_message,0, &send_mail_box);
}

//接收零点信息，这里只接收一个数据用来判断设置的是否成功【0x00:失败】【0x01:成功】
void HT_CAN_offset_RX(uint8_t aData[])
{
   Offset_judge =aData[2];
}

//can接收时候用的，使用的时候放到can接收回调函数下面
void CAN_ID_matching(uint32_t StdId ,uint8_t aData[])
{
	//接收零点的返回信息
	if(StdId ==  (0x21<<4 | DEVADDR))
		HT_CAN_offset_RX(aData);
	if(StdId ==  (0x53<<4| DEVADDR))
		HT_CAN_power_RX(aData);
}

void HT_small_pitch()
{
	if(lock_power == 0)
	{
		HT_CAN_power_TX(power_set);
		if(fabs((float)GET_speed)< 10 && last_HT_time == 0)
		{
			last_HT_time = Get_sys_time_s();
		}
		
		if((Get_sys_time_s()-last_HT_time) > 3 && fabs((float)GET_speed)< 10)
		{	
			{
				HT_CAN_offset_TX();
				lock_power = 1;
			}
			
		}
	}
	if(Offset_judge == 1)
	{
//HT_CAN_location_speed_set_TX(5000);
		if(gimbal.small_pitch.target == 1)
		{
			HT_CAN_location_TX(-16383);
		}
		else if(gimbal.small_pitch.target == 2)
		{
			HT_CAN_location_TX(-20478);
		}
		else
		{
			HT_CAN_location_TX(-7735);
		}
	}
	else if(Offset_judge == 0)
	{
		lock_power = 0;
		last_HT_time = 0;
	}
	else
	{
		lock_power = 0;
	}
}
