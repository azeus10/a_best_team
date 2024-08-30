#include "nlink_tofsense_frame0.h"

/*TOFSense/TOFSense-P/PS/F/FP data struct*/
typedef struct
{
	uint8_t ID;

	float dis;		// distance
	uint8_t status; // is accuracy£¿
	uint16_t signal_strength;
} Tofsense;
typedef struct
{
	int  average_value;
  float now_distance_processed;
} Tofsense_process_data;


extern Tofsense Nloop_TOF[8];
extern Tofsense_process_data  process_Nloop_TOF;

void Nloop_Tof_Unpack_Data(uint8_t *data, uint8_t len);
