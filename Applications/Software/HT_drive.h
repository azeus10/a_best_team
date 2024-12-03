#ifndef HT_DRIVE_H
#define HT_DRIVE_H
#include "main.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal.h"
#include "Stm32_time.h"
#include "gimbal.h"
#include "can.h"
#include <math.h>  
extern uint8_t  Offset_judge;
extern uint16_t Get_Angle;
extern int32_t GET_Total_Angle;
extern int16_t GET_speed;
extern int last_HT_time;
extern int lock_power;
void HT_CAN_location_TX(uint32_t location);
void HT_CAN_speed_TX( int16_t speed);
void HT_CAN_location_speed_set_TX(uint32_t speed_set);
void HT_CAN_power_TX(uint32_t power);
void HT_CAN_offset_TX(void);
void HT_CAN_offset_RX(uint8_t aData[]);
void CAN_ID_matching(uint32_t StdId ,uint8_t aData[]);
void HT_small_pitch(void);
#endif
