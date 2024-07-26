//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_4.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 10
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_4;
ui_interface_number_t *ui_default_Ungroup_shootup_speed = (ui_interface_number_t *)&(ui_default_Ungroup_4.data[0]);
ui_interface_number_t *ui_default_Ungroup_shootleft_speed = (ui_interface_number_t *)&(ui_default_Ungroup_4.data[1]);

void _ui_init_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_4.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_4.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_4.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_shootup_speed->figure_tpye = 6;
    ui_default_Ungroup_shootup_speed->layer = 0;
    ui_default_Ungroup_shootup_speed->font_size = 30;
    ui_default_Ungroup_shootup_speed->start_x = 286;
    ui_default_Ungroup_shootup_speed->start_y = 809;
    ui_default_Ungroup_shootup_speed->color = 0;
    ui_default_Ungroup_shootup_speed->number = 0;
    ui_default_Ungroup_shootup_speed->width = 3;

    ui_default_Ungroup_shootleft_speed->figure_tpye = 6;
    ui_default_Ungroup_shootleft_speed->layer = 2;
    ui_default_Ungroup_shootleft_speed->font_size = 30;
    ui_default_Ungroup_shootleft_speed->start_x = 286;
    ui_default_Ungroup_shootleft_speed->start_y = 758;
    ui_default_Ungroup_shootleft_speed->color = 0;
    ui_default_Ungroup_shootleft_speed->number = 0;
    ui_default_Ungroup_shootleft_speed->width = 3;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_update_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_remove_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}
