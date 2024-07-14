#include "math.h"
#include "pid.h"
#include <stdio.h>
//低通滤波
double lowPassFilter(double input, double prevOutput, double alpha)
{
    return alpha * input + (1 - alpha) * prevOutput;
}

void pid_set(pid_t *PidSet,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset)//PID设置
{
  PidSet->p = p_set;   PidSet->i = i_set;   PidSet->d = d_set;
	// PidSet->p_out = 0.0f;
	// PidSet->i_out = 0.0f;
	// PidSet->d_out = 0.0f;
  // PidSet->total_out = 0.0f;
  // PidSet->set = 0.0f;
  PidSet->lim_out = lim_out_set;   PidSet->lim_i_out = lim_i_outset;//将设置赋值
}

//PID计算
float pid_cal(pid_t *PidGoal,float Now,float Set)//PID
{
	PidGoal->set = Set;
  PidGoal->err_last = PidGoal->err;
  if(PidGoal->d < 7000)
  PidGoal->derr_last = PidGoal->err_last;
  PidGoal->err = Set - Now;//计算误差
//  if(fabs(PidGoal->err)<0.01)
//	{
//		PidGoal->err=0;
//	}
  PidGoal->p_out = PidGoal->p * PidGoal->err;
	if(PidGoal->i != 0)
		PidGoal->i_out += PidGoal->i * PidGoal->err;
  if(PidGoal->d > 7000)
  {
    PidGoal->derr_now = lowPassFilter(PidGoal->err,PidGoal->derr_last,0.2);
  }
  else
  {
    PidGoal->derr_now = PidGoal->err;
  }
  PidGoal->d_out = PidGoal->d * (PidGoal->derr_now - PidGoal->derr_last);//pid运算
  if(PidGoal->d > 7000)
  {
    PidGoal->derr_last = PidGoal->derr_now;
  }
  if(fabs(PidGoal->i_out) > PidGoal->lim_i_out)//防止积分过大
  {
    if(PidGoal->i_out < 0)
      PidGoal->i_out = -PidGoal->lim_i_out;
    else
      PidGoal->i_out = PidGoal->lim_i_out;
  }
	
  PidGoal->total_out = PidGoal->p_out + PidGoal->i_out + PidGoal->d_out;//计算总和输出  

	if(fabs(PidGoal->total_out) > PidGoal->lim_out)//防止总和过大
  {
    if(PidGoal->total_out < 0)
      PidGoal->total_out = -PidGoal->lim_out;
    else
      PidGoal->total_out = PidGoal->lim_out;
  }
	
  return PidGoal->total_out;
}
