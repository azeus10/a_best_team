#include "double.h"
#include "stdint.h"

//float send_message;
//float receive_message;


//void data_send()
//{
//	uint8_t can_send_data [8];
//	static CAN_TxHeaderTypeDef tx_message;
//	uint32_t send_mail_box;

//	tx_message.StdId = 0x303;
//	tx_message.IDE = CAN_ID_STD;
//	tx_message.RTR = CAN_RTR_DATA;
//	tx_message.DLC = 0x08;
//	
//	float_to_bytes(send_message, &can_send_data[0]);  // ��x���ֽڴ���can_data[0]~can_data[3]
//		
//	HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
//}
//// ��float���ֽڲ��
//void float_to_bytes(float f, uint8_t *bytes) 
//{
//    uint32_t *p = (uint32_t *)&f;  // ��floatָ��ǿ��ת��Ϊuint32_tָ��
//    uint32_t temp = *p;            // ��ȡfloat�Ķ����Ʊ�ʾ
//    bytes[0] = (temp >> 0) & 0xFF; // ��ȡ����ֽ�
//    bytes[1] = (temp >> 8) & 0xFF; // ��ȡ�ڶ����ֽ�
//    bytes[2] = (temp >> 16) & 0xFF; // ��ȡ�������ֽ�
//    bytes[3] = (temp >> 24) & 0xFF; // ��ȡ����ֽ�
// }
//  
////����
//void data_receive(uint8_t data[8])
//{
//		receive_message = bytes_to_float(&data[0]);  
//} 

//float bytes_to_float(uint8_t *bytes) 
//{
//    uint32_t temp = 0;
//    temp |= (bytes[0] << 0);  
//    temp |= (bytes[1] << 8);  
//    temp |= (bytes[2] << 16);
//    temp |= (bytes[3] << 24); 
//    return *(float *)&temp;   
//}

//if(rx_header.StdId == 0x303)
//{
//	data_receive(rx_data);
//	return;
//} 