#pragma once
#include "common.h"

extern lv_obj_t *time_label;
extern lv_obj_t *dwell_label;
extern lv_obj_t *distance_label;

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

static void limit_event_cb(lv_event_t * e);
static void btn_event_cb(lv_event_t * e);
static void time_event_cb(lv_event_t * e);
static void dwell_event_cb(lv_event_t * e);
static void distance_event_cb(lv_event_t * e);
static void startstop_event_cb(lv_event_t * e);
static void textarea_event_cb (lv_event_t *e);
static void textarea_focus_cb (lv_event_t *e);
static void btnmatrix_event_cb (lv_event_t *e);
static void update_count(lv_timer_t *timer);

lv_obj_t *create_textarea(lv_obj_t *parent);
void startstopswitch (lv_obj_t *start, lv_obj_t *stop);
lv_obj_t *create_limit(lv_obj_t *button);
lv_obj_t *create_panel(lv_obj_t *button, lv_obj_t* parent);
lv_obj_t *make_slider(lv_obj_t *button, lv_obj_t *container, int32_t min, int32_t max, int val);
lv_obj_t *create_title(lv_obj_t *parent, const char *text);
lv_obj_t *create_title_special(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color);