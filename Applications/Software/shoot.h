/**
 * @file shoot.h
 * @author sethome
 * @brief 发射模块
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "stdint.h"
#include "struct_typedef.h"
#define _SHOOT_H_
#ifdef _SHOOT_H_

#define USE_3508_AS_SHOOT_MOTOR
//摩擦轮电机标识 从枪管正面看
//Can id值
//   1
// 2   3     
// 拨弹电机配置
#define TRIGGER_MOTOR CAN_1_7
// #define A_BULLET_ANGEL -25502
#define A_BULLET_ANGEL -26225

// 摩擦轮电机配置 3508
#ifdef USE_3508_AS_SHOOT_MOTOR

#define SHOOT_MOTOR1 CAN_2_1
#define SHOOT_MOTOR2 CAN_2_3
#define SHOOT_MOTOR3 CAN_2_2

#endif

#pragma anon_unions

#define FULL_BULLETS 250

enum trigger_status_e // 拨弹轮控制模式
{
    LOCATIONS = 0,
    SPEEDS,
};

typedef struct
{
    /* data */
    //设定弹速
    float SetSpeedUp;
    float SetSpeedLeft;
    float SetSpeedRight;

    int remainingBullets;

    // 电机数据
    float SpeedUp; // 摩擦轮速度
    float SpeedLeft;
  	float SpeedRight;
    struct
    {
        /* data */
        float now;
        float set;
        float last_set;
        float last_now;
        float off_set;
    } trigger_location; // 拨弹电机位置

    float trigger_speed; // 拨弹电机速度
    float set_trigger_speed;
    float trigger_given_current;

    enum trigger_status_e trigger_status;
		enum trigger_status_e last_status;
		
		int compensate_angle;
		
		float speedUpLevel;
		float speedLeftLevel;
		float speedRightLevel;
} shoot_t;

extern shoot_t shoot;

//extern uint8_t trigger_cnt_auto;

// 外部调用
void shoot_init(void); // 初始化

void shoot_update(void); // 更新拨弹电机速度等

void shoot_pid_cal(void); // 计算pid


//void shoot_speed_limit(void); // 依据裁判系统更新速度限幅

//void trigger_anti_kill_and_set_speed(float set);

//void shoot_trigger_online(void); // PC操作

int shoot_Bullets(int n); // 发射N颗子弹

void shoot_reload(void); // 子弹重新计数

void shoot_set_back(void);

#endif

// end of file
