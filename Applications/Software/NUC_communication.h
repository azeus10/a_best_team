
#include "referee_handle_pack.h"
 __packed typedef struct 
 {
	float yaw,pitch;
	float b_speed;
	int mode;
	uint8_t enemy;
	char unknown[15];
 } STM32_data_t;


typedef struct
{
//uint8_t header;
	//char header;
	float yaw;
	float pitch;
    uint8_t shoot;//发弹标志位
	uint16_t checksum;
	char unknown[21];
	
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
