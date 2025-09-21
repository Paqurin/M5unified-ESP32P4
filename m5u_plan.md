# M5Unified-ESP32P4 Modernization Plan for ESP-IDF 5.4.1

## Problem Analysis

The current M5Unified-ESP32P4 port at `https://github.com/Paqurin/M5unified-ESP32P4` uses **legacy ESP-IDF component format** that causes build failures with ESP-IDF 5.4.1.

### Current Build Errors
```
CMake Error at managed_components/Paqurin__M5unified-ESP32P4/CMakeLists.txt:22 (target_compile_definitions):
  target_compile_definitions command is not scriptable
```

### Root Causes
1. **Legacy CMakeLists.txt format**: Uses old `register_component()` and variable names
2. **Missing component manifest**: No `idf_component.yml` for modern ESP-IDF
3. **Invalid component dependencies**: References non-existent `M5GFX`, `arduino-esp32`
4. **Incorrect target usage**: `target_compile_definitions` called outside proper context

## Detailed Modernization Steps

### Step 1: Create Component Manifest
**File: `idf_component.yml`**

```yaml
## IDF Component Manager Manifest File
version: "1.0.0"
description: "M5Unified library optimized for ESP32-P4 and M5Stack Tab5"
url: "https://github.com/Paqurin/M5unified-ESP32P4"
documentation:
  url: "https://github.com/Paqurin/M5unified-ESP32P4/blob/main/README_ESP32P4.md"

dependencies:
  idf: ">=5.3"
  # Remove M5GFX and arduino-esp32 dependencies - not available for ESP32-P4
  # Only use core ESP-IDF components
  esp_driver_gpio: "*"
  esp_driver_i2c: "*"
  driver: "*"

targets:
  - esp32p4

maintainers:
  - name: "Paqurin"
    email: "your-email@domain.com"
```

### Step 2: Modernize CMakeLists.txt
**File: `CMakeLists.txt`**

```cmake
# M5Unified-ESP32P4 - Modern ESP-IDF 5.4.1 Compatible Component

# Collect all source files
file(GLOB_RECURSE SRCS
    "src/*.cpp"
    "src/utility/*.cpp"
    "src/utility/imu/*.cpp"
)

# Register the component using modern ESP-IDF format
idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS "src"
    REQUIRES
        esp_driver_gpio
        esp_driver_i2c
        driver
        esp_system
        esp_timer
        freertos
        log
        nvs_flash
    PRIV_REQUIRES
        esp_driver_ledc
)

# Add ESP32-P4 specific compile definitions AFTER component registration
target_compile_definitions(${COMPONENT_LIB} PUBLIC
    CONFIG_IDF_TARGET_ESP32P4=1
    M5UNIFIED_ESP32P4_ONLY=1
    BOARD_HAS_PSRAM=1
    ARDUINO_M5TAB5=1
)

# Set C++ standard for component
target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)

message(STATUS "M5Unified-ESP32P4 component registered successfully")
```

### Step 3: Fix Source Code Dependencies

#### A. Remove Arduino Dependencies
**In `src/M5Unified.hpp` and `src/M5Unified.cpp`:**

1. Replace Arduino includes:
```cpp
// Remove:
#include <Arduino.h>
#include <Wire.h>

// Replace with:
#include "esp_system.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
```

2. Replace Arduino types:
```cpp
// Replace String with std::string or const char*
// Replace Arduino GPIO functions with ESP-IDF gpio_* functions
// Replace Wire.* with esp-idf i2c_master_* functions
```

#### B. Implement Native ESP-IDF RTC Functions
**Create new file: `src/utility/rtc_esp32p4.cpp`**

```cpp
#include "rtc_esp32p4.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

#define RTC_I2C_ADDR 0x51  // Common RTC address for M5Stack devices
#define I2C_MASTER_SCL_IO 32
#define I2C_MASTER_SDA_IO 31
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

static const char* TAG = "M5_RTC";
static i2c_master_bus_handle_t i2c_bus_handle = NULL;
static i2c_master_dev_handle_t rtc_dev_handle = NULL;

// Initialize I2C for RTC communication
esp_err_t rtc_init(void) {
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {
            .enable_internal_pullup = true,
        }
    };

    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t rtc_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = RTC_I2C_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_master_bus_add_device(i2c_bus_handle, &rtc_cfg, &rtc_dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add RTC device: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "RTC initialized successfully");
    return ESP_OK;
}

// BCD conversion functions
static uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) + (val % 10);
}

static uint8_t bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// Set RTC time
bool rtc_set_datetime(const rtc_datetime_t* datetime) {
    if (!rtc_dev_handle) {
        ESP_LOGE(TAG, "RTC not initialized");
        return false;
    }

    uint8_t data[8];
    data[0] = 0x00; // Register address (seconds)
    data[1] = dec_to_bcd(datetime->time.seconds);
    data[2] = dec_to_bcd(datetime->time.minutes);
    data[3] = dec_to_bcd(datetime->time.hours);
    data[4] = dec_to_bcd(datetime->date.weekday);
    data[5] = dec_to_bcd(datetime->date.date);
    data[6] = dec_to_bcd(datetime->date.month);
    data[7] = dec_to_bcd(datetime->date.year % 100);

    esp_err_t ret = i2c_master_transmit(rtc_dev_handle, data, sizeof(data), 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set RTC time: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "RTC time set successfully");
    return true;
}

// Get RTC time
bool rtc_get_datetime(rtc_datetime_t* datetime) {
    if (!rtc_dev_handle) {
        ESP_LOGE(TAG, "RTC not initialized");
        return false;
    }

    uint8_t reg_addr = 0x00;
    uint8_t data[7];

    // Write register address
    esp_err_t ret = i2c_master_transmit(rtc_dev_handle, &reg_addr, 1, 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write RTC register address: %s", esp_err_to_name(ret));
        return false;
    }

    // Read time data
    ret = i2c_master_receive(rtc_dev_handle, data, sizeof(data), 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read RTC time: %s", esp_err_to_name(ret));
        return false;
    }

    datetime->time.seconds = bcd_to_dec(data[0] & 0x7F);
    datetime->time.minutes = bcd_to_dec(data[1] & 0x7F);
    datetime->time.hours = bcd_to_dec(data[2] & 0x3F);
    datetime->date.weekday = bcd_to_dec(data[3] & 0x07);
    datetime->date.date = bcd_to_dec(data[4] & 0x3F);
    datetime->date.month = bcd_to_dec(data[5] & 0x1F);
    datetime->date.year = 2000 + bcd_to_dec(data[6]);

    return true;
}
```

**Create header: `src/utility/rtc_esp32p4.h`**

```cpp
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
```

### Step 4: Update M5Unified Main Interface

**Modify `src/M5Unified.hpp`:**

```cpp
#pragma once

#include "esp_system.h"
#include "esp_log.h"
#include "utility/rtc_esp32p4.h"

class M5Unified_Class {
public:
    struct RTC_Class {
        bool setDateTime(const rtc_datetime_t& datetime) {
            return rtc_set_datetime(&datetime);
        }

        bool getDateTime(rtc_datetime_t* datetime) {
            return rtc_get_datetime(datetime);
        }
    } Rtc;

    void begin() {
        ESP_LOGI("M5Unified", "Initializing M5Unified for ESP32-P4");
        rtc_init();
    }
};

extern M5Unified_Class M5;
```

**Modify `src/M5Unified.cpp`:**

```cpp
#include "M5Unified.hpp"

M5Unified_Class M5;
```

### Step 5: Testing and Integration

#### A. Build Test
1. Update component in your repository
2. Test build with Tab5_Launcher project:
```bash
. ~/esp/esp-idf-5.4.1/export.sh
idf.py reconfigure
idf.py build
```

#### B. Runtime Test
1. Flash to Tab5 device
2. Test RTC functions in settings screen
3. Verify time persistence across resets

### Step 6: Version and Documentation

#### A. Update Version
**In `library.json`:**
```json
{
  "version": "1.1.0-esp32p4",
  "description": "M5Unified library optimized for ESP32-P4 with ESP-IDF 5.4.1 compatibility"
}
```

#### B. Update README
Add section about ESP-IDF 5.4.1 compatibility and usage examples.

## Validation Checklist

- [ ] Component builds without errors in ESP-IDF 5.4.1
- [ ] `M5.begin()` initializes successfully
- [ ] `M5.Rtc.setDateTime()` sets hardware RTC
- [ ] `M5.Rtc.getDateTime()` reads hardware RTC
- [ ] Time persists across device resets
- [ ] Integration works with Tab5_Launcher project
- [ ] No Arduino dependencies remain
- [ ] Uses only ESP-IDF 5.4.1 compatible APIs

## Expected Outcome

After implementing these changes, the M5Unified-ESP32P4 component will:

1. **Build successfully** with ESP-IDF 5.4.1
2. **Provide working RTC functionality** for M5Stack Tab5
3. **Integrate seamlessly** with the Tab5_Launcher project
4. **Maintain API compatibility** for existing code
5. **Use native ESP-IDF drivers** instead of Arduino libraries

This will enable proper battery-backed RTC functionality in the Tab5_Launcher, solving the time reset issues you're experiencing.