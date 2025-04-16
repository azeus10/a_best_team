
#include "RM_Cilent_UI.h"
#include "UART_data_transmit.h"
#include "string.h"
#include "stdio.h"
#include "crc8_crc16.h"
#include "cap_ctl.h"
#include "shoot.h"
#include "math.h"
#include "cmsis_os.h"
#include "referee_handle_pack.h"
#include "Global_status.h"
#include "CAN_receive&send.h"
#include "IMU_updata.h"
#include "chassis_move.h"

#define Root_2 1.4142136f

uint8_t UI_Seq, press_refrsh_flag;       // 包序号
Graph_Data G1, G2, G3, G4, G5, G6, G7;   // 瞄准线UI 01
String_Data G8, G13, G14, G15, G16, G17; // 指示状态用UI

Graph_Data G18; // 自瞄框UI

Graph_Data G19; // 车辆指向UI
Graph_Data G20;
Graph_Data G21;
Graph_Data G22;
Graph_Data G23; // 云台指向竖直向上UI

String_Data G24; //指示状态UI
Graph_Data G25, G26, G27, G28, G29, G30, G31;
//测距信息显示
String_Data  G32,G33,G34,G35;
//瞄准线UI显示
Graph_Data G36,G37,G38,G39,G40,G41,G42;


uint16_t Robot_ID = UI_Data_RobotID_RStandard1; // ID默认红1
uint16_t Cilent_ID = UI_Data_CilentID_RStandard1;

void UI_task_init(void)
{
   memset(&G1, 0, sizeof(G1));
   memset(&G2, 0, sizeof(G2));
   memset(&G3, 0, sizeof(G3));
   memset(&G4, 0, sizeof(G4));
   memset(&G5, 0, sizeof(G5));
   memset(&G6, 0, sizeof(G6));
   memset(&G7, 0, sizeof(G7));
   memset(&G8, 0, sizeof(G8));
   memset(&G13, 0, sizeof(G13));
   memset(&G14, 0, sizeof(G14));
   memset(&G15, 0, sizeof(G15));
//   memset(&G16, 0, sizeof(G16));
   memset(&G17, 0, sizeof(G17));
   memset(&G18, 0, sizeof(G18));
   memset(&G19, 0, sizeof(G19));
   memset(&G20, 0, sizeof(G20));
   memset(&G21, 0, sizeof(G21));
   memset(&G22, 0, sizeof(G22));
   memset(&G23, 0, sizeof(G23));
	 memset(&G32, 0, sizeof(G32));
	
	 memset(&G33, 0, sizeof(G33));
   memset(&G34, 0, sizeof(G34));
	 memset(&G35, 0, sizeof(G35));
	 
//	 memset(&G36, 0, sizeof(G36));
//	 memset(&G37, 0, sizeof(G37));
//	 memset(&G38, 0, sizeof(G38));
//	 memset(&G39, 0, sizeof(G39));
//	 memset(&G40, 0, sizeof(G40));
//	 memset(&G41, 0, sizeof(G41));
	 memset(&G42, 0, sizeof(G42));
	 
   infanty_hero_status_UI_add();
}

void infanty_hero_status_UI_add(void) // 手动按键添加UI
{
   char normal_arr[4] = "norm";
   char lean_arr[4]   = "lean";
   char spin_arr[4]   = "spin";
   char tank_arr[4]   = "tank";

   char shoot_arr[5]  = "Shoot";
   char HitLimitArr[8] = "HeatLimt";

   char power_buff[3] = "Cap";
   char super_cap_arr[4] = "CAP1";
	
	 char pitch_arry[5]="Pitch";
	 char distance_arry[3]  ="Dis";
	
	 char shootup_arry[5] ="shoot";
	 char shootleft_arry[5] ="shoot";
	 char shootright_arry[5] ="shoot";
  
	 /*图形信息*/
	 //yaw轴弹道
   Line_Draw(&G1, "001", UI_Graph_ADD, 9, UI_Color_Purplish_red, 2, 968, 1080-888, 968, 1080-401);
	 //R3高地吊前哨站
	 Line_Draw(&G2, "002", UI_Graph_ADD, 9, UI_Color_Main, 2, 968, 1080-491, 982, 1080-491); //8m
   Line_Draw(&G3, "003", UI_Graph_ADD, 9, UI_Color_Main, 2, 968, 1080-508, 977, 1080-508); //9m
   Line_Draw(&G4, "004", UI_Graph_ADD, 9, UI_Color_Main, 2, 968, 1080-532, 978, 1080-532);  //10m
   Line_Draw(&G5, "005", UI_Graph_ADD, 9, UI_Color_Main, 2, 968, 1080-546, 979, 1080-546);  //11m
	 //对面环高吊基地
   Line_Draw(&G6, "006", UI_Graph_ADD, 9, UI_Color_Green, 2, 968, 1080-474, 982, 1080-474); //8m
	 	//对面环高吊基地
   Line_Draw(&G7, "007", UI_Graph_ADD, 9, UI_Color_Orange, 2, 968, 1080-401,1000 , 1080-401); //停转前哨站
	 
   //前哨站井字线瞄准装甲板
// 	 Line_Draw(&G36, "040", UI_Graph_ADD, 1, UI_Color_Purplish_red, 2, 938, 1080-549, 1043, 1080-549); //水平1
// 	 Line_Draw(&G37, "037", UI_Graph_ADD, 1, UI_Color_Purplish_red, 2, 938, 1080-586, 1043, 1080-586); //水平2
//	 Line_Draw(&G38, "038", UI_Graph_ADD, 1, UI_Color_Purplish_red, 2, 965, 1080-522, 965, 1080-617); //竖直1
//	 Line_Draw(&G39, "039", UI_Graph_ADD, 1, UI_Color_Purplish_red, 2, 1018, 1080-522, 1018, 1080-617); //竖直2
//   //瞄准镜边缘线
//	 Arc_Draw(&G40, " 040", UI_Graph_ADD,  4, UI_Color_Green,60,120, 2,1000,1080-543,431,404);
//	 Arc_Draw(&G41, " 041", UI_Graph_ADD,  7, UI_Color_Green,240,300, 2,1000,1080-543,440,404);
   //底盘状态指示线
   Line_Draw(&G19, "019", UI_Graph_ADD, 3, UI_Color_Green, 2, 898, 120, 1018, 120); 
   Line_Draw(&G20, "020", UI_Graph_ADD, 3, UI_Color_Pink, 2, 898, 40, 898, 120);
   Line_Draw(&G21, "021", UI_Graph_ADD, 3, UI_Color_Pink, 2, 1018, 40, 898, 40);
   Line_Draw(&G22, "022", UI_Graph_ADD, 3, UI_Color_Pink, 2, 1018, 120, 1018, 40);
	 //云台指向线
   Line_Draw(&G23, "023", UI_Graph_ADD, 4, UI_Color_Cyan, 2, 1200, 100, 1200, 185); 
   /*字符信息*/
	 //底盘模式信息
   Char_Draw(&G8, "008", UI_Graph_ADD, 7, UI_Color_Orange, 18, 4, 2, 1200, 186,&normal_arr[0]); 
   //摩擦轮开关信息
   Char_Draw(&G13,"013", UI_Graph_ADD, 5, UI_Color_Orange, 12, 5, 2, 200, 700, &shoot_arr[0]);   
	 //热量限制开关信息
   Char_Draw(&G14,"014", UI_Graph_ADD, 5, UI_Color_Orange, 12, 8, 2, 200, 750, &HitLimitArr[0]); 
	 //超电电压信息
   Char_Draw(&G15,"015", UI_Graph_ADD, 5, UI_Color_Orange, 12, 4, 2, 1000, 234, &super_cap_arr[0]); 
	 //超电开关信息
   Char_Draw(&G16,"016", UI_Graph_ADD, 5, UI_Color_Orange, 18, 3, 2, 110, 820, &power_buff[0]);    // BUFF
	 //Pitch数据信息
	 Char_Draw(&G17,"017", UI_Graph_ADD, 5, UI_Color_Orange, 12, 5, 2, 200, 650, &pitch_arry[0]);     
	 //测距距离信息
	 Char_Draw(&G32,"032", UI_Graph_ADD, 5, UI_Color_Orange, 12, 3, 2, 200, 600, &distance_arry[0]);  
   //摩擦转速变化信息
	 Char_Draw(&G33,"033", UI_Graph_ADD, 5, UI_Color_Orange, 18, 3, 2, 90, 750, &shootup_arry[0]);     //上摩擦轮变化量
	 Char_Draw(&G34,"034", UI_Graph_ADD, 5, UI_Color_Orange, 18, 3, 2, 90, 700, &shootleft_arry[0]);   //左摩擦轮变化量
	 Char_Draw(&G35,"035", UI_Graph_ADD, 5, UI_Color_Orange, 18, 3, 2, 90, 650, &shootright_arry[0]);  //右摩擦轮变化量
}

void press_refrsh(void) // 开局手动添加UI固定准星
{
   UI_ReFresh(7, G1, G2, G3, G4, G5, G6, G7);
   osDelay(10);
	UI_ReFresh(1, G13);
	osDelay(10);
}
void CHASSIS_ReFresh(void) // 底盘图示状态刷新
{
   UI_ReFresh(5, G19, G20, G21, G22, G23);
   osDelay(10);
 	 UI_ReFresh(7, G36,G37, G38, G39, G40, G41, G42); 
   osDelay(10);
}
void auto_refresh(void) // 其他字符刷新
{                       // freertos里面刷新任务，必须要间歇性发送才可以，不然只能刷新一个
   Char_ReFresh(G8);
   osDelay(15);
   Char_ReFresh(G13);
   osDelay(15);
	 Char_ReFresh(G14);//热量限制
	 osDelay(10);
   Char_ReFresh(G15);
   osDelay(15);
   Char_ReFresh(G16);
   osDelay(15);
	 Char_ReFresh(G17);
   osDelay(15);
	 Char_ReFresh(G32);
   osDelay(15);
	 Char_ReFresh(G33);
   osDelay(15);
	 Char_ReFresh(G34);
   osDelay(15);
	 Char_ReFresh(G35);
   osDelay(15);
}
void Chassis_Refresh(float angle)                        // 底盘图示状态更新
{
   Line_Draw(&G19, "019", UI_Graph_Change, 3, UI_Color_Pink, 2, 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 186.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 186.5f)), 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 96.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 96.5f)));

   Line_Draw(&G20, "020", UI_Graph_Change, 3, UI_Color_Pink, 2, 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 276.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 276.5f)), 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 186.5)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 186.5)));

   Line_Draw(&G21, "021", UI_Graph_Change, 3, UI_Color_Green, 2, 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 6.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 6.5f)), 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 276.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 276.5f)));

   Line_Draw(&G22, "022", UI_Graph_Change, 3, UI_Color_Pink, 2, 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 96.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 96.5f)), 1200 + 40 * Root_2 * cosf(-degree2rad(angle + 6.5f)), 50 + 40 * Root_2 * sinf(-degree2rad(angle + 6.5f)));

   Line_Draw(&G23, "023", UI_Graph_ADD, 4, UI_Color_Cyan, 2, 1200, 50, 1200, 120); // 云台指向
}

void shoot_refresh(int16_t status)                      //摩擦轮开关信息更新
{

   char shoot_arr2[8] = "SHOOT";
   if (status > 1000) 
   {
      Char_Draw(&G13, "013", UI_Graph_Change, 5, UI_Color_Green, 18, 8, 2, 110, 880, &shoot_arr2[0]);
   }
   else
   {
      Char_Draw(&G13, "013", UI_Graph_Change, 5, UI_Color_Orange, 18, 8, 2, 110, 880, &shoot_arr2[0]);
   }
}
void HeatLimitRefresh(uint8_t status)                   //热量限制开关信息更新
{
	 char HitLimitArr2[8]   = "HeatLimt";

   if (Global.input.isHeatLimit==0)                                // 热量有限制
   {
      Char_Draw(&G14, "014", UI_Graph_Change, 5, UI_Color_Orange, 18, 8, 2, 110, 850, &HitLimitArr2[0]);
   }
   else if(Global.input.isHeatLimit==1)                            //热量无限制
   {
      Char_Draw(&G14, "014", UI_Graph_Change, 5, UI_Color_Green, 18, 8, 2, 110, 850, &HitLimitArr2[0]);
   }
}
void PitchYawRefresh(fp32 pitch,fp32 yaw)               //pitch数据更新
{
	 char Strff_pitch[6] = {0};
   float pitch_value = pitch;
   int len_pitch = snprintf(Strff_pitch, 6, " %lf", pitch_value );
	
	 
	 Char_Draw(&G17, "017", UI_Graph_Change, 5, UI_Color_Cyan , 18, 6, 2, 90, 790, &Strff_pitch[0]);

}
void ShootDeltaRefresh(float Up,float Left,float Right) //摩擦轮转速变化更新
{
	 char Strff_up[6] = {0};
	 char Strff_left[6] = {0};
	 char Strff_right[6] = {0};
	 
   float Up_value    =  Up;
	 float Left_value  =  Left;
	 float Right_value =  Right;
	 
   int len_up    =  snprintf(Strff_up,   6, " %lf", Up_value );
	 int len_left  =  snprintf(Strff_left, 6, " %lf", Left_value );
	 int len_right =  snprintf(Strff_right,6, " %lf", Right_value );
	 
	 
	 Char_Draw(&G33, "033", UI_Graph_Change, 5, UI_Color_Orange, 18, 6, 2, 90, 730, &Strff_up[0]);  //上摩擦轮变化量
	 Char_Draw(&G34, "034", UI_Graph_Change, 5, UI_Color_Orange, 18, 6, 2, 90, 700, &Strff_left[0]);  //左摩擦轮变化量
	 Char_Draw(&G35, "035", UI_Graph_Change, 5, UI_Color_Orange, 18, 6, 2, 90, 670, &Strff_right[0]);  //右摩擦轮变化量
	
}
void DistanceRefresh(float dis)                         //更新测距仪的数据
{
	char Strff_dis[6] = {0};
	float dis_value=dis;
  int len_dis = snprintf(Strff_dis, 6, " %lf", dis_value );

  Char_Draw(&G32, "032", UI_Graph_Change, 5, UI_Color_Purplish_red , 18, 6, 2, 90, 760, &Strff_dis[0]);
}
void chassis_status_refresh(uint8_t status)             //底盘模式切换更新
{
   char normal_arr[4] = "NORM";
   char lean_arr[4]   = "LEAN";
   char spin_arr[4]   = "SPIN";
   char tank_arr[4]   = "LOB ";
   char fly_arr[4] = "FLY";

   switch (status)
   {
   case FLOW:
      if(fly_mode == 1)
      Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 18, 4, 2, 1200, 186, &fly_arr[0]);
      else
      Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 18, 4, 2, 1200, 186, &normal_arr[0]);
      break;

   case LEAN:
      Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 18, 4, 2, 1200, 186, &lean_arr[0]);
      break;

   case SPIN:
      Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 18, 4, 2, 1200, 186, &spin_arr[0]);
      break;

   case LEAN_LOB:
      Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 18, 4, 2, 1200, 186, &tank_arr[0]);
      break;

   default:
      break;
   }
}
void Super_POWER(float cap_voltage)                     //电量更新
{
   char Strff[6] = {0};
   float X_Value = cap_voltage;
   int Len = snprintf(Strff, 6, "%lf", X_Value);

   uint8_t flag, last_flag;
   if (cap_voltage > 13.0f)
   {
      if (cap_voltage > 17.0f)
      {
         flag = 1;
      }
      else
      {
         flag = 2;
      }
   }
   else
   {
      flag = 3;
   }
   if (last_flag != flag)
   {
      switch (flag)
      {
      case 1:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Green, 18, 6, 2, 1200, 234, &Strff[0]);
         break;

      case 2:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Yellow, 18, 6, 2, 1200, 234, &Strff[0]);
         break;

      case 3:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Orange, 18, 6, 2, 1200, 234, &Strff[0]); // 低于12v，显示危险状态
         break;

      default:
         break;
      }
   }
   last_flag = flag; // 比一直给结构体赋值运行的快
}

void super_cap_speedup(bool_t yes_or_no) 								// BUFF更新
{
   char super_cap_up[3] = "Cap";
   if (yes_or_no)
      Char_Draw(&G16, "016", UI_Graph_Change, 5, UI_Color_Green, 18, 3, 2, 110, 820,  &super_cap_up[0]);
   else
      Char_Draw(&G16, "016", UI_Graph_Change, 5, UI_Color_Orange, 18, 3, 2, 110, 820, &super_cap_up[0]);
}
void UI_id_check(void)
{
   const ext_robot_status_t *robot_state = get_robot_state_point(); // 获取机器人id
   switch (robot_state->robot_id)
   {
   case 1: // 红1
      Robot_ID = UI_Data_RobotID_RHero;
      Cilent_ID = UI_Data_CilentID_RHero;
      break;
   case 2: // 红2
      Robot_ID = UI_Data_RobotID_REngineer;
      Cilent_ID = UI_Data_CilentID_REngineer;
      break;
   case 3: // 红3
      Robot_ID = UI_Data_RobotID_RStandard1;
      Cilent_ID = UI_Data_CilentID_RStandard1;
      break;
   case 4: // 红4
      Robot_ID = UI_Data_RobotID_RStandard2;
      Cilent_ID = UI_Data_CilentID_RStandard2;
      break;
   case 5: // 红5
      Robot_ID = UI_Data_RobotID_RStandard3;
      Cilent_ID = UI_Data_CilentID_RStandard3;
      break;
   case 6: // 红6
      Robot_ID = UI_Data_RobotID_RAerial;
      Cilent_ID = UI_Data_CilentID_RAerial;
      break;
   case 101: // 蓝一
      Robot_ID = UI_Data_RobotID_BHero;
      Cilent_ID = UI_Data_CilentID_BHero;
      break;
   case 102: // 蓝二
      Robot_ID = UI_Data_RobotID_BEngineer;
      Cilent_ID = UI_Data_CilentID_BEngineer;
      break;
   case 103: // 蓝三
      Robot_ID = UI_Data_RobotID_BStandard1;
      Cilent_ID = UI_Data_CilentID_BStandard1;
      break;
   case 104: // 蓝四
      Robot_ID = UI_Data_RobotID_BStandard2;
      Cilent_ID = UI_Data_CilentID_BStandard2;
      break;
   case 105: // 蓝五
      Robot_ID = UI_Data_RobotID_BStandard3;
      Cilent_ID = UI_Data_CilentID_BStandard3;
      break;
   case 106: // 蓝六
      Robot_ID = UI_Data_RobotID_BAerial;
      Cilent_ID = UI_Data_CilentID_BAerial;
      break;
   }
}

/********************************************删除操作*************************************
**参数：Del_Operate  对应头文件删除操作
        Del_Layer    要删除的层 取值0-9
*****************************************************************************************/
// 没写好，没有清除ui的必要，所以别用了
void UI_Delete(uint8_t Del_Operate, uint8_t Del_Layer)
{
   uint8_t cache[100];
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值
   UI_Packhead framehead;
   UI_Data_Operate datahead;
   UI_Data_Delete del;

   framepoint = (uint8_t *)&framehead;

   framehead.SOF = UI_SOF;
   framehead.Data_Length = 8;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据

   datahead.Data_ID = UI_Data_ID_Del;
   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据

   del.Delete_Operate = Del_Operate;
   del.Layer = Del_Layer; // 控制信息

   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail);
   framepoint = (uint8_t *)&del;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(del), frametail); // CRC16校验值计算

   memcpy(cache, &framehead, sizeof(framehead));
   UART_send_data(UART6_data, cache, sizeof(framehead));
   framepoint = (uint8_t *)&datahead;
   memcpy(cache, &datahead, sizeof(datahead));
   UART_send_data(UART6_data, cache, sizeof(datahead));
   memcpy(cache, &del, sizeof(del));
   UART_send_data(UART6_data, cache, sizeof(del));
   memcpy(cache, &frametail, sizeof(frametail));
   UART_send_data(UART6_data, cache, sizeof(frametail));

   UI_Seq++; // 包序号+1
}

/************************************************绘制直线*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标
**********************************************************************************************************/

void Line_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x, uint32_t End_y)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************绘制矩形*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标（对顶角坐标）
**********************************************************************************************************/

void Rectangle_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x, uint32_t End_y)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Rectangle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************绘制整圆*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    圆心坐标
        Graph_Radius  图形半径
**********************************************************************************************************/

void Circle_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t Graph_Radius)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Circle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->radius = Graph_Radius;
}
/************************************************绘制圆弧*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_StartAngle,Graph_EndAngle    开始，终止角度
        Start_y,Start_y    圆心坐标
        x_Length,y_Length   x,y方向上轴长，参考椭圆
**********************************************************************************************************/

void Arc_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_StartAngle, uint32_t Graph_EndAngle, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t x_Length, uint32_t y_Length)
{
   int i;

   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Arc;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_StartAngle;
   image->end_angle = Graph_EndAngle;
   image->end_x = x_Length;
   image->end_y = y_Length;
}

///************************************************绘制浮点型数据*************************************************
//**参数：*image Graph_Data类型变量指针，用于存放图形数据
//        imagename[3]   图片名称，用于标识更改
//        Graph_Operate   图片操作，见头文件
//        Graph_Layer    图层0-9
//        Graph_Color    图形颜色
//        Graph_Width    图形线宽
//        Graph_Size     字号
//        Graph_Digit    小数位数
//        Start_x、Start_x    开始坐标
//        Graph_Float   要显示的变量
//**********************************************************************************************************/
//// 不要用，咱们浮点数使用snprintf转化为字符串，这个我没时间测试了。
// void Float_Draw(Float_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, float Graph_Float)
//{
//    int i;

//   for (i = 0; i < 3 && imagename[i] != 0; i++)
//      image->graphic_name[2 - i] = imagename[i];
//   image->graphic_tpye = UI_Graph_Float;
//   image->operate_tpye = Graph_Operate;
//   image->layer = Graph_Layer;
//   image->color = Graph_Color;
//   image->width = Graph_Width;
//   image->start_x = Start_x;
//   image->start_y = Start_y;
//   image->start_angle = Graph_Size;
//   image->end_angle = Graph_Digit;
//   image->graph_Float = Graph_Float;
//}

/************************************************绘制字符型数据*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_Size     字号
        Graph_Digit    字符个数
        Start_x、Start_x    开始坐标
        *Char_Data          待发送字符串开始地址
**********************************************************************************************************/
void Char_Draw(String_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, char *Char_Data)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->Graph_Control.graphic_name[2 - i] = imagename[i];
   image->Graph_Control.graphic_tpye = UI_Graph_Char;
   image->Graph_Control.operate_tpye = Graph_Operate;
   image->Graph_Control.layer = Graph_Layer;
   image->Graph_Control.color = Graph_Color;
   image->Graph_Control.width = Graph_Width;
   image->Graph_Control.start_x = Start_x;
   image->Graph_Control.start_y = Start_y;
   image->Graph_Control.start_angle = Graph_Size;
   image->Graph_Control.end_angle = Graph_Digit;

   for (i = 0; i < Graph_Digit; i++)
   {
      image->show_Data[i] = *Char_Data;
      Char_Data++;
   }
}

/************************************************UI推送函数（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数


Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int UI_ReFresh(int cnt, ...)
{
   uint8_t cache[200]; // 声明一个缓冲数组
   int i;
   Graph_Data imageData;
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值

   UI_Packhead framehead;
   UI_Data_Operate datahead;

   va_list ap;
   va_start(ap, cnt);
   memset(cache, 0, sizeof(cache));

   framepoint = (uint8_t *)&framehead;
   framehead.SOF = UI_SOF;
   framehead.Data_Length = 6 + cnt * 15;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据
   switch (cnt)
   {
   case 1:
      datahead.Data_ID = UI_Data_ID_Draw1;
      break;
   case 2:
      datahead.Data_ID = UI_Data_ID_Draw2;
      break;
   case 5:
      datahead.Data_ID = UI_Data_ID_Draw5;
      break;
   case 7:
      datahead.Data_ID = UI_Data_ID_Draw7;
      break;
   default:
      return (-1);
   }
   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据
   framepoint = (uint8_t *)&framehead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail); // CRC16校验值计算（部分）
   memcpy(cache, &framehead, sizeof(framehead));                                // 首先填充帧头包
   memcpy(cache + sizeof(framehead), &datahead, sizeof(datahead));              // 其次填充数据帧头包
   for (i = 0; i < cnt; i++)                                                    // 使用遍历，把图形数据填充进来
   {
      imageData = va_arg(ap, Graph_Data);
      framepoint = (unsigned char *)&imageData;
      frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(imageData), frametail);
      memcpy(cache + sizeof(framehead) + sizeof(datahead) + i * sizeof(imageData), &imageData, sizeof(imageData)); // 循环，第一次填充偏移帧头包和数据帧头包，i=0，之后偏移i*添加的量大小
   }
   memcpy(cache + sizeof(framehead) + sizeof(datahead) + cnt * sizeof(imageData), &frametail, sizeof(frametail));
   HAL_UART_Transmit_DMA(UART6_data.huart, cache, sizeof(framehead) + sizeof(datahead) + cnt * sizeof(imageData) + sizeof(frametail));
   va_end(ap);
   UI_Seq++; // 包序号+1
   return 0;
}

/************************************************UI推送字符（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数
Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int Char_ReFresh(String_Data string_Data)
{
   uint8_t cache[200]; // 声明一个缓冲数组
   String_Data imageData;
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值

   UI_Packhead framehead;
   UI_Data_Operate datahead;
   imageData = string_Data;

   framepoint = (uint8_t *)&framehead;
   framehead.SOF = UI_SOF;
   framehead.Data_Length = 6 + 45;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据

   datahead.Data_ID = UI_Data_ID_DrawChar; // 有待争议

   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据

   framepoint = (uint8_t *)&framehead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail);
   framepoint = (uint8_t *)&imageData;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(imageData), frametail); // CRC16校验   //CRC16校验值计算（部分）

   memcpy(cache, &framehead, sizeof(framehead));
   memcpy(cache + sizeof(framehead), &datahead, sizeof(datahead));
   memcpy(cache + sizeof(framehead) + sizeof(datahead), &imageData, sizeof(imageData));
   memcpy(cache + sizeof(framehead) + sizeof(datahead) + sizeof(imageData), &frametail, sizeof(frametail));
   // append_CRC16_check_sum(cache,sizeof(framehead)+sizeof(datahead)+sizeof(imageData)+sizeof(frametail));
   HAL_UART_Transmit_DMA(UART6_data.huart, cache, sizeof(framehead) + sizeof(datahead) + sizeof(imageData) + sizeof(frametail));
   UI_Seq++;
   return 0;
}
/*****************************************************CRC8校验值计算**********************************************/
const uint8_t CRC8_INIT_UI = 0xff;
const uint8_t CRC8_TAB_UI[256] =
    {
        0x00,
        0x5e,
        0xbc,
        0xe2,
        0x61,
        0x3f,
        0xdd,
        0x83,
        0xc2,
        0x9c,
        0x7e,
        0x20,
        0xa3,
        0xfd,
        0x1f,
        0x41,
        0x9d,
        0xc3,
        0x21,
        0x7f,
        0xfc,
        0xa2,
        0x40,
        0x1e,
        0x5f,
        0x01,
        0xe3,
        0xbd,
        0x3e,
        0x60,
        0x82,
        0xdc,
        0x23,
        0x7d,
        0x9f,
        0xc1,
        0x42,
        0x1c,
        0xfe,
        0xa0,
        0xe1,
        0xbf,
        0x5d,
        0x03,
        0x80,
        0xde,
        0x3c,
        0x62,
        0xbe,
        0xe0,
        0x02,
        0x5c,
        0xdf,
        0x81,
        0x63,
        0x3d,
        0x7c,
        0x22,
        0xc0,
        0x9e,
        0x1d,
        0x43,
        0xa1,
        0xff,
        0x46,
        0x18,
        0xfa,
        0xa4,
        0x27,
        0x79,
        0x9b,
        0xc5,
        0x84,
        0xda,
        0x38,
        0x66,
        0xe5,
        0xbb,
        0x59,
        0x07,
        0xdb,
        0x85,
        0x67,
        0x39,
        0xba,
        0xe4,
        0x06,
        0x58,
        0x19,
        0x47,
        0xa5,
        0xfb,
        0x78,
        0x26,
        0xc4,
        0x9a,
        0x65,
        0x3b,
        0xd9,
        0x87,
        0x04,
        0x5a,
        0xb8,
        0xe6,
        0xa7,
        0xf9,
        0x1b,
        0x45,
        0xc6,
        0x98,
        0x7a,
        0x24,
        0xf8,
        0xa6,
        0x44,
        0x1a,
        0x99,
        0xc7,
        0x25,
        0x7b,
        0x3a,
        0x64,
        0x86,
        0xd8,
        0x5b,
        0x05,
        0xe7,
        0xb9,
        0x8c,
        0xd2,
        0x30,
        0x6e,
        0xed,
        0xb3,
        0x51,
        0x0f,
        0x4e,
        0x10,
        0xf2,
        0xac,
        0x2f,
        0x71,
        0x93,
        0xcd,
        0x11,
        0x4f,
        0xad,
        0xf3,
        0x70,
        0x2e,
        0xcc,
        0x92,
        0xd3,
        0x8d,
        0x6f,
        0x31,
        0xb2,
        0xec,
        0x0e,
        0x50,
        0xaf,
        0xf1,
        0x13,
        0x4d,
        0xce,
        0x90,
        0x72,
        0x2c,
        0x6d,
        0x33,
        0xd1,
        0x8f,
        0x0c,
        0x52,
        0xb0,
        0xee,
        0x32,
        0x6c,
        0x8e,
        0xd0,
        0x53,
        0x0d,
        0xef,
        0xb1,
        0xf0,
        0xae,
        0x4c,
        0x12,
        0x91,
        0xcf,
        0x2d,
        0x73,
        0xca,
        0x94,
        0x76,
        0x28,
        0xab,
        0xf5,
        0x17,
        0x49,
        0x08,
        0x56,
        0xb4,
        0xea,
        0x69,
        0x37,
        0xd5,
        0x8b,
        0x57,
        0x09,
        0xeb,
        0xb5,
        0x36,
        0x68,
        0x8a,
        0xd4,
        0x95,
        0xcb,
        0x29,
        0x77,
        0xf4,
        0xaa,
        0x48,
        0x16,
        0xe9,
        0xb7,
        0x55,
        0x0b,
        0x88,
        0xd6,
        0x34,
        0x6a,
        0x2b,
        0x75,
        0x97,
        0xc9,
        0x4a,
        0x14,
        0xf6,
        0xa8,
        0x74,
        0x2a,
        0xc8,
        0x96,
        0x15,
        0x4b,
        0xa9,
        0xf7,
        0xb6,
        0xe8,
        0x0a,
        0x54,
        0xd7,
        0x89,
        0x6b,
        0x35,
};
uint8_t Get_CRC8_Check_Sum_UI(uint8_t *pchMessage, unsigned int dwLength, uint8_t ucCRC8)
{
   uint8_t ucIndex;
   while (dwLength--)
   {
      ucIndex = ucCRC8 ^ (*pchMessage++);
      ucCRC8 = CRC8_TAB_UI[ucIndex];
   }
   return (ucCRC8);
}
uint16_t CRC_INIT_UI = 0xffff;
const uint16_t wCRC_Table_UI[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};
/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint16_t Get_CRC16_Check_Sum_UI(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC)
{
   Uint8_t chData;
   if (pchMessage == NULL)
   {
      return 0xFFFF;
   }
   while (dwLength--)
   {
      chData = *pchMessage++;
      (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table_UI[((uint16_t)(wCRC) ^ (uint16_t)(chData)) &
                                                       0x00ff];
   }
   return wCRC;
}
