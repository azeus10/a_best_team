/**
 * @file chassis_move.h
 * @author sethome
 * @brief 底盘控制文件
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "struct_typedef.h"
#include "referee_handle_pack.h"

#define __chassis_move_H_
#ifdef __chassis_move_H_
#define chassis_FR CAN_1_1
#define chassis_FL CAN_1_2
#define chassis_BL CAN_1_3
#define chassis_BR CAN_1_4

struct chassis_status
{
	struct
	{
		float x, y, r;
		float now_x, now_y, now_r;
		float last_x, last_y, last_r;
		float max_x, max_y, max_r; // m/s
	} speed;

	struct
	{
		float now_x, now_y, now_r;
		float max_x, max_y, max_r; // m/s^2
	} acc;

	int16_t is_open_cap; //超电置位标志

	int16_t wheel_current[4]; // PID输出的电调电流
};

struct cap
{
	float cap_vol;
	float cap_now_vol;
	float cap_last_vol;
	float cap_error_vol;
};

extern struct chassis_status chassis;
extern struct cap cap_chassis;
extern ext_power_heat_data_t power_heat_data;
extern float Power;
extern float Plimit;
extern int fly_mode;
extern float CHASSIS_WZ_SET_SCALE;
void chassis_move_init(void);		// 底盘初始化							//底盘移动初始化
void chassis_moto_speed_calc(void); //计算马达速度
void val_limit(float *val, float MAX);
void chassis_power_limit(void);

#endif


// end of file
