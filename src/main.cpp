#include "common.h"
#include "firmware.h"
#include "UIstyles.h"
#include "UIobjects.h"

unsigned long st_time;
const unsigned long SEND_INTERVAL = 300;

static lv_obj_t *lbl_progress_num;

// TX going through Serial
// RX going through Serial2
#define RX_PIN 38

void setup() {
  Serial.begin(115200, SERIAL_8N1, 44, 43);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, -1);

  //GPIO init
  #if defined (CrowPanel_50) || defined (CrowPanel_70)
  //pinMode(38, OUTPUT);    Being used as RX2
  //digitalWrite(38, LOW);
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
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //black background
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
  
  /* Creating flexboxes */
  lv_obj_t *settings_area = flexbox(screen, LV_ALIGN_LEFT_MID, LV_FLEX_FLOW_COLUMN, 25, 80, 5, 0);
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
  lv_obj_t *btn_time = objcreate_button(settings_area, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  lv_obj_t *btn_dwell = objcreate_button(settings_area, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  lv_obj_t *btn_distance = objcreate_button(settings_area, style_norm_btn, style_norm_btn_p, BTN_W, BTN_H);
  
  //Progress menu
  lv_obj_t *time_slider_panel = create_panel(btn_time, screen);
  lv_obj_t *time_slider = make_slider(btn_time, time_slider_panel, MIN_TIME, MAX_TIME, TIME);
  lv_obj_t *dwell_slider_panel = create_panel(btn_dwell, screen);
  lv_obj_t *dwell_slider = make_slider(btn_dwell, dwell_slider_panel, MIN_DWELL, MAX_DWELL, DWELL);
  lv_obj_t *distance_slider_panel = create_panel(btn_distance, screen);
  lv_obj_t *distance_slider = make_slider(btn_distance, distance_slider_panel, MIN_DISTANCE, MAX_DISTANCE, DISTANCE);

  //Start/Stop menu
  lv_obj_t *btn_strt = objcreate_button(startstop, style_strt_btn, style_strt_btn_p, 40, 90);
  lv_obj_t *btn_stop = objcreate_button(startstop, style_stop_btn, style_stop_btn_p, 40, 90);
  startstopswitch(btn_strt, btn_stop);
  lv_obj_t *btn_limit = objcreate_button(startstop, style_norm_btn, style_norm_btn_p, 40, 90);

  /* Creating text */
  lv_obj_t *lbl_time = create_title(btn_time, "Time");
  lv_obj_t *lbl_dwell = create_title(btn_dwell, "Dwell");
  lv_obj_t *lbl_distance = create_title(btn_distance, "Distance");
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
  
  lbl_progress_num = lv_label_create(row1);
  lv_obj_set_align(row1, LV_ALIGN_CENTER);
  lv_label_set_text_fmt(lbl_progress_num, "%ld", settings.count);
  lv_obj_set_style_text_font(lbl_progress_num, &lv_font_montserrat_48, 0);
  lv_obj_set_style_text_color(lbl_progress_num, lv_color_white(), 0);
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
}

void loop() {
  lv_timer_handler();

  if ((st_time + SEND_INTERVAL) < millis()) { // ESP32 TX0 to Arduino RX1
    Serial.printf("%d, %d, %d, %d, %d, %d, %ld\n",
                  settings.time,
                  settings.dwell,
                  settings.distance,
                  settings.limit,
                  settings.start,
                  settings.limit_en,
                  settings.count);
    st_time = millis();
  }
  
  if (Serial2.available()) { //Arduino TX1 to ESP32 RX2
    String receivedData = Serial2.readStringUntil('\n');
    settings.count = receivedData.toInt();
    if (lbl_progress_num != nullptr) lv_label_set_text_fmt(lbl_progress_num, "%ld", settings.count);
  }
  
  delay(50);
}