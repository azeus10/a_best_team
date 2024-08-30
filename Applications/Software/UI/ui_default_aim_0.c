//
// Created by RM UI Designer
//

#include "ui_default_aim_0.h"

#define FRAME_ID 0
#define GROUP_ID 3
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_aim_0;
ui_interface_line_t *ui_default_aim_DropLine = (ui_interface_line_t *)&(ui_default_aim_0.data[0]);
ui_interface_rect_t *ui_default_aim_aim_12 = (ui_interface_rect_t *)&(ui_default_aim_0.data[1]);

void _ui_init_default_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_aim_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_aim_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_aim_0.data[i].figure_name[2] = i + START_ID;
        ui_default_aim_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_aim_0.data[i].operate_tpyel = 0;
    }

    ui_default_aim_DropLine->figure_tpye = 0;
    ui_default_aim_DropLine->layer = 3;
    ui_default_aim_DropLine->start_x = 960;
    ui_default_aim_DropLine->start_y = 542;
    ui_default_aim_DropLine->end_x = 960;
    ui_default_aim_DropLine->end_y = 400;
    ui_default_aim_DropLine->color = 0;
    ui_default_aim_DropLine->width = 3;

    ui_default_aim_aim_12->figure_tpye = 1;
    ui_default_aim_aim_12->layer = 3;
    ui_default_aim_aim_12->start_x = 1054;
    ui_default_aim_aim_12->start_y = 701;
    ui_default_aim_aim_12->color = 0;
    ui_default_aim_aim_12->width = 4;
    ui_default_aim_aim_12->end_x = 1106;
    ui_default_aim_aim_12->end_y = 725;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_aim_0, sizeof(ui_default_aim_0));
}

void _ui_update_default_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_aim_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_aim_0, sizeof(ui_default_aim_0));
}

void _ui_remove_default_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_aim_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_aim_0, sizeof(ui_default_aim_0));
}
