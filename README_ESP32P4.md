# M5Unified-ESP32P4

ESP32-P4 optimized version of M5Unified library specifically designed for **M5Stack Tab5** hardware.

## Overview

This is a specialized fork of M5Unified that focuses exclusively on ESP32-P4 platform and M5Stack Tab5 hardware, providing enhanced compatibility and optimizations for:

- **GPIO Configuration**: ESP32-P4 specific pin mappings and configurations
- **I2S Audio System**: Enhanced compatibility with ESP32-P4 I2S drivers
- **Power Management**: Tab5-specific power management features
- **PSRAM Utilization**: Optimized for 32MB PSRAM configuration
- **Display Integration**: Native support for Tab5's high-resolution display

## Key Features

### Hardware Support
- **Target Platform**: ESP32-P4 only
- **Supported Devices**: M5Stack Tab5
- **Memory**: 32MB PSRAM, 16MB Flash
- **Display**: High-resolution with PPA acceleration support
- **Audio**: Dual microphone PDM interface
- **Connectivity**: USB OTG, I2C, SPI, GPIO

### ESP32-P4 Specific Optimizations
- Streamlined codebase with non-P4 platform code removed
- ESP32-P4 specific I2S driver compatibility
- Enhanced GPIO pin configuration for Tab5
- Power management optimizations
- Memory management for large PSRAM configurations

## Pin Configuration (Tab5)

### I2C Pins
- **Internal I2C**: SCL=32, SDA=31
- **External I2C**: SCL=54, SDA=53

### Port Configurations
- **Port B**: Pin1=17, Pin2=52
- **Port C**: Pin1=7, Pin2=6

### SPI (SD Card)
- **CLK**: GPIO43
- **MOSI**: GPIO44
- **MISO**: GPIO39
- **CS**: GPIO42

### M-Bus Connector (30-pin)
Comprehensive GPIO mapping for the M-Bus expansion connector with ESP32-P4 specific pin assignments.

## Installation

### PlatformIO
Add to your `platformio.ini`:
```ini
[env:m5tab5]
platform = espressif32
board = esp32-p4-function-ev-board
framework = arduino

lib_deps =
    https://github.com/m5stack/M5Unified-ESP32P4.git

build_flags =
    -DCONFIG_IDF_TARGET_ESP32P4=1
    -DM5UNIFIED_ESP32P4_ONLY=1
    -DBOARD_HAS_PSRAM=1
    -DARDUINO_M5TAB5=1
```

### Arduino IDE
1. Download or clone this repository
2. Install as a library in Arduino IDE
3. Select ESP32-P4 board configuration
4. Include in your sketch: `#include <M5Unified.h>`

## Usage

Basic usage remains the same as standard M5Unified:

```cpp
#include <M5Unified.h>

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    M5.Display.setTextSize(2);
    M5.Display.println("M5Unified-ESP32P4");
    M5.Display.println("Tab5 Ready!");
}

void loop() {
    M5.update();
    // Your code here
}
```

## Build Configuration

The library includes ESP32-P4 specific build flags:
- `CONFIG_IDF_TARGET_ESP32P4=1`
- `M5UNIFIED_ESP32P4_ONLY=1` 
- `BOARD_HAS_PSRAM=1`
- `ARDUINO_M5TAB5=1`

## Compatibility

### Supported Features
- Display and touch interface
- Button management
- I2C communication (internal/external)
- Power management
- IMU integration
- Speaker and microphone
- GPIO and port management
- PSRAM utilization

### Upcoming Features
- Enhanced I2S audio drivers for ESP32-P4
- Advanced power management
- USB OTG integration
- Improved GPIO configuration API

## Development

This ESP32-P4 specific version provides a clean foundation for:
- ESP32-P4 hardware integration
- Tab5-specific feature development
- I2S driver migration and compatibility
- Power management enhancements
- GPIO configuration improvements

## Differences from Standard M5Unified

1. **Platform Focus**: Only ESP32-P4 support, other platforms removed
2. **Simplified Codebase**: Non-P4 hardware configurations stripped out
3. **Enhanced Compatibility**: ESP32-P4 specific driver implementations
4. **Memory Optimization**: 32MB PSRAM specific optimizations
5. **Build Configuration**: ESP32-P4 focused build system

## Contributing

When contributing to M5Unified-ESP32P4:
- Focus on ESP32-P4 platform compatibility
- Test specifically on M5Stack Tab5 hardware
- Consider PSRAM memory management
- Follow ESP32-P4 driver patterns
- Document Tab5-specific features

## License

Same license as original M5Unified - MIT License.

## Acknowledgments

Based on the excellent work of M5Stack and lovyan03 on the original M5Unified library. This ESP32-P4 fork focuses the codebase for enhanced compatibility with the newest ESP32-P4 platform and M5Stack Tab5 hardware.