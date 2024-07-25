#include <ui.h>
#include "referee_handle_pack.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Global_status.h"
#include "chassis_move.h"
#include "gimbal.h"
#include "CAN_receive&send.h"
#include "shoot.h"

void ui_init(){
  ui_self_id=get_robot_id();
  osDelay(20);
  _ui_init_default_Ungroup_0();
  osDelay(20);		
  _ui_init_default_Ungroup_1();
	osDelay(20);		
  _ui_init_default_Ungroup_2();
	osDelay(20);		
  _ui_init_default_Ungroup_3();
	osDelay(20);		
  _ui_init_default_Ungroup_4();
  osDelay(20);		
  _ui_init_default_Ungroup_5();
	osDelay(20);		
}

void ui_updata(){
  _ui_update_default_Ungroup_4();
  osDelay(2);
  _ui_update_default_Ungroup_0();
  osDelay(2);
  _ui_update_default_Ungroup_1();
  osDelay(2);
  _ui_update_default_Ungroup_2();
  osDelay(2);
  _ui_update_default_Ungroup_3();
  osDelay(2);
  _ui_update_default_Ungroup_5();
  osDelay(2);
}

void ui_supercap(float votage){
  float cnt=votage*votage;
  float percent=(cnt-49.0f)/480.0f;
  if(percent>=1)
    percent=1;
  if(percent<=0)
    percent = 0;
  if(percent >=0.6)
    ui_default_Ungroup_EnergyLine->color = 2;//绿色
  if(percent<=0.6)
    ui_default_Ungroup_EnergyLine->color = 3;//橙色
  if(percent <=0.3)
    ui_default_Ungroup_EnergyLine->color = 4;//紫红色
  ui_default_Ungroup_EnergyLine->end_x = ui_default_Ungroup_EnergyLine->start_x +795.0f*percent;
  
  if(Global.cap != FULL){
    ui_default_Ungroup_energycircle->color=6;
    ui_default_Ungroup_energycircle->width=1;
  }
  else{
    ui_default_Ungroup_energycircle->color=1;
    ui_default_Ungroup_energycircle->width=8;

  }
}


void char_change()
{
  //飞坡模式
  if(fly_mode !=1)
  {
    ui_default_Ungroup_Fly->color=7;
    ui_default_Ungroup_Fly->font_size=30;
  }
  else
  {
    ui_default_Ungroup_Fly->color=2;
    ui_default_Ungroup_Fly->font_size=30;
  }
   //摩擦轮
  if(Global.input.shooter_status != 1){
    ui_default_Ungroup_Shoot->color=7;
    ui_default_Ungroup_Shoot->font_size=30;
  }
  else
  {
    ui_default_Ungroup_Shoot->color=2;
    ui_default_Ungroup_Shoot->font_size=30;
  }

  //吊射
  if(Global.mode != LEAN_LOB)
  {
    ui_default_Ungroup_Lob->color=7;
    ui_default_Ungroup_Lob->font_size=30;
  }

  else{
    ui_default_Ungroup_Lob->color=2;
    ui_default_Ungroup_Lob->font_size=30;
  }
  //卡弹
  if(Global.input.shooter_status == 1 && get_motor_data(SHOOT_MOTOR1).speed_rpm<10 && get_motor_data(SHOOT_MOTOR2).speed_rpm<10
    && get_motor_data(SHOOT_MOTOR3).speed_rpm<10)
  {
    ui_default_Ungroup_LALE->color=2;
    ui_default_Ungroup_LALE->font_size=30;
  }
  else{ 
    ui_default_Ungroup_LALE->color=7;
    ui_default_Ungroup_LALE->font_size=30;
  }
  if(shoot.trigger_location.now > shoot.trigger_location.set + 28225)
    {
    ui_default_Ungroup_LALE->color=2;
    ui_default_Ungroup_LALE->font_size=30;
  }
    else{ 
    ui_default_Ungroup_LALE->color=7;
    ui_default_Ungroup_LALE->font_size=30;
  }
}

void ui_chassis(float angle){
    float angle_start,angle_end;
    if(angle>360)
      angle-=360;
    angle_start=angle-20.0f;
    angle_end=angle+20.0f;
    if(angle_start<=0.0f)
      angle_start+=360.0f;
    if(angle_end<=0.0f)
      angle_end+=360.0f;

    if(angle_end>=360.0f)
      angle_end-=360.0f;
    if(angle_start>=360.0f)
      angle_start-=360.0f;
    ui_default_Ungroup_Chassis->start_angle=angle_start;
    ui_default_Ungroup_Chassis->end_angle=angle_end;
    
    

}


// void ui_auto(uint8_t status){//自瞄圈与自瞄框
//    switch (status)//自瞄圈
// 	 {
// 		 case 0:		 
//       ui_default_Ungroup_AutoRound->color=4;//紫红色
// 			break;
// 		 case 1:
//       ui_default_Ungroup_AutoRound->color=3;//橙色
// 			break;
// 		 case 2:
//       ui_default_Ungroup_AutoRound->color=2;//绿色
// 			break;
// 			default:
//       ui_default_Ungroup_AutoRound->color=4;//紫红色
// 			break;			
// 	 }	
//    if(Global.input.vision_status!=1)
//      ui_default_Ungroup_AutoRect->color=8;
//    else
//      ui_default_Ungroup_AutoRect->color=2;
//    if(Global.input.vision_online==0){//自瞄掉线
//      ui_default_Ungroup_AutoRound->color=4;//紫红色
//      ui_default_Ungroup_AutoRect->color=7;
//    }
// }

void ui_pitch_angle(float angle)
{
  ui_default_Ungroup_pitch_angle->number = angle;
}

void ui_chassisline(){
  if(fly_mode ==1){
    ui_default_Ungroup_MoveLineleft->end_y = 408;
    ui_default_Ungroup_MoveLineright->end_y = 408;
  }else{
    ui_default_Ungroup_MoveLineleft->end_y = 1;
    ui_default_Ungroup_MoveLineright->end_y = 1;
  }
}