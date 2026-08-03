#include "UIstyles.h"

void styleinit_button(lv_style_t &style_button, lv_style_t &style_button_pressed, int button_type) {
  switch (button_type) {
    case NORMAL_BUTTON:
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
      lv_style_set_text_font(&style_button, &lv_font_montserrat_24);

      lv_style_init(&style_button_pressed);
      lv_style_set_bg_color(&style_button_pressed, lv_color_hex(0xbdbdbd));
      lv_style_set_bg_grad_color(&style_button_pressed, lv_color_hex(0x757575));
      break;

    case START_BUTTON:
      lv_style_init(&style_button);
      lv_style_set_radius(&style_button, 10);
      lv_style_set_bg_opa(&style_button, (255*100/100));
      lv_style_set_bg_color(&style_button, lv_color_hex(0xeeeeee)); //white to
      lv_style_set_bg_grad_color(&style_button, lv_color_hex(0x30ff20)); //bright green
      lv_style_set_bg_grad_dir(&style_button, LV_GRAD_DIR_VER);
      lv_style_set_border_color(&style_button, lv_color_hex(0x000000)); //black
      lv_style_set_border_opa(&style_button, (255*20/100));
      lv_style_set_border_width(&style_button, 2);
      lv_style_set_text_color(&style_button, lv_color_hex(0x000000)); //black
      lv_style_set_text_font(&style_button, &lv_font_montserrat_24);

      lv_style_init(&style_button_pressed);
      lv_style_set_bg_color(&style_button_pressed, lv_color_hex(0xbdbdbd)); //light grey to
      lv_style_set_bg_grad_color(&style_button_pressed, lv_color_hex(0x0ccd00)); //bright green, but less saturated  
      break;

    case STOP_BUTTON:
      lv_style_init(&style_button);
      lv_style_set_radius(&style_button, 10);
      lv_style_set_bg_opa(&style_button, (255*100/100));
      lv_style_set_bg_color(&style_button, lv_color_hex(0xeeeeee)); //white to
      lv_style_set_bg_grad_color(&style_button, lv_color_hex(0xff2020)); //bright red
      lv_style_set_bg_grad_dir(&style_button, LV_GRAD_DIR_VER);
      lv_style_set_border_color(&style_button, lv_color_hex(0x000000)); //black
      lv_style_set_border_opa(&style_button, (255*20/100));
      lv_style_set_border_width(&style_button, 2);
      lv_style_set_text_color(&style_button, lv_color_hex(0x000000)); //black
      lv_style_set_text_font(&style_button, &lv_font_montserrat_24);

      lv_style_init(&style_button_pressed);
      lv_style_set_bg_color(&style_button_pressed, lv_color_hex(0xbdbdbd)); //light grey to
      lv_style_set_bg_grad_color(&style_button_pressed, lv_color_hex(0xcd2020)); //bright red, but less saturated  
      break;

    case RESET_BUTTON: //I dunno how i wanna do this one yet, no color but it has to be distinguishable from the normal buttons in some way
      break;
  }
}