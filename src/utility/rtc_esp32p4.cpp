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

// BCD conversion functions
static uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) + (val % 10);
}

static uint8_t bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// Initialize I2C for RTC communication
esp_err_t rtc_init(void) {
    if (i2c_bus_handle != NULL) {
        ESP_LOGW(TAG, "RTC already initialized");
        return ESP_OK;
    }

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
        i2c_del_master_bus(i2c_bus_handle);
        i2c_bus_handle = NULL;
        return ret;
    }

    ESP_LOGI(TAG, "RTC initialized successfully");
    return ESP_OK;
}

// Cleanup I2C resources
void rtc_deinit(void) {
    if (rtc_dev_handle != NULL) {
        i2c_master_bus_rm_device(rtc_dev_handle);
        rtc_dev_handle = NULL;
    }
    if (i2c_bus_handle != NULL) {
        i2c_del_master_bus(i2c_bus_handle);
        i2c_bus_handle = NULL;
    }
    ESP_LOGI(TAG, "RTC deinitialized");
}

// Set RTC time
bool rtc_set_datetime(const rtc_datetime_t* datetime) {
    if (!rtc_dev_handle) {
        ESP_LOGE(TAG, "RTC not initialized");
        return false;
    }

    if (!datetime) {
        ESP_LOGE(TAG, "Invalid datetime pointer");
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

    ESP_LOGI(TAG, "RTC time set: %04d-%02d-%02d %02d:%02d:%02d",
             datetime->date.year, datetime->date.month, datetime->date.date,
             datetime->time.hours, datetime->time.minutes, datetime->time.seconds);
    return true;
}

// Get RTC time
bool rtc_get_datetime(rtc_datetime_t* datetime) {
    if (!rtc_dev_handle) {
        ESP_LOGE(TAG, "RTC not initialized");
        return false;
    }

    if (!datetime) {
        ESP_LOGE(TAG, "Invalid datetime pointer");
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

    ESP_LOGD(TAG, "RTC time read: %04d-%02d-%02d %02d:%02d:%02d",
             datetime->date.year, datetime->date.month, datetime->date.date,
             datetime->time.hours, datetime->time.minutes, datetime->time.seconds);
    return true;
}