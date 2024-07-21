
#include "referee_handle_pack.h"
 __packed typedef struct 
 {
// 	uint8_t header;
//	 
// 	float Q0,Q1,Q2,Q3;
//	 
// 	float yaw,pitch;	 
//	 
// 	float x_speed,y_speed,r_speed;
//	 
// 	uint8_t bullet;

// 	uint8_t distance;
//	 
//	float robot_speed_mps;

//	uint8_t mode;
//	
//  uint8_t	enemy_color;

//	uint8_t enemy_blood_[6];

// 	char unknow[11];
//	
//	uint16_t checksum;
	char header;
	float yaw,pitch;

	float robot_speed_mps;
	float b_speed;
	int mode;
	uint8_t enemy;
	char unknown[9];
	char tails;
 } STM32_data_t;


typedef struct
{
//uint8_t header;
	char header;
	float yaw;
	float pitch;
  uint8_t shoot;//发弹标志位
	//uint16_t checksum;
	char unknown[21];
	char tails;
} __attribute__((packed)) NUC_data_t;



extern STM32_data_t toNUC;
extern NUC_data_t fromNUC;

//对STM32向NUC的信息解/编码
int encodeSTM32(STM32_data_t *target, unsigned char rx_buff[], unsigned int len);
int decodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len);

//对NUC向STM32的信息解/编码
int decodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len);
int encodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len);




//end of file
