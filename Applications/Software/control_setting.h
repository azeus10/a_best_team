#ifndef _CONTROL_SETTING_H
#define _CONTROL_SETTING_H

#include "stdint.h"

void remove_control_task(void);

extern float PitchCofficientFromRC;
extern float YawCofficientFromRC;

extern float PitchCofficientFromNUC;
extern float YawCofficientFromNUC;
extern float  auto_yaw,auto_pitch;
extern float PitchCofficientFromPC;
extern float YawCofficientFromPC;
extern uint8_t one_key_back;
extern float  auto_yaw,auto_pitch;
#endif /* CONTROL_SETTING_H */
