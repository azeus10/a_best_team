#include "stdint.h"
#include "struct_typedef.h"
#define _RAMPFUNC_H_
#ifdef  _RAMPFUNC_H_


#pragma anon_unions

// 定义一个用于表示斜坡发生器状态的结构体
 typedef struct RampGenerator
{
    float currentValue; // 当前值
    float targetValue;  // 目标值
    float step;         // 每个控制周期应当改变的数值大小
    int  isBusy;        // 指示斜坡发生器是否正在调整中
} RampGenerator;
typedef __packed struct
{
    fp32 input;        //输入数据
    fp32 out;          //滤波输出的数据
    fp32 num[1];       //滤波参数
    fp32 frame_period; //滤波的时间间隔 单位 s
} first_order_filter_type_t;
// 一个周期内对斜坡发生器状态的更新
void rampIterate(RampGenerator *ramp);
// 初始化斜坡发生器
void rampInit(RampGenerator *ramp, float startValue, float targetValue, float time, float cycleTime);

//一阶滤波初始化
extern void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, fp32 frame_period, const fp32 num[1]);
//一阶滤波计算
extern void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, fp32 input);

#endif

// end of file
