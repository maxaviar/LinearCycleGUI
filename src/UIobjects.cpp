#include "UIobjects.h"

Settings settings;
bool limit_en;

lv_obj_t *speed_label;
lv_obj_t *dwell_label;
lv_obj_t *angle_label;

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
  lv_obj_set_style_bg_color(box, lv_color_hex(0x333333), 0);
  lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
  lv_obj_set_size(box, LV_PCT(w), LV_PCT(h));
  lv_obj_align(box, alignment, lv_pct(x_align), lv_pct(y_align));
  lv_obj_set_flex_flow(box, flow);
  lv_obj_set_flex_align(box, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  return box;
} 

static void limit_event_cb(lv_event_t *e) {
  lv_obj_t *label = (lv_obj_t *) lv_event_get_user_data(e);

  if (!limit_en) lv_label_set_text(label, "Limit: ON");
  else lv_label_set_text(label, "Limit: OFF");
  
  limit_en = !limit_en;
}

static void btn_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_user_data(e);
  bool hidden = lv_obj_has_flag(slider, LV_OBJ_FLAG_HIDDEN);

  if(code == LV_EVENT_CLICKED) {
    if (hidden) lv_obj_clear_flag(slider, LV_OBJ_FLAG_HIDDEN); // Show slider
    else lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN); // Hide slider
  }
}

static void speed_event_cb(lv_event_t *e) {
  if (lv_event_get_user_data(e) != NULL) { //shouldn't ever be null, but dereferencing a null pointer is never fun so #justincase
    int32_t *var = (int32_t*)lv_event_get_user_data(e);
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t slide_val = lv_slider_get_value(slider);
    *var = slide_val*10;

    lv_label_set_text_fmt(speed_label, "%d step/sec", *var);
  }
}

static void dwell_event_cb(lv_event_t *e) {
  if (lv_event_get_user_data(e) != NULL) { //shouldn't ever be null, but dereferencing a null pointer is never fun so #justincase
    int32_t *var = (int32_t*)lv_event_get_user_data(e);
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t slide_val = lv_slider_get_value(slider);
    *var = slide_val*500;

    lv_label_set_text_fmt(dwell_label, "%d ms", *var);
  }
}

static void angle_event_cb(lv_event_t *e) {
  if (lv_event_get_user_data(e) != NULL) { //shouldn't ever be null, but dereferencing a null pointer is never fun so #justincase
    int32_t *var = (int32_t*)lv_event_get_user_data(e);
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t slide_val = lv_slider_get_value(slider);
    *var = slide_val*10;

    lv_label_set_text_fmt(angle_label, "%d degrees", *var);
  }
}

static void startstop_event_cb(lv_event_t *e) {
  //lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *toshow = (lv_obj_t *)lv_event_get_user_data(e);
  lv_obj_t *tohide = (lv_obj_t *)lv_event_get_target(e);

  lv_obj_clear_flag(toshow, LV_OBJ_FLAG_HIDDEN); // Show button
  lv_obj_add_flag(tohide, LV_OBJ_FLAG_HIDDEN); // Hide button
}

void startstopswitch (lv_obj_t *start, lv_obj_t *stop) {
  lv_obj_add_flag(stop, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(start, startstop_event_cb, LV_EVENT_CLICKED, stop);
  lv_obj_add_event_cb(stop, startstop_event_cb, LV_EVENT_CLICKED, start);
}

static void textarea_event_cb (lv_event_t *e) {
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *btnmatrix = (lv_obj_t *)lv_event_get_user_data(e);

  char *endptr;
  settings.limit = (int)strtol(lv_textarea_get_text(ta), &endptr, 10);

  lv_obj_add_flag(btnmatrix, LV_OBJ_FLAG_HIDDEN);
}

static void textarea_focus_cb (lv_event_t *e) {
  lv_obj_t *btnmatrix = (lv_obj_t *)lv_event_get_user_data(e);
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_FOCUSED) lv_obj_clear_flag(btnmatrix, LV_OBJ_FLAG_HIDDEN);
}

static void btnmatrix_event_cb (lv_event_t *e) {
  lv_obj_t *btnmatrix = lv_event_get_target(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_user_data(e);
  const char *txt = lv_btnmatrix_get_btn_text(btnmatrix, lv_btnmatrix_get_selected_btn(btnmatrix));

  if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
  else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) lv_event_send(ta, LV_EVENT_READY, NULL);
  else lv_textarea_add_text(ta, txt);
}

lv_obj_t *create_textarea(lv_obj_t *parent) {
  lv_obj_t *ta = lv_textarea_create(parent);
  lv_textarea_set_one_line(ta, true);
  lv_obj_set_align(ta, LV_ALIGN_BOTTOM_MID);
  lv_obj_add_state(ta, LV_STATE_FOCUSED); // To be sure cursor is visible, whatever that means

  static const char *btnmatrix_map[] = {
    "1", "2", "3", "\n",
    "4", "5", "6", "\n",
    "7", "8", "9", "\n",
    LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
  };

  lv_obj_t *btnmatrix = lv_btnmatrix_create(lv_scr_act());
  lv_obj_add_flag(btnmatrix, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_size(btnmatrix, lv_pct(40), lv_pct(30));
  lv_obj_align_to(btnmatrix, parent, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_obj_add_event_cb(btnmatrix, btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, ta);
  lv_obj_add_event_cb(ta, textarea_focus_cb, LV_EVENT_FOCUSED, btnmatrix);
  lv_obj_add_event_cb(ta, textarea_event_cb, LV_EVENT_READY, btnmatrix);
  lv_obj_clear_flag(btnmatrix, LV_OBJ_FLAG_CLICK_FOCUSABLE);
  lv_btnmatrix_set_map(btnmatrix, btnmatrix_map);

  return ta;
}

lv_obj_t *create_limit(lv_obj_t *button) {
  lv_obj_t *label = lv_label_create(button);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, "Limit: OFF");
  limit_en = false;

  lv_obj_add_event_cb(button, limit_event_cb, LV_EVENT_CLICKED, label);

  return label;
}

lv_obj_t *create_panel(lv_obj_t *button, lv_obj_t* parent) {
  lv_obj_t *slider_panel = lv_obj_create(parent);

  lv_obj_add_flag(slider_panel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(button, btn_event_cb, LV_EVENT_CLICKED, slider_panel);

  lv_obj_set_size(slider_panel, lv_pct(30), lv_pct(18));
  lv_obj_align_to(slider_panel, button, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_set_flex_flow(slider_panel, LV_FLEX_FLOW_COLUMN_REVERSE);
  lv_obj_set_flex_align(slider_panel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_set_style_bg_color(slider_panel, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(slider_panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(slider_panel, 2, LV_PART_MAIN);
  lv_obj_set_style_border_color(slider_panel, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
  lv_obj_set_style_radius(slider_panel, 10, LV_PART_MAIN);

  return slider_panel;
}

lv_obj_t *make_slider(lv_obj_t *button, lv_obj_t *container, int32_t min, int32_t max, int val) {
  lv_obj_t *slider = lv_slider_create(container);

  lv_obj_add_flag(slider, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(button, btn_event_cb, LV_EVENT_CLICKED, slider);
  lv_obj_set_align(slider, LV_ALIGN_CENTER);
  lv_obj_set_width(slider, lv_pct(90));

  switch (val) {
    case SPEED:
      min = min/10;
      max = max/10;
      lv_slider_set_range(slider, min, max);
      speed_label = lv_label_create(container);
      lv_label_set_text_fmt(speed_label, "%d step/sec", settings.speed);
      
      lv_obj_add_event_cb(slider, speed_event_cb, LV_EVENT_VALUE_CHANGED, &settings.speed);
      break;

    case DWELL:
      max = max/500;
      lv_slider_set_range(slider, min, max);
      dwell_label = lv_label_create(container);
      lv_label_set_text_fmt(dwell_label, "%d ms", settings.dwell);
      
      lv_obj_add_event_cb(slider, dwell_event_cb, LV_EVENT_VALUE_CHANGED, &settings.dwell);
      break;

    case ANGLE:
      min = min/10;
      max = max/10;
      lv_slider_set_range(slider, min, max);
      angle_label = lv_label_create(container);
      lv_label_set_text_fmt(angle_label, "%d degrees", settings.angle);

      lv_obj_add_event_cb(slider, angle_event_cb, LV_EVENT_VALUE_CHANGED, &settings.angle);
      break;
    /*  
    case LIMIT:
      lv_slider_set_range(slider, min, max);
      lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, &settings.speed);
      break;
    */
  }

  return slider;
}

lv_obj_t *create_title(lv_obj_t *parent, const char *text) {
  lv_obj_t* label = lv_label_create(parent);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, text);

  return label;
}

lv_obj_t *create_title_special(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color) {
  lv_obj_t* label = lv_label_create(parent);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, text);
  
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, color, 0);

  return label;
}