//
// Created by RM UI Designer
//

#include "ui_default_close_aim_0.h"

#define FRAME_ID 0
#define GROUP_ID 6
#define START_ID 0
#define OBJ_NUM 1
#define FRAME_OBJ_NUM 1

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_close_aim_0;
ui_interface_rect_t *ui_default_close_aim_aim_10 = (ui_interface_rect_t *)&(ui_default_close_aim_0.data[0]);

void _ui_init_default_close_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_close_aim_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_close_aim_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_close_aim_0.data[i].figure_name[2] = i + START_ID;
        ui_default_close_aim_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_close_aim_0.data[i].operate_tpyel = 0;
    }

    ui_default_close_aim_aim_10->figure_tpye = 1;
    ui_default_close_aim_aim_10->layer = 5;
    ui_default_close_aim_aim_10->start_x = 932;
    ui_default_close_aim_aim_10->start_y = 439;
    ui_default_close_aim_aim_10->color = 6;
    ui_default_close_aim_aim_10->width = 2;
    ui_default_close_aim_aim_10->end_x = 952;
    ui_default_close_aim_aim_10->end_y = 448;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_close_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_close_aim_0, sizeof(ui_default_close_aim_0));
}

void _ui_update_default_close_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_close_aim_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_close_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_close_aim_0, sizeof(ui_default_close_aim_0));
}

void _ui_remove_default_close_aim_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_close_aim_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_close_aim_0);
    SEND_MESSAGE((uint8_t *) &ui_default_close_aim_0, sizeof(ui_default_close_aim_0));
}
