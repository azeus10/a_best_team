#include "nloop_middlewares.h"

Tofsense Nloop_TOF[8];
Tofsense_process_data process_Nloop_TOF;
static uint32_t cnt;

void Nloop_Tof_Unpack_Data(uint8_t *data, uint8_t len)
{
	uint8_t tof_flag = 0, tof_id = 0;
	tof_flag = g_nts_frame0.UnpackData(data, len); // unpack data
	tof_id = g_nts_frame0.result.id;			   // get laser ID
	if (tof_flag == 1)							   // ID 1
	{
		Nloop_TOF[tof_id].ID = tof_id;
		Nloop_TOF[tof_id].dis = g_nts_frame0.result.dis;//M
		Nloop_TOF[tof_id].status = g_nts_frame0.result.dis_status;
		Nloop_TOF[tof_id].signal_strength = g_nts_frame0.result.signal_strength;
		
		//筛选数据
		if(Nloop_TOF[tof_id].signal_strength>3000)
		{	
			process_Nloop_TOF.average_value+=Nloop_TOF[tof_id].dis;
			cnt++;
		}
	}
	//取10次可靠数据取平均
	if(cnt==10)
	{
		process_Nloop_TOF.now_distance_processed=process_Nloop_TOF.average_value/10.0f;
		cnt=0;
	}
}