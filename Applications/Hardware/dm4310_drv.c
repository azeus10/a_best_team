#include "dm4310_drv.h"
#include "CAN_receive&send.h"
#include "cmsis_os.h"
#include "can_bsp.h"

#include "Stm32_time.h"
#include "IMU_updata.h"

#include "CAN_ID_Library.h"
uint32_t Time_delay_trs = 0;
uint32_t Time_delay_ENABLE = 0;

DM_can_recive_message DM_motor_data[5];
uint8_t DM_Mode[22] = {0};
/**********************************用户调用函数**************************************/
/*
	达秒电机的初始化函数
*/
#define MIT      1
#define LOCATION 2
#define SPEED    3
#define E_MIT    4
void DM_Enable(can_id Motor_ID)
{
	if(Motor_ID >= CAN_2_1)//can2
	{
		if (Get_sys_time_ms() - Time_delay_ENABLE > 50)
		{
			DM_enable_TX((can_id)Motor_ID);
			Time_delay_ENABLE = Get_sys_time_ms();
		}	
	}
	else
	{
	
	}

}

void DM_Init()
{
		DM_Mode[PITCH_MOTOR] = LOCATION;
	DM_enable_TX(PITCH_MOTOR);
}


/*
设置某个电机的位置跟速度，控制电机主要就调用这个就行了
Motor_ID：达秒电机返回来数据的ID（设置Master ID时跟CAN ID设置成一样的，防止麻烦）
pos：位置
vel：速度
*/
void DM_Motor_set(can_id Motor_ID,float pos,float vel)//如果是速度模式pos随便给就可以了，因为就算赋值了也发不出去
{
	if(DM_motor_data[Motor_ID - CAN_2_1].state == 0)//如果电机未使能
		DM_Enable(Motor_ID);
	else
	{
		if(DM_Mode[Motor_ID] == LOCATION)//位置速度模式
			DM_CAN_location_TX(Motor_ID,pos,vel);
		else if(DM_Mode[Motor_ID] == SPEED)//速度模式
			DM_CAN_speed_TX(Motor_ID,vel);
	}
}

/**********************************DM电机驱动****************************************/

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


CAN_TxHeaderTypeDef DM_message_Tx;
CAN_TxHeaderTypeDef DM_message_Rx;

//发送给达秒电机的数据包
uint8_t  DM_pos_ves_data[8];
uint8_t  DM_speed_data[4];

/**************************************************************************
* @brief:      	uint_to_float: 无符号整数转换为浮点数函数
* @param[in]:   x_int: 待转换的无符号整数
* @param[in]:   x_min: 范围最小值
* @param[in]:   x_max: 范围最大值
* @param[in]:   bits:  无符号整数的位数
* @retval:     	浮点数结果
* @details:    	将给定的无符号整数 x_int 在指定范围 [x_min, x_max] 内进
*								行线性映射，映射结果为一个浮点数
**************************************************************************/
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

/**************************************************************************
* @brief:      	DM_enable_TX: 使能电机函数
* @retval:     	void
* @details:    	用于达妙电机的使能操作，不使能则电机不启动，为红灯
**************************************************************************/
void DM_enable_TX(can_id ID)
{
		uint32_t send_mail_box;
		uint8_t data[8]; 
		
		DM_message_Tx.IDE = CAN_ID_STD;
		DM_message_Tx.RTR = CAN_RTR_DATA;
		DM_message_Tx.DLC = 0x08;
		data[0] = 0xFF;
		data[1] = 0xFF;
		data[2] = 0xFF;
		data[3] = 0xFF;
		data[4] = 0xFF;
		data[5] = 0xFF;
		data[6] = 0xFF;
		data[7] = 0xFC;
		if(ID < CAN_2_1)
		{
			DM_message_Tx.StdId = ID + 1;
			HAL_CAN_AddTxMessage(&hcan1, &DM_message_Tx, data, &send_mail_box);
		}
		else
		{
			DM_message_Tx.StdId = ID - CAN_2_1 + 1;
			HAL_CAN_AddTxMessage(&hcan2, &DM_message_Tx, data, &send_mail_box);
		}
}

/**************************************************************************
* @brief:      	DM_disable_TX: 失能电机函数
* @retval:     	void
* @details:    	用于达妙电机的失能操作。
**************************************************************************/
void DM_disable_TX(can_id ID)
{
		uint32_t send_mail_box;
		uint8_t data[8]; 
	
		DM_message_Tx.StdId = 0x200+ID;
		DM_message_Tx.IDE = CAN_ID_STD;
		DM_message_Tx.RTR = CAN_RTR_DATA;
		DM_message_Tx.DLC = 0x08;
		data[0] = 0xFF;
		data[1] = 0xFF;
		data[2] = 0xFF;
		data[3] = 0xFF;
		data[4] = 0xFF;
		data[5] = 0xFF;
		data[6] = 0xFF;
		data[7] = 0xFD;
		if(ID > CAN_1_6020_7)//can2
			HAL_CAN_AddTxMessage(&hcan2, &DM_message_Tx, data, &send_mail_box);
		else//can1
			HAL_CAN_AddTxMessage(&hcan1, &DM_message_Tx, data, &send_mail_box);
}

/**************************************************************************
* @brief:      	DM_CAN_location_TX: 位置速度模式控制函数
* @param[in]:   _pos: 位置角度（rad）
* @param[in]:   _vel: 速度（rad/s)
* @retval:     	void
* @details:    	通过CAN总线向电机发送位置速度控制命令
**************************************************************************/


void DM_CAN_location_TX(can_id ID,float _pos, float _vel)
{
	uint8_t *pbuf,*vbuf;
	pbuf=(uint8_t*)&_pos;
	vbuf=(uint8_t*)&_vel;
	
	DM_pos_ves_data[0] = *pbuf;
	DM_pos_ves_data[1] = *(pbuf+1);
	DM_pos_ves_data[2] = *(pbuf+2);
	DM_pos_ves_data[3] = *(pbuf+3);
	
	DM_pos_ves_data[4] = *vbuf;
	DM_pos_ves_data[5] = *(vbuf+1);
	DM_pos_ves_data[6] = *(vbuf+2);
	DM_pos_ves_data[7] = *(vbuf+3);	


//	if(Get_sys_time_ms() - Time_delay_trs > 0.5)
//	{
		if(ID > CAN_1_6020_7)//can2
		{	
			canx_send_data(&hcan2,POS_MODE+ID-CAN_2_1,DM_pos_ves_data,0x08);
		}
		else//can1
		{
			canx_send_data(&hcan1,POS_MODE+ID,DM_pos_ves_data,0x08);
		}
		Time_delay_trs = Get_sys_time_ms();
//+
}

/**************************************************************************
* @brief:      	DM_CAN_speed_TX: 速度模式控制函数
* @param[in]:   _vel: 速度（rad/s)
* @retval:     	void
* @details:    	通过CAN总线向电机发送速度控制命令
**************************************************************************/
void DM_CAN_speed_TX(can_id ID,float _vel)
{
		uint8_t *vbuf;
		vbuf=(uint8_t*)&_vel;

		DM_message_Tx.StdId = SPEED_MODE+ID;
		DM_message_Tx.IDE = CAN_ID_STD;
		DM_message_Tx.RTR = CAN_RTR_DATA;
		DM_message_Tx.DLC = 0x04;
	
		DM_speed_data[0] = *vbuf;
		DM_speed_data[1] = *(vbuf+1);
		DM_speed_data[2] = *(vbuf+2);
		DM_speed_data[3] = *(vbuf+3);

	if(Get_sys_time_ms() - Time_delay_trs > 2)
	{
		if(ID > CAN_1_6020_7)//can2
		{	
			canx_send_data(&hcan2,POS_MODE+ID-CAN_2_1,DM_speed_data,0x04);
		}
		else//can1
		{
			canx_send_data(&hcan1,POS_MODE+ID,DM_speed_data,0x04);	
		}
		Time_delay_trs = Get_sys_time_ms();
	}
}
/**************************************************************************
* @brief:      	DM_CAN_Callback: 获取DM4310电机反馈数据函数
* @param[in]:   rx_data:  指向包含反馈数据的数组指针
* @param[in]:   data_len: 数据长度
* @retval:     	void
* @details:    	从接收到的数据中提取DM4310电机的反馈信息，包括电机ID、
*               状态、位置、速度、扭矩相关温度参数、寄存器数据等
*	@details: 		id:电机ID
*								state:状态信息（参考手册，此处不详细给出)
*								position:位置信息
*								velocity:速度信息
*								torque:扭矩信息
*								Tmos:驱动上MOS 的平均温度
*								Tcoil:电机内部线圈的平均温度
**************************************************************************/
void DM_CAN_Callback(can_id master_ID,uint8_t data[8])
{
		DM_motor_data[master_ID].id=(data[0])&0x0F;//00001111
		DM_motor_data[master_ID].state=(data[0])>>4;
		DM_motor_data[master_ID].p_int=(data[1]<<8)|data[2];
		DM_motor_data[master_ID].v_int=(data[3]<<4)|(data[4]>>4);
		DM_motor_data[master_ID].t_int=((data[4]&0xF)<<8)|data[5];		
		DM_motor_data[master_ID].position = uint_to_float(DM_motor_data[master_ID].p_int, P_MIN, P_MAX, 16);// (-12.5,12.5)
		DM_motor_data[master_ID].velocity = uint_to_float(DM_motor_data[master_ID].v_int, V_MIN, V_MAX, 12);// (-30.0,30.0)
		DM_motor_data[master_ID].torque = uint_to_float(DM_motor_data[master_ID].t_int, T_MIN, T_MAX, 12);	// (-10.0,10.0)	
		DM_motor_data[master_ID].Tmos=(float)(data[6]);
		DM_motor_data[master_ID].Tcoil=(float)(data[7]);	
	
}

