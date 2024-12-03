#ifndef __DM4310_DRV_H__
#define __DM4310_DRV_H__
#include "main.h"
#include "CAN_receive&send.h"
#include "struct_typedef.h"


#define MIT_MODE 			0x001
#define POS_MODE			0x101
#define SPEED_MODE			0x201

#define P_MIN -3.1415926f
#define P_MAX 3.1415926f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define T_MIN -10.0f
#define T_MAX 10.0f

void DM_enable_TX(can_id ID);
void DM_disable_TX(can_id ID);
void DM_CAN_location_TX(can_id ID,float _pos,float _vel);
void DM_CAN_speed_TX(can_id ID,float _vel);
void DM_CAN_Callback(can_id ID,uint8_t data[8]);

//外部调用
void DM_Init(void);//达妙电机初始化函数
void DM_Motor_set(can_id Motor_ID,float pos,float vel);//点击设置函数

typedef struct 
{
	uint16_t id;
	uint16_t state;
	int	p_int;
	int	v_int;
	int	t_int;
	
	float Tmos;
	float Tcoil;	
	float	position; 
	float	velocity;
	float	torque; //温度
} DM_can_recive_message;

#endif /* __DM4310_DRV_H__ */



