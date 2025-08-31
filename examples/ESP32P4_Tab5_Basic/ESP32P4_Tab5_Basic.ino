/*
 * M5Unified-ESP32P4 Basic Example for M5Stack Tab5
 * 
 * This example demonstrates the ESP32-P4 specific features and
 * optimizations available in the M5Unified-ESP32P4 library.
 * 
 * Features demonstrated:
 * - ESP32-P4 platform detection
 * - Tab5 hardware initialization
 * - PSRAM utilization
 * - GPIO pin mapping
 * - Display and touch interface
 */

#include <M5Unified.h>

void setup() {
    // ESP32-P4 specific configuration
    auto cfg = M5.config();
    
    // Enable ESP32-P4 features
    cfg.clear_display = true;
    cfg.output_power = true;
    cfg.internal_imu = true;
    cfg.internal_rtc = true;
    cfg.internal_mic = true;
    cfg.internal_spk = true;
    
    // Initialize M5Unified with ESP32-P4 optimizations
    M5.begin(cfg);
    
    // Verify we're running on ESP32-P4
    if (M5.getBoard() != m5::board_t::board_M5Tab5) {
        M5.Display.setTextColor(TFT_RED);
        M5.Display.println("ERROR: Not running on M5Stack Tab5!");
        while(1) delay(1000);
    }
    
    // Display ESP32-P4 information
    M5.Display.clear();
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.setCursor(0, 0);
    
    M5.Display.println("M5Unified-ESP32P4");
    M5.Display.println("M5Stack Tab5");
    M5.Display.println("");
    
    // Show version information
    M5.Display.setTextSize(1);
    M5.Display.printf("Library Version: %s\n", M5UNIFIED_VERSION);
    M5.Display.printf("Board: %s\n", "M5Stack Tab5");
    M5.Display.printf("Platform: %s\n", "ESP32-P4");
    M5.Display.println("");
    
    // Show PSRAM information
    M5.Display.printf("PSRAM Size: %d bytes\n", ESP.getPsramSize());
    M5.Display.printf("PSRAM Free: %d bytes\n", ESP.getFreePsram());
    M5.Display.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
    M5.Display.println("");
    
    // Show GPIO pin information
    M5.Display.println("ESP32-P4 Pin Configuration:");
    M5.Display.printf("I2C Internal: SCL=%d, SDA=%d\n", 
                      M5.getPin(m5::in_i2c_scl), 
                      M5.getPin(m5::in_i2c_sda));
    M5.Display.printf("I2C External: SCL=%d, SDA=%d\n", 
                      M5.getPin(m5::ex_i2c_scl), 
                      M5.getPin(m5::ex_i2c_sda));
    M5.Display.printf("SD SPI: CLK=%d, MOSI=%d\n", 
                      M5.getPin(m5::sd_spi_sclk), 
                      M5.getPin(m5::sd_spi_copi));
    M5.Display.println("");
    
    M5.Display.setTextColor(TFT_GREEN);
    M5.Display.println("ESP32-P4 Tab5 Ready!");
    
    // Initialize components
    Serial.println("M5Unified-ESP32P4 initialized successfully");
    Serial.printf("Running on M5Stack Tab5 (ESP32-P4)\n");
    Serial.printf("PSRAM: %d bytes available\n", ESP.getFreePsram());
}

void loop() {
    M5.update();
    
    // Simple touch detection demo
    static uint32_t last_touch_time = 0;
    if (M5.Touch.getCount() > 0) {
        if (millis() - last_touch_time > 500) {  // Debounce
            auto touch = M5.Touch.getDetail();
            
            // Clear previous touch info area
            M5.Display.fillRect(0, 200, 320, 40, TFT_BLACK);
            M5.Display.setCursor(0, 200);
            M5.Display.setTextColor(TFT_YELLOW);
            M5.Display.printf("Touch: X=%d, Y=%d", touch.x, touch.y);
            
            last_touch_time = millis();
        }
    }
    
    // Show running status
    static uint32_t last_update = 0;
    if (millis() - last_update > 1000) {
        // Update status indicator
        M5.Display.fillRect(280, 0, 40, 20, TFT_BLACK);
        M5.Display.setCursor(280, 0);
        M5.Display.setTextColor(TFT_CYAN);
        M5.Display.printf("%.1fs", millis() / 1000.0f);
        
        last_update = millis();
        
        // Print PSRAM usage to Serial
        Serial.printf("PSRAM Free: %d bytes\n", ESP.getFreePsram());
    }
    
    delay(10);
}