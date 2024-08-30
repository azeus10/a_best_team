//
// Created by RM UI Designer
//

#include "ui_default_chassis_0.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_chassis_0;
ui_interface_arc_t *ui_default_chassis_Chassis = (ui_interface_arc_t *)&(ui_default_chassis_0.data[0]);
ui_interface_rect_t *ui_default_chassis_aim_18 = (ui_interface_rect_t *)&(ui_default_chassis_0.data[1]);

void _ui_init_default_chassis_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_chassis_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_chassis_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_chassis_0.data[i].figure_name[2] = i + START_ID;
        ui_default_chassis_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_chassis_0.data[i].operate_tpyel = 0;
    }

    ui_default_chassis_Chassis->figure_tpye = 4;
    ui_default_chassis_Chassis->layer = 0;
    ui_default_chassis_Chassis->rx = 360;
    ui_default_chassis_Chassis->ry = 360;
    ui_default_chassis_Chassis->start_x = 960;
    ui_default_chassis_Chassis->start_y = 540;
    ui_default_chassis_Chassis->color = 1;
    ui_default_chassis_Chassis->width = 8;
    ui_default_chassis_Chassis->start_angle = 160;
    ui_default_chassis_Chassis->end_angle = 200;

    ui_default_chassis_aim_18->figure_tpye = 1;//12Ã×µõÉäÇ°ÉÚÕ¾
    ui_default_chassis_aim_18->layer = 3;
    ui_default_chassis_aim_18->start_x = 981;
    ui_default_chassis_aim_18->start_y = 573;
    ui_default_chassis_aim_18->color = 5;
    ui_default_chassis_aim_18->width = 4;
    ui_default_chassis_aim_18->end_x = 1047;
    ui_default_chassis_aim_18->end_y = 608;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_chassis_0);
    SEND_MESSAGE((uint8_t *) &ui_default_chassis_0, sizeof(ui_default_chassis_0));
}

void _ui_update_default_chassis_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_chassis_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_chassis_0);
    SEND_MESSAGE((uint8_t *) &ui_default_chassis_0, sizeof(ui_default_chassis_0));
}

void _ui_remove_default_chassis_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_chassis_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_chassis_0);
    SEND_MESSAGE((uint8_t *) &ui_default_chassis_0, sizeof(ui_default_chassis_0));
}
