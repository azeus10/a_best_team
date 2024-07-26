//
// Created by RM UI Designer
//

#include "ui_default_energy_0.h"

#define FRAME_ID 0
#define GROUP_ID 1
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_energy_0;
ui_interface_rect_t *ui_default_energy_energycircle = (ui_interface_rect_t *)&(ui_default_energy_0.data[0]);
ui_interface_rect_t *ui_default_energy_EnergyLine = (ui_interface_rect_t *)&(ui_default_energy_0.data[1]);

void _ui_init_default_energy_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_energy_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_energy_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_energy_0.data[i].figure_name[2] = i + START_ID;
        ui_default_energy_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_energy_0.data[i].operate_tpyel = 0;
    }

    ui_default_energy_energycircle->figure_tpye = 1;
    ui_default_energy_energycircle->layer = 1;
    ui_default_energy_energycircle->start_x = 528;
    ui_default_energy_energycircle->start_y = 114;
    ui_default_energy_energycircle->color = 6;
    ui_default_energy_energycircle->width = 1;
    ui_default_energy_energycircle->end_x = 1408;
    ui_default_energy_energycircle->end_y = 152;

    ui_default_energy_EnergyLine->figure_tpye = 1;
    ui_default_energy_EnergyLine->layer = 1;
    ui_default_energy_EnergyLine->start_x = 529;
    ui_default_energy_EnergyLine->start_y = 118;
    ui_default_energy_EnergyLine->color = 2;
    ui_default_energy_EnergyLine->width = 15;
    ui_default_energy_EnergyLine->end_x = 1391;
    ui_default_energy_EnergyLine->end_y = 133;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_energy_0);
    SEND_MESSAGE((uint8_t *) &ui_default_energy_0, sizeof(ui_default_energy_0));
}

void _ui_update_default_energy_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_energy_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_energy_0);
    SEND_MESSAGE((uint8_t *) &ui_default_energy_0, sizeof(ui_default_energy_0));
}

void _ui_remove_default_energy_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_energy_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_energy_0);
    SEND_MESSAGE((uint8_t *) &ui_default_energy_0, sizeof(ui_default_energy_0));
}
