/*
 领控电机数据处理
*/

#ifndef __LK_MOTOR_PROCESS_H_
#define __LK_MOTOR_PROCESS_H_

#include "struct_typedef.h"
#include "main.h"

// 使用6020电机的ID5-7，添加其解析，一般不建议使用
// #define USE_CAN_1_6020
// #define USE_CAN_2_6020
#define USE_FREERTOS_DELAY
// #define USE_NOP_DELAY
//单电机指令集
typedef enum
{
  Open_loop_control=0xA0,                                //开环控制
	Torque_closed_loop_control,                            //转矩闭环控制
	Speed_closed_loop_control,                             //速度闭环控制
	Multi_turn_position_closed_loop_control,               //多圈位置闭环控制命令
	Multi_turn_position_speed_closed_loop_control,         //多圈位置速度闭环控制命令
	Single_turn_position_closed_loop_control,              //单圈位置闭环控制命令
	Single_turn_position_speed_closed_loop_control,        //单圈位置速度闭环控制命令
	Incremental_position_closed_loop_control,              //增量位置闭环控制命令
	Incremental_position_speed_closed_loop_control,        //增量位置速度闭环控制命令
} sigle_motor_mode_cmd_set;  
typedef struct 
{
	sigle_motor_mode_cmd_set mode;
	uint16_t maxSpeed;
  uint32_t angleControl;
	uint32_t speed;

} LK_can_send_message;
typedef struct 
{
	uint8_t cmd;          //命令
	uint8_t temperature;  //温度
	uint16_t power;       //转矩电流
	uint16_t speed;       //电机速度
	uint16_t encoder;     //编码器位置
	
	uint32_t circleAngle; //单圈角度
} LK_can_recive_message;


extern LK_can_recive_message LK_motor_data;
extern LK_can_send_message   LK_control;
void LK_handle_message(uint8_t data[8]);
void LK_send_message(void);
void LK_motor_control(void);




#endif
