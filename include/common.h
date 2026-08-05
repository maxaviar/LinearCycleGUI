#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <lvgl.h>
#include "PCA9557.h"
#include "gfx_conf.h"

/* Sizing and positioning of objects */
#define PAD20 20
#define PAD30 30
#define PAD40 40

#define BTN_W 95
#define BTN_H 20

#define NORMAL_BUTTON   0
#define START_BUTTON    1
#define STOP_BUTTON     2
#define RESET_BUTTON    3

#define TITLE_H 80

/* Settings for operation */
#define MIN_SPEED 100
#define MAX_SPEED 200
#define MIN_DWELL 0
#define MAX_DWELL 5000
#define MIN_ANGLE 90
#define MAX_ANGLE 270

#define SPEED 0
#define DWELL 1
#define ANGLE 2 
#define LIMIT 3

struct Settings {
    int32_t speed = 100; // in steps/sec. | Ranges from 100 to 200
    int32_t dwell = 0; // in millisec | Ranges from 0 to 5000
    int32_t angle = 90; // in degrees | 90 to 270
    int32_t limit = 0;
    bool start = false;
    bool apply_limit = false;
    long count = 0;
};

extern Settings settings;
extern bool limit_en;