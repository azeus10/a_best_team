#include "cap_ctl.h"
#include "CAN_receive&send.h"
#include "string.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2


cap_t cap;

typedef struct
{
	uint8_t setPower;
	uint16_t cacheEnergy;
  uint16_t cacheEnergylimit;
} can_send_t;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void cap_handle_message(uint8_t data[8])
{
	static float rev_vol;

	memcpy(&rev_vol, data, sizeof(rev_vol));

	cap.remain_vol = rev_vol;
}

void cap_update()
{
	#ifndef USE_DOUBLE_CTL_MODE
	static uint8_t can_send_data[8];
	static CAN_TxHeaderTypeDef tx_message;
	uint32_t send_mail_box;

	tx_message.StdId = 0x306;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	can_send_t send_data;

	send_data.setPower = cap.set_max_power;
	send_data.cacheEnergy = cap.cache_energy;
	send_data.cacheEnergylimit=cap.cacheEnergylimit;

	memcpy(can_send_data, &send_data, sizeof(can_send_t));

	HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
	#endif
}

int cap_set_power(uint8_t set)
{
	if (set > 200 || set < 50)
		return -1;
	cap.set_max_power = set;
	cap_update();
	return 0;
}

float cap_get_remain_vol()
{
	return cap.remain_vol;
}

float cap_get_predict_energy()
{
	return cap.prediect_energy;
}
can_send_t double_2;

void data_send()
{
	#ifdef USE_DOUBLE_CTL_MODE
	uint8_t double_send_data[8];
	static CAN_TxHeaderTypeDef tx_message;
	uint32_t send_mail_box;

	tx_message.StdId = 0x308;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;
	
	double_send_data[0] = cap.set_max_power;
	
	double_send_data[1] = cap.cache_energy;
//	double_send_data[2] = (cap.cache_energy >> 8) & 0xFF; 
	
	double_send_data[2] = (cap.cacheEnergylimit >> 0) & 0xFF;
	double_send_data[3] = (cap.cacheEnergylimit >> 8) & 0xFF; 
	 
	HAL_CAN_AddTxMessage(&hcan1, &tx_message, double_send_data, &send_mail_box);
	#endif
}
// ??float???????
void float_to_bytes(float f, uint8_t *bytes) 
{
    uint32_t *p = (uint32_t *)&f;  
    uint32_t temp = *p;            
    bytes[0] = (temp >> 0) & 0xFF;
    bytes[1] = (temp >> 8) & 0xFF; 
    bytes[2] = (temp >> 16) & 0xFF;
    bytes[3] = (temp >> 24) & 0xFF; 
 }

void data_receive(uint8_t data[8])
{
	
//	double_2.setPower = data[0]; 
//	
//	double_2.cacheEnergy = (data[1] << 0)|(data[2] << 8);   

//	double_2.cacheEnergylimit = (data[3] << 0)|(data[4] << 8);
	cap.remain_vol = bytes_to_float(&data[0]);
	
} 

float bytes_to_float(uint8_t *bytes) 
{
    uint32_t temp = 0;
    temp |= (bytes[0] << 0);  
    temp |= (bytes[1] << 8);  
    temp |= (bytes[2] << 16);
    temp |= (bytes[3] << 24); 
    return *(float *)&temp;   
}
