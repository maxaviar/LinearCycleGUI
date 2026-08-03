#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <lvgl.h>
#include "PCA9557.h"
#include "gfx_conf.h"

/*
All macros used for sizing and positioning of objects
*/
#define PAD20 20
#define PAD30 30
#define PAD40 40

#define BTN_W 180
#define BTN_H 60

#define NORMAL_BUTTON   0
#define START_BUTTON    1
#define STOP_BUTTON     2
#define RESET_BUTTON    3

#define TITLE_H 80