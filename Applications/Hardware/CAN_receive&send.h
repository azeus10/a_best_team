/**
 * @file CAN_receive&send.h
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2021-12-09
 *
 * @copyright Copyright (c) 2021 sethome
 *
 */

#ifndef __CAN_RECEIVE_H__
#define __CAN_RECEIVE_H__

#include "struct_typedef.h"
#include "main.h"
#include "can_bsp.h"
/* CAN send and receive ID */
typedef enum
{
  CAN_1_4_SIGN_ID = 0x200,//发送帧头
  CAN_ID1 = 0x201,//接收帧头
  CAN_ID2 = 0x202,
  CAN_ID3 = 0x203,
  CAN_ID4 = 0x204,

  CAN_5_8_SIGN_ID = 0x1FF,
  CAN_ID5 = 0x205,
  CAN_ID6 = 0x206,
  CAN_ID7 = 0x207,
  CAN_ID8 = 0x208,

  CAN_6020_SIGN_ID = 0x2FF,
  CAN_6020_ID5 = 0x209,
  CAN_6020_ID6 = 0x210,
  CAN_6020_ID7 = 0x211,
} can_msg_id_e;

// 马达基础参数
#define ECD_MAX 8192.0f    // 编码器最大值
#define M3508_P 19.0f      //13.7f      // M3508电机减速比
#define M2006_P 36.0f      // M2006电机减速比
#define MAX_CURRENT 16384  // M2006+M3508最大电流 20A / MAX_CURRENT
#define MAX_6020_VOL 30000 // 6020最大电压 24V / MAX_6020_VOL


// 外部调用
void CAN1_send_current(void);                   // 发送电机控制电流
void CAN2_send_current(void);                   // 发送电机控制电流
void set_motor(int16_t val, can_id motorID);    // 设定马达电流/电压

// 对马达信息二次解码（计算），使用部分计算数据前使用，由于编码器0点，可能在高速下不准确！！！
void decode_as_3508(can_id motorID);
void decode_as_2006(can_id motorID);
void decode_as_6020(can_id motorID);
void decode_as_3508_rotor(can_id motorID);
void set_motor_offset(can_id motorID, float angle);
void clear_motor_cnt(can_id motorID);
void decode_as_6020_test(can_id motorID);

void CAN1_send_ZERO_current(void);
void CAN2_send_ZERO_current(void);

void DJI_CAN1_handle_message(uint32_t StdId,uint8_t *data);
void DJI_CAN2_handle_message(uint32_t StdId,uint8_t *data);


#endif
