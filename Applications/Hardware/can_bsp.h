#include "can.h"
#include "stdint.h"
#include "struct_typedef.h"

#ifndef CAN_BSP_H_
#define CAN_BSP_H_


// 使用6020电机的ID5-7，添加其解析，一般不建议使用
// #define USE_CAN_1_6020
// #define USE_CAN_2_6020
#define USE_FREERTOS_DELAY
// #define USE_NOP_DELAY



typedef enum
{
  CAN_1_1 = 0,
  CAN_1_2,
  CAN_1_3,
  CAN_1_4,
  CAN_1_5,
  CAN_1_6,
  CAN_1_7,
  CAN_1_8,
  CAN_1_6020_5,
  CAN_1_6020_6,
  CAN_1_6020_7,

  CAN_2_1,//11
  CAN_2_2,//12
  CAN_2_3,
  CAN_2_4,
  CAN_2_5, //15
  CAN_2_6,
  CAN_2_7, //17
  CAN_2_8,
  CAN_2_6020_5,
  CAN_2_6020_6,
  CAN_2_6020_7,
  
} can_id;

// rm motor data
// notice:默认为3508电机
// 3508/2006用的是电流，6020用的是电压
typedef struct
{
  int16_t set; // 设定的电流 / 电压

  // 原始数据
  uint16_t ecd;          // 编码器数值
  int16_t speed_rpm;     // 转速
  int16_t given_current; // 电调给的电流
  uint8_t temperate;     // 温度（获取不到）
  int16_t last_ecd;      // 上一次编码器的数值

  // 计算数据
  long long ecd_cnt;  // 编码器计数器
  double angle_cnt;   // 转过的总角度 degree
  double angle_zero;  // 编码器0点角度 degree
  double angle;       // -180~180 degree
  double round_speed; // 出轴旋转速度 degree/s
	
} motor_measure_t;
motor_measure_t get_motor_data(can_id motorID); // 获取马达信息
extern motor_measure_t motor_data[22];
//外部调用
void canx_send_data(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t *data, uint32_t len);

#endif

