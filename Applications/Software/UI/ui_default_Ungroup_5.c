//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_5.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 11
#define OBJ_NUM 5
#define FRAME_OBJ_NUM 5

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_5;
ui_interface_rect_t *ui_default_Ungroup_aim_12 = (ui_interface_rect_t *)&(ui_default_Ungroup_5.data[0]);
ui_interface_number_t *ui_default_Ungroup_pitch_angle = (ui_interface_number_t *)&(ui_default_Ungroup_5.data[1]);
ui_interface_number_t *ui_default_Ungroup_shootup_speed = (ui_interface_number_t *)&(ui_default_Ungroup_5.data[2]);
ui_interface_number_t *ui_default_Ungroup_shootright_speed = (ui_interface_number_t *)&(ui_default_Ungroup_5.data[3]);
ui_interface_number_t *ui_default_Ungroup_shootleft_speed = (ui_interface_number_t *)&(ui_default_Ungroup_5.data[4]);

void _ui_init_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_5.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_5.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_5.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_aim_12->figure_tpye = 1;
    ui_default_Ungroup_aim_12->layer = 3;
    ui_default_Ungroup_aim_12->start_x = 834;
    ui_default_Ungroup_aim_12->start_y = 624;
    ui_default_Ungroup_aim_12->color = 0;
    ui_default_Ungroup_aim_12->width = 4;
    ui_default_Ungroup_aim_12->end_x = 903;
    ui_default_Ungroup_aim_12->end_y = 651;

    ui_default_Ungroup_pitch_angle->figure_tpye = 5;
    ui_default_Ungroup_pitch_angle->layer = 4;
    ui_default_Ungroup_pitch_angle->font_size = 30;
    ui_default_Ungroup_pitch_angle->start_x = 290;
    ui_default_Ungroup_pitch_angle->start_y = 862;
    ui_default_Ungroup_pitch_angle->color = 2;
    ui_default_Ungroup_pitch_angle->number = 0;
    ui_default_Ungroup_pitch_angle->width = 3;

    ui_default_Ungroup_shootup_speed->figure_tpye = 6;
    ui_default_Ungroup_shootup_speed->layer = 4;
    ui_default_Ungroup_shootup_speed->font_size = 30;
    ui_default_Ungroup_shootup_speed->start_x = 286;
    ui_default_Ungroup_shootup_speed->start_y = 809;
    ui_default_Ungroup_shootup_speed->color = 0;
    ui_default_Ungroup_shootup_speed->number = 12345;
    ui_default_Ungroup_shootup_speed->width = 3;

    ui_default_Ungroup_shootright_speed->figure_tpye = 6;
    ui_default_Ungroup_shootright_speed->layer = 4;
    ui_default_Ungroup_shootright_speed->font_size = 30;
    ui_default_Ungroup_shootright_speed->start_x = 286;
    ui_default_Ungroup_shootright_speed->start_y = 711;
    ui_default_Ungroup_shootright_speed->color = 0;
    ui_default_Ungroup_shootright_speed->number = 12345;
    ui_default_Ungroup_shootright_speed->width = 3;

    ui_default_Ungroup_shootleft_speed->figure_tpye = 6;
    ui_default_Ungroup_shootleft_speed->layer = 4;
    ui_default_Ungroup_shootleft_speed->font_size = 30;
    ui_default_Ungroup_shootleft_speed->start_x = 286;
    ui_default_Ungroup_shootleft_speed->start_y = 758;
    ui_default_Ungroup_shootleft_speed->color = 0;
    ui_default_Ungroup_shootleft_speed->number = 12345;
    ui_default_Ungroup_shootleft_speed->width = 3;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_update_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_remove_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}
