/**
 * @file USB_VirCom.c
 * @author sethome
 * @brief 虚拟串口数据发送
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "usbd_cdc_if.h"
#include "USB_VirCom.h"
#include "NUC_communication.h"
#include "Global_status.h"


NUC_data_t fromNUC;

uint8_t StartCommunciate=0;

void VirCom_send(uint8_t data[], uint16_t len)
{
//	if(	StartCommunciate==1)
//	{
		 CDC_Transmit_FS(data, len);
//	}
}
void VirCom_rev(uint8_t data[], uint16_t len)
{

 if (data[0] == 0xaa)
 {
	decodeNUC(&fromNUC,data,len);
 }
}




