//
// Created by RM UI Designer
//

#include "ui_default_gimbal_0.h"

#define FRAME_ID 0
#define GROUP_ID 4
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_gimbal_0;
ui_interface_number_t *ui_default_gimbal_pitch_angle = (ui_interface_number_t *)&(ui_default_gimbal_0.data[0]);
ui_interface_rect_t *ui_default_gimbal_aim_5 = (ui_interface_rect_t *)&(ui_default_gimbal_0.data[1]);

void _ui_init_default_gimbal_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_gimbal_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_gimbal_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_gimbal_0.data[i].figure_name[2] = i + START_ID;
        ui_default_gimbal_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_gimbal_0.data[i].operate_tpyel = 0;
    }

    ui_default_gimbal_pitch_angle->figure_tpye = 5;
    ui_default_gimbal_pitch_angle->layer = 4;
    ui_default_gimbal_pitch_angle->font_size = 30;
    ui_default_gimbal_pitch_angle->start_x = 288;
    ui_default_gimbal_pitch_angle->start_y = 862;
    ui_default_gimbal_pitch_angle->color = 2;
    ui_default_gimbal_pitch_angle->number = 0;
    ui_default_gimbal_pitch_angle->width = 3;

    ui_default_gimbal_aim_5->figure_tpye = 1;//г╟изу╬3m©Р
    ui_default_gimbal_aim_5->layer = 5;
    ui_default_gimbal_aim_5->start_x = 881;
    ui_default_gimbal_aim_5->start_y = 402;
    ui_default_gimbal_aim_5->color = 6;
    ui_default_gimbal_aim_5->width = 2;
    ui_default_gimbal_aim_5->end_x = 998;
    ui_default_gimbal_aim_5->end_y = 439;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_gimbal_0);
    SEND_MESSAGE((uint8_t *) &ui_default_gimbal_0, sizeof(ui_default_gimbal_0));
}

void _ui_update_default_gimbal_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_gimbal_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_gimbal_0);
    SEND_MESSAGE((uint8_t *) &ui_default_gimbal_0, sizeof(ui_default_gimbal_0));
}

void _ui_remove_default_gimbal_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_gimbal_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_gimbal_0);
    SEND_MESSAGE((uint8_t *) &ui_default_gimbal_0, sizeof(ui_default_gimbal_0));
}
