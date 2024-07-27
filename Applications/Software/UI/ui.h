#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"
#include "ui_default_chassis_0.h"
#include "ui_default_energy_0.h"
#include "ui_default_state_0.h"
#include "ui_default_state_1.h"
#include "ui_default_state_2.h"
#include "ui_default_aim_0.h"
#include "ui_default_gimbal_0.h"
#include "ui_default_shoot_0.h"

// 初始化宏
#define ui_init_default_chassis() _ui_init_default_chassis_0()
#define ui_update_default_chassis() _ui_update_default_chassis_0()
#define ui_remove_default_chassis() _ui_remove_default_chassis_0()

#define ui_init_default_energy() _ui_init_default_energy_0()
#define ui_update_default_energy() _ui_update_default_energy_0()
#define ui_remove_default_energy() _ui_remove_default_energy_0()

#define ui_init_default_state() \
    _ui_init_default_state_0(); \
    _ui_init_default_state_1(); \
    _ui_init_default_state_2()

#define ui_update_default_state() \
    _ui_update_default_state_0(); \
    _ui_update_default_state_1(); \
    _ui_update_default_state_2()

#define ui_remove_default_state() \
    _ui_remove_default_state_0(); \
    _ui_remove_default_state_1(); \
    _ui_remove_default_state_2()

#define ui_init_default_aim() _ui_init_default_aim_0()
#define ui_update_default_aim() _ui_update_default_aim_0()
#define ui_remove_default_aim() _ui_remove_default_aim_0()

#define ui_init_default_gimbal() _ui_init_default_gimbal_0()
#define ui_update_default_gimbal() _ui_update_default_gimbal_0()
#define ui_remove_default_gimbal() _ui_remove_default_gimbal_0()

#define ui_init_default_shoot() _ui_init_default_shoot_0()
#define ui_update_default_shoot() _ui_update_default_shoot_0()
#define ui_remove_default_shoot() _ui_remove_default_shoot_0()

// 函数声明
void ui_init();
void ui_update();
void ui_supercap(float voltage);
void char_change();
void ui_chassis(float angle);
void ui_chassisline();
void ui_pitch_angle(float angle);

#ifdef __cplusplus
}
#endif

#endif // UI_H

