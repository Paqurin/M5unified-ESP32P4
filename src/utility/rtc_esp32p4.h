#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

typedef struct {
    struct {
        uint16_t year;
        uint8_t month;
        uint8_t date;
        uint8_t weekday;
    } date;
    struct {
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
    } time;
} rtc_datetime_t;

esp_err_t rtc_init(void);
bool rtc_set_datetime(const rtc_datetime_t* datetime);
bool rtc_get_datetime(rtc_datetime_t* datetime);
void rtc_deinit(void);