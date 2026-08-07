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
#define MIN_TIME 1
#define MAX_TIME 10
#define MIN_DWELL 0
#define MAX_DWELL 5000
#define MIN_DISTANCE 5
#define MAX_DISTANCE 15

#define TIME 0
#define DWELL 1
#define DISTANCE 2 
#define LIMIT 3

struct Settings {
    int time = 1; // in seconds | Ranges from 1 to 10
    unsigned long dwell = 0; // in milliseconds | Ranges from 0 to 5000
    int distance = 5; // in millimeters | Ranges from 5 to 15
    int32_t limit = 0;
    bool start = false;
    bool limit_en = false;
    long count = 0;
};

extern Settings settings;
