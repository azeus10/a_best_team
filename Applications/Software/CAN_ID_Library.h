#ifndef __CAN_ID_LIBRARY
#define __CAN_ID_LIBRARY
/*******************如果使用了达秒电机的话，要注意以下几项********************/
/*
1.注意！！	如果在使用了达秒电机的时候， ！！!!!!!!!!!!!!!
			配置达妙电机的ID时要避免使用can1_1,!!!!!!!!!!!
			因为会与大疆电机的can通讯接收报文ID重复 !!!!!!		
			
2.因为达妙电机的接收ID和发送ID都要通过串口自己改，我们规定设置在0x01至0x39之间，并且设置成相同的

3.使用达妙电机时需要在达秒电机的驱动文件内初始化
*/


/*
canID手册，所有电机使用时都要在这定义
没啥实际意义，主要是为了方便避免ID重复
*/
#define chassis_FR 		CAN_1_1//底盘右前轮
#define chassis_FL 		CAN_1_2//左前轮
#define chassis_BL 		CAN_1_3//左后轮
#define chassis_BR 		CAN_1_4//右后轮
#define YAW_MOTOR 		CAN_1_5//云台yaw轴电机
//  CAN_1_6
#define TRIGGER_MOTOR   CAN_1_7//拨弹电机
//  CAN_1_8
//  CAN_1_6020_5
//  CAN_1_6020_6
//  CAN_1_6020_7

#define SHOOT_MOTOR1	CAN_2_1//从枪管正面看 上
#define SHOOT_MOTOR2 	CAN_2_2//从枪管正面看 左
#define SHOOT_MOTOR3 	CAN_2_3//从枪管正面看 右

#define SMALL_PITCH 	CAN_2_4//云台小pitch轴电机
//  CAN_2_5
#define PITCH_MOTOR 	CAN_2_6//云台pitch轴电机
#define SCOPE_MOTOR 	CAN_2_7//云台倍镜电机
//  CAN_2_8
//  CAN_2_6020_5
//  CAN_2_6020_6
//  CAN_2_6020_7



#endif

