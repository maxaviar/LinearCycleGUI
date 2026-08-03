#pragma once
#include "common.h"

/* Creating objects */
lv_obj_t *objcreate_button (
  lv_obj_t *parent,
  lv_style_t &style_button,
  lv_style_t &style_button_pressed,
  lv_coord_t w,
  lv_coord_t h
);

lv_obj_t *flexbox (
  lv_obj_t *parent, 
  lv_align_t alignment, 
  lv_flex_flow_t flow, 
  lv_coord_t w, 
  lv_coord_t h,
  lv_coord_t x_align, 
  lv_coord_t y_align
); 

static void btn_event_cb(lv_event_t * e);
lv_obj_t *create_panel(lv_obj_t *button, lv_obj_t* parent);
lv_obj_t *make_slider(lv_obj_t *button, lv_obj_t *container);
lv_obj_t *create_title(lv_obj_t *parent, const char *text);