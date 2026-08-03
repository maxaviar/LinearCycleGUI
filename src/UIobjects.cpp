#include "UIobjects.h"

void objcreate_button(lv_style_t &style_button, lv_style_t &style_button_pressed,
lv_obj_t *button, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
  lv_obj_set_align(button, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(button, x);
  lv_obj_set_y(button, y);
  lv_obj_set_size(button, w, h);
  lv_obj_add_style(button, &style_button, 0);
  lv_obj_add_style(button, &style_button_pressed, LV_STATE_PRESSED);
}

lv_obj_t* create_title(lv_obj_t* parent, const char* text) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, text);

    return label;
}