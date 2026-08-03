#include "UIobjects.h"

lv_obj_t *objcreate_button(lv_obj_t *parent, lv_style_t &style_button, lv_style_t &style_button_pressed, lv_coord_t w, lv_coord_t h) {
  lv_obj_t *btn = lv_btn_create(parent);

  lv_obj_set_align(btn, LV_ALIGN_LEFT_MID);
  lv_obj_set_size(btn, lv_pct(w), lv_pct(h));
  lv_obj_add_style(btn, &style_button, 0);
  lv_obj_add_style(btn, &style_button_pressed, LV_STATE_PRESSED);

  return btn;
}

lv_obj_t *flexbox (lv_obj_t* parent, lv_align_t alignment, lv_flex_flow_t flow, lv_coord_t w, lv_coord_t h, lv_coord_t x_align, lv_coord_t y_align) {
  // Create a container on the active screen
  lv_obj_t *box = lv_obj_create(parent);

  // Size and align the container
  lv_obj_set_size(box, LV_PCT(w), LV_PCT(h));
  lv_obj_align(box, alignment, lv_pct(x_align), lv_pct(y_align));
  lv_obj_set_flex_flow(box, flow);
  lv_obj_set_flex_align(box, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  return box;
} 

static void btn_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  
  if(code == LV_EVENT_CLICKED) {
    lv_obj_t *slider = (lv_obj_t *)lv_event_get_user_data(e);
    lv_obj_clear_flag(slider, LV_OBJ_FLAG_HIDDEN); // Show slider
  }
}

lv_obj_t *make_slider(lv_obj_t *button, lv_obj_t *container) {
  lv_obj_t *slider = lv_slider_create(container);

  lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN);  
  lv_obj_add_event_cb(button, btn_event_cb, LV_EVENT_CLICKED, slider);

  lv_obj_set_width(slider, lv_pct(25));
  lv_obj_align_to(slider, button, LV_ALIGN_RIGHT_MID, lv_pct(40), 0);

  /* Hide initially */
  lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN);
  
  return slider;
}

lv_obj_t* create_title(lv_obj_t* parent, const char* text) {
  lv_obj_t* label = lv_label_create(parent);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, text);

  return label;
}