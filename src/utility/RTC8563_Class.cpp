// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "RTC8563_Class.hpp"

#include <stdlib.h>

namespace m5
{
  tm rtc_datetime_t::get_tm(void) const
  {
    tm t_st = {
      time.seconds,
      time.minutes,
      time.hours,
      date.date,
      date.month - 1,
      date.year - 1900,
      date.weekDay,
      0,
      0,
    };
    return t_st;
  }

  void rtc_datetime_t::set_tm(tm& datetime)
  {
    date = rtc_date_t { datetime };
    time = rtc_time_t { datetime };
  }

  static std::uint8_t bcd2ToByte(std::uint8_t value)
  {
    return ((value >> 4) * 10) + (value & 0x0F);
  }

  static std::uint8_t byteToBcd2(std::uint8_t value)
  {
    std::uint_fast8_t bcdhigh = value / 10;
    return (bcdhigh << 4) | (value - (bcdhigh * 10));
  }

  // Convert between M5Unified and native ESP-IDF datetime formats
  static void m5_to_native(const rtc_datetime_t& m5_dt, ::rtc_datetime_t& native_dt)
  {
    native_dt.date.year = m5_dt.date.year;
    native_dt.date.month = m5_dt.date.month;
    native_dt.date.date = m5_dt.date.date;
    native_dt.date.weekday = m5_dt.date.weekDay;
    native_dt.time.hours = m5_dt.time.hours;
    native_dt.time.minutes = m5_dt.time.minutes;
    native_dt.time.seconds = m5_dt.time.seconds;
  }

  static void native_to_m5(const ::rtc_datetime_t& native_dt, rtc_datetime_t& m5_dt)
  {
    m5_dt.date.year = native_dt.date.year;
    m5_dt.date.month = native_dt.date.month;
    m5_dt.date.date = native_dt.date.date;
    m5_dt.date.weekDay = native_dt.date.weekday;
    m5_dt.time.hours = native_dt.time.hours;
    m5_dt.time.minutes = native_dt.time.minutes;
    m5_dt.time.seconds = native_dt.time.seconds;
  }

  bool RTC8563_Class::begin(I2C_Class* i2c)
  {
    // Initialize native ESP-IDF RTC instead of legacy I2C implementation
    esp_err_t ret = rtc_init();
    _init = (ret == ESP_OK);
    return _init;
  }

  bool RTC8563_Class::isEnabled(void) const
  {
    return _init;
  }

  bool RTC8563_Class::getVoltLow(void)
  {
    // For native ESP-IDF implementation, assume power is always OK
    return false;
  }

  bool RTC8563_Class::getDateTime(rtc_datetime_t* datetime) const
  {
    if (!datetime || !_init) return false;

    ::rtc_datetime_t native_dt;
    bool success = rtc_get_datetime(&native_dt);
    if (success) {
      native_to_m5(native_dt, *datetime);
    }
    return success;
  }


  bool RTC8563_Class::getTime(rtc_time_t* time) const
  {
    if (!time || !_init) return false;

    rtc_datetime_t datetime;
    if (getDateTime(&datetime)) {
      *time = datetime.time;
      return true;
    }
    return false;
  }

  void RTC8563_Class::setTime(const rtc_time_t& time)
  {
    if (!_init) return;

    // Get current date and set new time
    rtc_datetime_t datetime;
    if (getDateTime(&datetime)) {
      datetime.time = time;
      setDateTime(datetime);
    }
  }

  bool RTC8563_Class::getDate(rtc_date_t* date) const
  {
    if (!date || !_init) return false;

    rtc_datetime_t datetime;
    if (getDateTime(&datetime)) {
      *date = datetime.date;
      return true;
    }
    return false;
  }

  void RTC8563_Class::setDate(const rtc_date_t& date)
  {
    if (!_init) return;

    // Get current time and set new date
    rtc_datetime_t datetime;
    if (getDateTime(&datetime)) {
      datetime.date = date;
      setDateTime(datetime);
    }
  }

  void RTC8563_Class::setDateTime(const rtc_datetime_t& datetime)
  {
    if (!_init) return;

    ::rtc_datetime_t native_dt;
    m5_to_native(datetime, native_dt);
    rtc_set_datetime(&native_dt);
  }

  int RTC8563_Class::setAlarmIRQ(int afterSeconds)
  {
    // Simplified alarm implementation for native ESP-IDF
    // TODO: Implement using ESP-IDF timer APIs if needed
    if (afterSeconds < 0) {
      return -1; // disabled
    }
    return afterSeconds; // return as-is for now
  }

  int RTC8563_Class::setAlarmIRQ(const rtc_time_t &time)
  {
    // Simplified alarm implementation for native ESP-IDF
    // TODO: Implement using ESP-IDF timer APIs if needed
    (void)time; // Suppress unused parameter warning
    return 0; // return success for now
  }

  int RTC8563_Class::setAlarmIRQ(const rtc_date_t &date, const rtc_time_t &time)
  {
    // Simplified alarm implementation for native ESP-IDF
    // TODO: Implement using ESP-IDF timer APIs if needed
    (void)date; (void)time; // Suppress unused parameter warnings
    return 0; // return success for now
  }

  bool RTC8563_Class::getIRQstatus(void)
  {
    // Simplified IRQ status for native ESP-IDF
    return false; // no IRQ pending for now
  }

  void RTC8563_Class::clearIRQ(void)
  {
    // Simplified IRQ clear for native ESP-IDF
    // No action needed for basic implementation
  }

  void RTC8563_Class::disableIRQ(void)
  {
    // Simplified IRQ disable for native ESP-IDF
    // No action needed for basic implementation
  }

  void RTC8563_Class::setSystemTimeFromRtc(struct timezone* tz)
  {
#if defined (M5UNIFIED_PC_BUILD)
    (void)tz;
#else
    rtc_datetime_t dt;
    if (getDateTime(&dt))
    {
      tm t_st;
      t_st.tm_isdst = -1;
      t_st.tm_year = dt.date.year - 1900;
      t_st.tm_mon  = dt.date.month - 1;
      t_st.tm_mday = dt.date.date;
      t_st.tm_hour = dt.time.hours;
      t_st.tm_min  = dt.time.minutes;
      t_st.tm_sec  = dt.time.seconds;
      timeval now;
      // mktime(3) uses localtime, force UTC
      char *oldtz = getenv("TZ");
      setenv("TZ", "GMT0", 1);
      tzset(); // Workaround for https://github.com/espressif/esp-idf/issues/11455
      now.tv_sec = mktime(&t_st);
      if (oldtz)
      {
        setenv("TZ", oldtz, 1);
      } else {
        unsetenv("TZ");
      }
      now.tv_usec = 0;
      settimeofday(&now, tz);
    }
#endif
  }
}
