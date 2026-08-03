#pragma once
#include "common.h"

/* Creating objects */
void objcreate_button(
  lv_style_t &style_button,
  lv_style_t &style_button_pressed,
  lv_obj_t *button,
  lv_coord_t x,
  lv_coord_t y,
  lv_coord_t w,
  lv_coord_t h
);

lv_obj_t* create_title(lv_obj_t* parent, const char* text);
