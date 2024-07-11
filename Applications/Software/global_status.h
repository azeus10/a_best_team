/**
 * @file global_status.h
 * @author sethome
 * @brief 全局状态机
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright sethome Copyright (c) 2022
 *
 */
#include "stdint.h"

#define __GLOBAL_STATUS_H__
#ifdef __GLOBAL_STATUS_H__

// 错误码
enum err_e
{
    GIMBAL_ERR = 0,
    CHASSIS_ERR,
    SHOOT_ERR,
    CAP_ERR,
    REMOTE_ERR,
    PC_ERR,
};
enum ctl_e
{
	RC=0,
	PC,
};
// 适合十几个简单状态的情况
struct GlobalStatus_t
{
    uint8_t err[6];
	
    enum mode_e
    {
        LOCK = 0,
        FLOW,
        LEAN_LOB,//超级对抗赛专用，倾斜+吊射模式
        SPIN,
		    LEAN,
    } mode;

    enum cap_e
    {
        STOP = 0, // 此时电容应在充电
        FULL,     // 全力响应
    } cap;		
			
    struct
    {
			/* data */
			float x, y, r; // 底盘移动
			float pitch, yaw;
			uint8_t vision_status;
			enum ctl_e ctl;
			uint8_t shooter_status;
			uint8_t shoot_fire;	
      uint8_t isHeatLimit;	
			uint8_t isOnForce;
			uint8_t ScopeisOpen;
			uint8_t Subgimbalisfollow;
			uint8_t fly;
    } input;

};

extern struct GlobalStatus_t Global;
//全局状态机初始化
void Global_status_init();
void Global_set_err(enum err_e err, uint8_t status);

#endif
// end of file
