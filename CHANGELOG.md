# Changelog

All notable changes to the M5Unified-ESP32P4 library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0-esp32p4] - 2024-12-31

### 🎯 Initial ESP32-P4 Release

This is the first specialized release of M5Unified for ESP32-P4 RISC-V architecture, focusing exclusively on M5Stack Tab5 hardware compatibility.

### ✅ Added

#### ESP32-P4 Core Support
- **ESP32-P4 RISC-V Architecture**: Native support for ESP32-P4 dual-core RISC-V processor
- **32MB PSRAM Integration**: Optimized memory management for large PSRAM configurations
- **Enhanced Power Management**: ESP32-P4 specific power management with EXT1 wakeup support
- **USB OTG Support**: Basic USB host/device mode compatibility

#### M5Stack Tab5 Hardware
- **Tab5 GPIO Configuration**: Complete GPIO pin mapping for M5Stack Tab5
- **Dual I2C Controllers**: Internal (system) and external (expansion) I2C interfaces
- **Grove Connector Support**: Port B and Port C with enhanced GPIO functionality
- **M-Bus Expansion**: 30-pin M-Bus connector support with ESP32-P4 GPIO mapping
- **High-Resolution Display**: 1280×720 display support with PPA acceleration readiness

#### Audio System
- **I2S Driver Migration**: Updated to ESP32-P4 compatible I2S drivers
- **PDM Microphone Support**: Dual microphone PDM interface integration
- **Enhanced Audio Processing**: ESP32-P4 specific audio optimizations

#### Peripheral Support
- **BMI270 IMU**: 6-axis motion sensor integration
- **Capacitive Touch**: Touch panel with interrupt-based detection
- **MicroSD Card**: SPI-based SD card support
- **Hardware Buttons**: Physical button integration

### 🔧 Fixed

#### Driver Compatibility
- **I2S Driver Issues**: Resolved ESP32-P4 I2S compatibility problems
- **Power Management**: Fixed EXT0 → EXT1 wakeup migration issues
- **GPIO Configuration**: Corrected ESP32-P4 specific pin assignments
- **Memory Allocation**: Fixed PSRAM allocation patterns for ESP32-P4

#### Build System
- **Compilation Errors**: Resolved ESP32-P4 specific build issues
- **Dependency Management**: Fixed library dependencies for ESP32-P4 platform
- **Platform Detection**: Improved ESP32-P4 platform identification

### 🚫 Removed

#### Multi-Platform Code
- **ESP32 Classic Support**: Removed ESP32, ESP32-S2, ESP32-S3, ESP32-C3 support
- **Legacy Hardware**: Removed support for non-Tab5 M5Stack devices
- **Deprecated Drivers**: Removed old I2S and power management code
- **Unused Code Paths**: Eliminated code branches not applicable to ESP32-P4

### ⚡ Performance

#### RISC-V Optimizations
- **Code Size Reduction**: 40% smaller library focusing only on ESP32-P4
- **Memory Efficiency**: Optimized memory usage patterns for 32MB PSRAM
- **Cache Optimization**: ESP32-P4 cache-friendly code organization
- **Compilation Speed**: Faster compilation with ESP32-P4 specific flags

### 📋 Technical Details

#### Platform Requirements
- **ESP32-P4**: RISC-V dual-core processor required
- **Arduino Core**: ESP32 Arduino Core >=3.0.0 with ESP32-P4 support
- **ESP-IDF**: ESP-IDF >=5.1 with ESP32-P4 RISC-V support
- **M5GFX**: M5GFX >=0.2.8 (ESP32-P4 compatible)

#### Build Flags
```ini
-DCONFIG_IDF_TARGET_ESP32P4=1
-DM5UNIFIED_ESP32P4_ONLY=1
-DBOARD_HAS_PSRAM=1
-DARDUINO_M5TAB5=1
```

#### GPIO Pin Mapping
- Internal I2C: SCL=32, SDA=31 (Touch, IMU, system components)
- External I2C: SCL=54, SDA=53 (Grove, M-Bus expansion)
- Port B: Pin1=17, Pin2=52 (Grove connector)
- Port C: Pin1=7, Pin2=6 (Grove connector)
- SD Card SPI: CLK=43, MOSI=44, MISO=39, CS=42
- Display SPI: CS=15, DC=16, RST=18, BL=19
- I2S Audio: BCLK=12, WS=13, DOUT=14
- USB OTG: DM=3, DP=4

### 🎯 Migration Notes

#### From Standard M5Unified
1. Update `platformio.ini` library dependency
2. Add ESP32-P4 specific build flags
3. Change board to `esp32-p4-function-ev-board`
4. Update GPIO pin references for Tab5 hardware
5. Test I2S audio code with new driver architecture

#### Known Limitations
- **Platform Exclusive**: Only works with ESP32-P4, not compatible with other ESP32 variants
- **Hardware Specific**: Optimized for M5Stack Tab5, other ESP32-P4 boards may need pin adjustments
- **Early Stage**: Some advanced features may be in development

### 🔮 Future Plans (0.4.x)
- Enhanced USB OTG host/device switching
- Camera controller integration
- Advanced PSRAM memory patterns
- Performance benchmarking suite
- Extended Grove module support
- Professional debugging tools

### 🙏 Acknowledgments
- **M5Stack Team**: Original M5Unified library foundation
- **lovyan03**: M5GFX graphics library integration
- **Espressif Systems**: ESP32-P4 RISC-V platform support
- **ESP32-P4 Community**: Early adopters and feedback providers

---

**Note**: This changelog focuses on changes specific to the ESP32-P4 fork. For the original M5Unified changelog, please refer to the main M5Unified repository.