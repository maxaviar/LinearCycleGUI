#include "common.h"
#include "firmware.h"
#include "UIstyles.h"
#include "UIobjects.h"

//io44 - rx0
//io43 - tx0
//3v3
//gnd
unsigned long st_time;
unsigned long interval = 1000;

String receivedData = "";

void setup() {
  Serial.begin(115200);

  //GPIO init
  #if defined (CrowPanel_50) || defined (CrowPanel_70)
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);

  //touch timing init
  Wire.begin(19, 20);
  Out.reset();
  Out.setMode(IO_OUTPUT);
  Out.setState(IO0, IO_LOW);
  Out.setState(IO1, IO_LOW);
  delay(20);
  Out.setState(IO0, IO_HIGH);
  delay(100);
  Out.setMode(IO1, IO_INPUT);

  #elif defined (CrowPanel_43)
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);
  pinMode(35, OUTPUT);
  digitalWrite(35, LOW);
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(0, OUTPUT);//TOUCH-CS
  #endif

  display_init();
  tft.fillScreen(TFT_BLACK);

  lv_obj_t *screen = lv_scr_act();
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //grey background
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
  
  /* Creating flexboxes */
  lv_obj_t *settings = flexbox(screen, LV_ALIGN_LEFT_MID, LV_FLEX_FLOW_COLUMN, 25, 80, 5, 0);
  lv_obj_t *progress = flexbox(screen, LV_ALIGN_TOP_RIGHT, LV_FLEX_FLOW_COLUMN, 60, 45, -5, 10);
  lv_obj_t *startstop = flexbox(screen, LV_ALIGN_BOTTOM_RIGHT, LV_FLEX_FLOW_ROW, 60, 25, -5, -10);
  
  /* Creating styles */
  static lv_style_t style_norm_btn;
  static lv_style_t style_norm_btn_p;
  styleinit_button(style_norm_btn, style_norm_btn_p, NORMAL_BUTTON);

  static lv_style_t style_strt_btn;
  static lv_style_t style_strt_btn_p;
  styleinit_button(style_strt_btn, style_strt_btn_p, START_BUTTON);

  static lv_style_t style_stop_btn;
  static lv_style_t style_stop_btn_p;
  styleinit_button(style_stop_btn, style_stop_btn_p, STOP_BUTTON);

  static lv_style_t style_rst_btn;
  static lv_style_t style_rst_btn_p;
  styleinit_button(style_rst_btn, style_rst_btn_p, RESET_BUTTON);

  /* Creating objects */
  //Settings menu
  lv_obj_t *btn_speed = objcreate_button(settings, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  lv_obj_t *btn_dwell = objcreate_button(settings, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  lv_obj_t *btn_angle = objcreate_button(settings, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  
  //Progress menu
  lv_obj_t *speed_slider_panel = create_panel(btn_speed, screen);
  lv_obj_t *speed_slider = make_slider(btn_speed, speed_slider_panel, MIN_SPEED, MAX_SPEED, SPEED);
  lv_obj_t *dwell_slider_panel = create_panel(btn_dwell, screen);
  lv_obj_t *dwell_slider = make_slider(btn_dwell, dwell_slider_panel, MIN_DWELL, MAX_DWELL, DWELL);
  lv_obj_t *angle_slider_panel = create_panel(btn_angle, screen);
  lv_obj_t *angle_slider = make_slider(btn_angle, angle_slider_panel, MIN_ANGLE, MAX_ANGLE, ANGLE);

  //Start/Stop menu
  lv_obj_t *btn_strt = objcreate_button(startstop, style_strt_btn, style_strt_btn_p, 40, 90);
  lv_obj_t *btn_stop = objcreate_button(startstop, style_stop_btn, style_stop_btn_p, 40, 90);
  startstopswitch(btn_strt, btn_stop);
  lv_obj_t *btn_limit = objcreate_button(startstop, style_norm_btn, style_norm_btn_p, 40, 90);

  /* Creating text */
  lv_obj_t *lbl_speed = create_title(btn_speed, "Speed");
  lv_obj_t *lbl_dwell = create_title(btn_dwell, "Dwell");
  lv_obj_t *lbl_angle = create_title(btn_angle, "Angle");
  lv_obj_t *lbl_strt = create_title(btn_strt, "Start");
  lv_obj_t *lbl_stop = create_title(btn_stop, "Stop");
  lv_obj_t *lbl_limit = create_limit(btn_limit);

  /* Rows for progress box*/
  // Row 1
  lv_obj_t * row1 = lv_obj_create(progress);
  lv_obj_remove_style_all(row1);
  lv_obj_set_width(row1, lv_pct(100));
  lv_obj_set_height(row1, lv_pct(45));
  lv_obj_set_layout(row1, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(row1, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_t *lbl_progress_text = create_title_special(row1, "Count:", &lv_font_montserrat_48, lv_color_white());
  lv_obj_t *lbl_progress_num = create_title_special(row1, "123456", &lv_font_montserrat_48, lv_color_white());
  lv_obj_set_flex_grow(lbl_progress_num, 0.1);

  // Row 2
  lv_obj_t * row2 = lv_obj_create(progress);
  lv_obj_remove_style_all(row2);
  lv_obj_set_width(row2, lv_pct(100));
  lv_obj_set_height(row2, lv_pct(45));
  lv_obj_set_layout(row2, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(row2, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row2, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_t *lbl_target = create_title_special(row2, "Target:", &lv_font_montserrat_36, lv_color_white());
  lv_obj_t *txtarea = create_textarea(row2);
  lv_obj_set_flex_grow(txtarea, 0.1);

  st_time = millis();
  Serial.println("Setup done");
}

void loop() {
    lv_timer_handler();

    if ((st_time + interval) < millis()) { // Sends info once per second
      Serial.print(settings.speed);
      Serial.print(",");
      Serial.print(settings.dwell);
      Serial.print(",");
      Serial.print(settings.angle);
      Serial.print(",");
      Serial.print(settings.limit);
      Serial.print(",");
      Serial.print(settings.start);
      Serial.print(",");
      Serial.print(settings.apply_limit);
      Serial.print('\n');

      st_time = millis();
    }

    if (Serial.available()) {
      receivedData = Serial.readStringUntil('\n');
      settings.count = receivedData.toInt();
    }

    delay(5);
}