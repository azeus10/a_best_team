//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_3.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 3
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_3;
ui_interface_arc_t *ui_default_Ungroup_Chassis = (ui_interface_arc_t *)&(ui_default_Ungroup_3.data[0]);
ui_interface_rect_t *ui_default_Ungroup_energycircle = (ui_interface_rect_t *)&(ui_default_Ungroup_3.data[1]);
ui_interface_rect_t *ui_default_Ungroup_EnergyLine = (ui_interface_rect_t *)&(ui_default_Ungroup_3.data[2]);
ui_interface_rect_t *ui_default_Ungroup_aim_18 = (ui_interface_rect_t *)&(ui_default_Ungroup_3.data[3]);
ui_interface_line_t *ui_default_Ungroup_DropLine = (ui_interface_line_t *)&(ui_default_Ungroup_3.data[4]);
ui_interface_rect_t *ui_default_Ungroup_aim_12 = (ui_interface_rect_t *)&(ui_default_Ungroup_3.data[5]);
ui_interface_number_t *ui_default_Ungroup_pitch_angle = (ui_interface_number_t *)&(ui_default_Ungroup_3.data[6]);

void _ui_init_default_Ungroup_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_3.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_3.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_3.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_3.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_3.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_Chassis->figure_tpye = 4;
    ui_default_Ungroup_Chassis->layer = 0;
    ui_default_Ungroup_Chassis->rx = 360;
    ui_default_Ungroup_Chassis->ry = 360;
    ui_default_Ungroup_Chassis->start_x = 960;
    ui_default_Ungroup_Chassis->start_y = 540;
    ui_default_Ungroup_Chassis->color = 1;
    ui_default_Ungroup_Chassis->width = 8;
    ui_default_Ungroup_Chassis->start_angle = 160;
    ui_default_Ungroup_Chassis->end_angle = 200;

    ui_default_Ungroup_energycircle->figure_tpye = 1;
    ui_default_Ungroup_energycircle->layer = 1;
    ui_default_Ungroup_energycircle->start_x = 528;
    ui_default_Ungroup_energycircle->start_y = 114;
    ui_default_Ungroup_energycircle->color = 6;
    ui_default_Ungroup_energycircle->width = 1;
    ui_default_Ungroup_energycircle->end_x = 1408;
    ui_default_Ungroup_energycircle->end_y = 152;

    ui_default_Ungroup_EnergyLine->figure_tpye = 1;
    ui_default_Ungroup_EnergyLine->layer = 1;
    ui_default_Ungroup_EnergyLine->start_x = 529;
    ui_default_Ungroup_EnergyLine->start_y = 118;
    ui_default_Ungroup_EnergyLine->color = 2;
    ui_default_Ungroup_EnergyLine->width = 15;
    ui_default_Ungroup_EnergyLine->end_x = 1391;
    ui_default_Ungroup_EnergyLine->end_y = 133;

    ui_default_Ungroup_aim_18->figure_tpye = 1;
    ui_default_Ungroup_aim_18->layer = 3;
    ui_default_Ungroup_aim_18->start_x = 875;
    ui_default_Ungroup_aim_18->start_y = 721;
    ui_default_Ungroup_aim_18->color = 5;
    ui_default_Ungroup_aim_18->width = 4;
    ui_default_Ungroup_aim_18->end_x = 932;
    ui_default_Ungroup_aim_18->end_y = 763;

    ui_default_Ungroup_DropLine->figure_tpye = 0;
    ui_default_Ungroup_DropLine->layer = 3;
    ui_default_Ungroup_DropLine->start_x = 960;
    ui_default_Ungroup_DropLine->start_y = 542;
    ui_default_Ungroup_DropLine->end_x = 960;
    ui_default_Ungroup_DropLine->end_y = 400;
    ui_default_Ungroup_DropLine->color = 0;
    ui_default_Ungroup_DropLine->width = 3;

    ui_default_Ungroup_aim_12->figure_tpye = 1;
    ui_default_Ungroup_aim_12->layer = 3;
    ui_default_Ungroup_aim_12->start_x = 879;
    ui_default_Ungroup_aim_12->start_y = 635;
    ui_default_Ungroup_aim_12->color = 0;
    ui_default_Ungroup_aim_12->width = 4;
    ui_default_Ungroup_aim_12->end_x = 948;
    ui_default_Ungroup_aim_12->end_y = 662;

    ui_default_Ungroup_pitch_angle->figure_tpye = 5;
    ui_default_Ungroup_pitch_angle->layer = 4;
    ui_default_Ungroup_pitch_angle->font_size = 30;
    ui_default_Ungroup_pitch_angle->start_x = 288;
    ui_default_Ungroup_pitch_angle->start_y = 862;
    ui_default_Ungroup_pitch_angle->color = 2;
    ui_default_Ungroup_pitch_angle->number = 0;
    ui_default_Ungroup_pitch_angle->width = 3;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_3, sizeof(ui_default_Ungroup_3));
}

void _ui_update_default_Ungroup_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_3.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_3, sizeof(ui_default_Ungroup_3));
}

void _ui_remove_default_Ungroup_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_3.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_3, sizeof(ui_default_Ungroup_3));
}
