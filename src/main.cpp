#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <lvgl.h>
#include "PCA9557.h"
#include "gfx_conf.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;

PCA9557 Out;    //for touch timing init

/* Initializing display */
void display_init();

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

/* Setting styles */
void styleinit_button(lv_style_t &style_button, lv_style_t &style_button_pressed);

/* Creating objects */
void objcreate_button(
  lv_style_t &style_button,
  lv_style_t &style_button_pressed,
  lv_obj_t button,
  lv_coord_t x,
  lv_coord_t y,
  lv_coord_t w,
  lv_coord_t h
);

void setup() {
  Serial.begin(115200);
  Serial.println("LVGL Widgets Demo");

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
  static lv_style_t style_button;
  static lv_style_t style_button_pressed;
  styleinit_button(style_button, style_button_pressed);

  /* Creating objects */
  lv_obj_t * btn_speed = lv_btn_create(screen);
  lv_obj_set_align(btn_speed, LV_ALIGN_CENTER);
  lv_obj_set_y(btn_speed, 0);
  lv_obj_set_size(btn_speed, 80, 40);
  lv_obj_add_style(btn_speed, &style_button, 0);
  lv_obj_add_style(btn_speed, &style_button_pressed, LV_STATE_PRESSED);

  /* Drawing objects */
  lv_obj_t *lbl_speed = lv_label_create(btn_speed);
  lv_obj_set_align(lbl_speed, LV_ALIGN_CENTER);
  lv_label_set_text(lbl_speed, "Button");

  Serial.println("Setup done");
}

void loop() {
    lv_timer_handler();
    delay(5);
}

void display_init() {
  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  delay(200);

  lv_init();

  delay(100);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight/10);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);

   lv_disp_flush_ready( disp );

}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}

void styleinit_button(lv_style_t &style_button, lv_style_t &style_button_pressed) {
  //After verifying this works, change the colors off of hex
  lv_style_init(&style_button);
  lv_style_set_radius(&style_button, 10);
  lv_style_set_bg_opa(&style_button, (255*100/100));
  lv_style_set_bg_color(&style_button, lv_color_hex(0xeeeeee));
  lv_style_set_bg_grad_color(&style_button, lv_color_hex(0x9e9e9e));
  lv_style_set_bg_grad_dir(&style_button, LV_GRAD_DIR_VER);
  lv_style_set_border_color(&style_button, lv_color_hex(0x000000));
  lv_style_set_border_opa(&style_button, (255*20/100));
  lv_style_set_border_width(&style_button, 2);
  lv_style_set_text_color(&style_button, lv_color_hex(0x000000));

  lv_style_init(&style_button_pressed);
  lv_style_set_bg_color(&style_button_pressed, lv_color_hex(0xbdbdbd));
  lv_style_set_bg_grad_color(&style_button_pressed, lv_color_hex(0x757575));
}

void objcreate_button(lv_style_t &style_button, lv_style_t &style_button_pressed,
  lv_obj_t button, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {

}