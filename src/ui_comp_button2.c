// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: esp32-2.4

#include "ui.h"


// COMPONENT Button2

lv_obj_t * ui_Button2_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Button2;
    cui_Button2 = lv_btn_create(comp_parent);
    lv_obj_set_width(cui_Button2, 100);
    lv_obj_set_height(cui_Button2, 50);
    lv_obj_set_x(cui_Button2, 4);
    lv_obj_set_y(cui_Button2, 32);
    lv_obj_set_align(cui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(cui_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_BUTTON2_NUM);
    children[UI_COMP_BUTTON2_BUTTON2] = cui_Button2;
    lv_obj_add_event_cb(cui_Button2, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Button2, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_Button2_create_hook(cui_Button2);
    return cui_Button2;
}

