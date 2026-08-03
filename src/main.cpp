#include "common.h"
#include "firmware.h"
#include "UIstyles.h"
#include "UIobjects.h"

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
  //Buttons for changing settings
  lv_obj_t * btn_speed = lv_btn_create(screen);
  objcreate_button(style_norm_btn, style_norm_btn_p, btn_speed, PAD30, -(PAD30+BTN_H)/2 - PAD40, BTN_W, BTN_H);

  lv_obj_t * btn_dwell = lv_btn_create(screen);
  objcreate_button(style_norm_btn, style_norm_btn_p, btn_dwell, PAD30, (PAD30+BTN_H)/2 - PAD40, BTN_W, BTN_H);

  lv_obj_t * btn_angle = lv_btn_create(screen);
  objcreate_button(style_norm_btn, style_norm_btn_p, btn_angle, PAD30, (3*(PAD30+BTN_H))/2 - PAD40, BTN_W, BTN_H);

  lv_obj_t * btn_count = lv_btn_create(screen);
  objcreate_button(style_norm_btn, style_norm_btn_p, btn_count, PAD30, (5*(PAD30+BTN_H))/2 - PAD40, BTN_W, BTN_H);

  //Title
  lv_obj_t *txt_title = lv_obj_create(screen);
  lv_obj_set_size(txt_title, screenWidth - 2*(PAD30), TITLE_H);
  lv_obj_set_align(txt_title, LV_ALIGN_TOP_MID);
  lv_obj_set_y(txt_title, PAD20);

  //Count
  lv_obj_t *txt_count = lv_obj_create(screen);
  lv_obj_set_size(txt_count, (2*(screenWidth - 2*(PAD30)))/3, TITLE_H);
  lv_obj_set_align(txt_count, LV_ALIGN_RIGHT_MID);
  lv_obj_set_x(txt_count, -PAD30);
  lv_obj_set_y(txt_count, -(PAD30+BTN_H)/2 - PAD40);

  //Horizontal line
  static lv_point_t line_points[] = {{screenWidth/3,screenHeight/2}, {screenWidth-PAD30, screenHeight/2}}; //Adjust y (eventually)
  
  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_width(&style_line, 8);
  lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_line_rounded(&style_line, true);

  lv_obj_t *h_line = lv_line_create(screen);
  lv_line_set_points(h_line, line_points, 2);
  lv_obj_add_style(h_line, &style_line, 0);

  /* Drawing objects */
  lv_obj_t *lbl_speed = create_title(btn_speed, "Speed");
  lv_obj_t *lbl_dwell = create_title(btn_dwell, "Dwell");
  lv_obj_t *lbl_angle = create_title(btn_angle, "Angle");
  lv_obj_t *lbl_count = create_title(btn_count, "Count");

  lv_obj_t *lbl_title = create_title(txt_title, "Cycle Tester - Rotational Motion");
  lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_28_compressed, LV_PART_MAIN); //find out why this isn't bold
  lv_obj_t *lbl_txt_count = create_title(txt_count, "Count = 123456");
  lv_obj_set_style_text_font(lbl_txt_count, &lv_font_montserrat_40, 0);

  Serial.println("Setup done");
}

void loop() {
    lv_timer_handler();
    delay(5);
}