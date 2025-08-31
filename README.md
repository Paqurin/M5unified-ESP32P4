# M5Unified-ESP32P4

**ESP32-P4 RISC-V compatible fork of M5Unified library for M5Stack Tab5 hardware**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32P4-orange)](https://platformio.org/)
[![Arduino](https://img.shields.io/badge/Arduino-ESP32P4-blue)](https://arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![GitHub Release](https://img.shields.io/github/release/m5stack/M5Unified-ESP32P4.svg)](https://github.com/m5stack/M5Unified-ESP32P4/releases)

## 🚀 Overview

This is a **specialized ESP32-P4 optimized version** of M5Unified, specifically designed for the **M5Stack Tab5** with its revolutionary ESP32-P4 RISC-V architecture. This fork addresses critical compatibility issues with ESP32-P4 and provides enhanced features that standard M5Unified cannot support.

### 🎯 Why ESP32-P4 Specific?

The ESP32-P4 represents a major architectural shift to RISC-V, requiring:
- **New I2S driver architecture** - ESP32-P4 uses completely different I2S drivers
- **Enhanced power management** - EXT0 → EXT1 wakeup migration and advanced power features
- **PSRAM optimization** - Native support for 32MB PSRAM configurations  
- **GPIO reconfiguration** - ESP32-P4 specific pin mappings and capabilities
- **Performance optimization** - RISC-V architecture specific optimizations

### 🌟 Key Features

- **🎯 ESP32-P4 RISC-V Exclusive**: Optimized specifically for ESP32-P4 architecture
- **🖥️ M5Stack Tab5 Hardware**: Native support for Tab5's advanced capabilities
- **🔧 Enhanced Compatibility**: Fixed power management, I2S, and GPIO issues
- **💾 32MB PSRAM**: Optimized memory management for large applications
- **⚡ Performance**: RISC-V specific optimizations and streamlined codebase
- **🛠️ Developer Ready**: Full PlatformIO and Arduino IDE support

## 📦 Installation

### PlatformIO (Recommended)

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

1. Download this repository as ZIP
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select ESP32-P4 board configuration
4. Include: `#include <M5Unified.h>`

## 🚀 Quick Start

```cpp
#include <M5Unified.h>

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println("M5Unified-ESP32P4");
    M5.Display.println("Tab5 Ready!");
    M5.Display.printf("PSRAM: %d KB\n", ESP.getPsramSize() / 1024);
}

void loop() {
    M5.update();
    // Your ESP32-P4 code here
    delay(100);
}
```

## 🏗️ Hardware Support

### Target Platform
- **CPU**: ESP32-P4 RISC-V dual-core @ 400MHz
- **Memory**: 32MB PSRAM, 16MB Flash
- **Display**: 1280×720 high-resolution with PPA acceleration
- **Audio**: Dual microphone PDM interface + speaker
- **Connectivity**: USB OTG, WiFi 6, Bluetooth 5.0
- **Sensors**: BMI270 6-axis IMU, touch panel
- **Expansion**: M-Bus connector, GPIO, I2C, SPI

### M5Stack Tab5 Pin Configuration

#### I2C Interfaces
```cpp
// Internal I2C (system components)
#define INTERNAL_I2C_SCL  32
#define INTERNAL_I2C_SDA  31

// External I2C (user expansion)
#define EXTERNAL_I2C_SCL  54
#define EXTERNAL_I2C_SDA  53
```

#### Port Configurations
```cpp
// Port B
#define PORT_B_PIN1       17
#define PORT_B_PIN2       52

// Port C  
#define PORT_C_PIN1       7
#define PORT_C_PIN2       6
```

#### SPI (SD Card)
```cpp
#define SD_CLK            43
#define SD_MOSI           44
#define SD_MISO           39
#define SD_CS             42
```

## 📚 Examples & Usage

### Core ESP32-P4 Features

```cpp
// Display with PPA acceleration support
M5.Display.setTextSize(3);
M5.Display.fillScreen(BLACK);
M5.Display.setTextColor(GREEN);
M5.Display.println("ESP32-P4 RISC-V");

// PSRAM utilization
void* large_buffer = ps_malloc(1024 * 1024);  // 1MB allocation
M5.Display.printf("Free PSRAM: %d KB\n", ESP.getFreePsram() / 1024);

// Enhanced I2C communication
Wire.begin(INTERNAL_I2C_SDA, INTERNAL_I2C_SCL);
Wire1.begin(EXTERNAL_I2C_SDA, EXTERNAL_I2C_SCL);

// USB OTG support
USB.begin();
if (USB.isConnected()) {
    M5.Display.println("USB Device Connected");
}
```

### Tab5 Hardware Integration

```cpp
// BMI270 IMU access
float accelX, accelY, accelZ;
M5.Imu.getAccelData(&accelX, &accelY, &accelZ);

// Touch panel events
if (M5.Touch.getCount()) {
    auto touch = M5.Touch.getDetail();
    M5.Display.printf("Touch: %d, %d\n", touch.x, touch.y);
}

// Microphone PDM interface
M5.Mic.begin();
if (M5.Mic.isEnabled()) {
    // Process audio data
}
```

## 🔧 ESP32-P4 Specific Features

### Advanced Power Management
```cpp
// ESP32-P4 enhanced wakeup (EXT1 instead of EXT0)
esp_sleep_enable_ext1_wakeup(BUTTON_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
esp_deep_sleep_start();

// PSRAM power optimization
esp_pm_config_esp32p4_t pm_config = {
    .max_freq_mhz = 400,
    .min_freq_mhz = 40,
    .light_sleep_enable = true
};
esp_pm_configure(&pm_config);
```

### I2S Audio System (ESP32-P4 Compatible)
```cpp
// New I2S driver configuration for ESP32-P4
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(44100),
    .slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_12,
        .ws = GPIO_NUM_13,
        .dout = GPIO_NUM_15,
        .din = GPIO_NUM_34,
    },
};
```

## ⚡ Performance & Optimization

### Memory Management
- **32MB PSRAM**: Optimized allocation patterns for large datasets
- **Heap Management**: RISC-V specific memory layout optimization
- **Cache Efficiency**: ESP32-P4 cache-friendly code organization

### Compilation Optimizations
```ini
build_flags =
    -DCONFIG_IDF_TARGET_ESP32P4=1
    -DM5UNIFIED_ESP32P4_ONLY=1
    -DBOARD_HAS_PSRAM=1
    -DARDUINO_M5TAB5=1
    -Os                          ; Size optimization
    -ffunction-sections          ; Function-level linking
    -fdata-sections              ; Data-level linking
    -Wl,--gc-sections           ; Dead code elimination
```

## 🔌 Compatibility

### Supported Frameworks
- **Arduino IDE**: ESP32-P4 board support
- **ESP-IDF**: v5.1+ with ESP32-P4 support
- **PlatformIO**: Platform Espressif32 with ESP32-P4

### Supported Devices
- **Primary**: M5Stack Tab5 (ESP32-P4)
- **Future**: ESP32-P4 based M5Stack devices

### Dependencies
- **M5GFX**: >=0.2.8 (ESP32-P4 compatible)
- **ESP32 Arduino Core**: >=3.0.0 (ESP32-P4 support)
- **ESP-IDF**: >=5.1 (ESP32-P4 RISC-V support)

## 🔗 M5Stack Tab5 Ecosystem

### Supported Tab5 Accessories
- **M-Bus Modules**: Full M-Bus connector support with ESP32-P4 GPIO mapping
- **External I2C Devices**: Dual I2C controller support
- **USB OTG Devices**: Native USB host/device mode
- **Grove Connectors**: Port B and Port C with enhanced GPIO
- **Audio Accessories**: PDM microphones, I2S speakers
- **Camera Modules**: ESP32-P4 camera controller support

### Future Ecosystem
- **Tab5 HAT**: Specialized expansion boards
- **Professional Modules**: Industrial and IoT applications
- **Developer Accessories**: Debug and prototyping tools

## 🚀 Why Choose M5Unified-ESP32P4?

### vs Standard M5Unified

| Feature | Standard M5Unified | M5Unified-ESP32P4 |
|---------|-------------------|-------------------|
| **ESP32-P4 Support** | ❌ Limited/Broken | ✅ Native & Optimized |
| **I2S Audio** | ❌ Old drivers | ✅ New ESP32-P4 drivers |
| **Power Management** | ❌ EXT0 (deprecated) | ✅ EXT1 (ESP32-P4) |
| **32MB PSRAM** | ⚠️ Generic | ✅ Optimized |
| **Code Size** | 🔶 Multi-platform | ✅ ESP32-P4 focused |
| **Performance** | 🔶 Generic | ✅ RISC-V optimized |
| **Tab5 Hardware** | ❌ No support | ✅ Full support |

### Migration from Standard M5Unified

1. **Update platformio.ini**:
   ```ini
   lib_deps = 
       ; Remove: m5stack/M5Unified
       ; Add:
       https://github.com/m5stack/M5Unified-ESP32P4.git
   ```

2. **Add ESP32-P4 flags**:
   ```ini
   build_flags = 
       -DCONFIG_IDF_TARGET_ESP32P4=1
       -DM5UNIFIED_ESP32P4_ONLY=1
   ```

3. **Update board configuration**:
   ```ini
   board = esp32-p4-function-ev-board
   ```

## 🛠️ Development

### Building from Source

```bash
git clone https://github.com/m5stack/M5Unified-ESP32P4.git
cd M5Unified-ESP32P4
pio lib install
pio run -e m5tab5
```

### Contributing

We welcome contributions that enhance ESP32-P4 compatibility:

1. **Focus Areas**:
   - ESP32-P4 RISC-V optimizations
   - Tab5 hardware integration
   - I2S audio improvements
   - Power management enhancements

2. **Development Guidelines**:
   - Test on actual M5Stack Tab5 hardware
   - Consider 32MB PSRAM memory patterns
   - Follow ESP32-P4 driver conventions
   - Document Tab5-specific features

3. **Contribution Process**:
   - Fork the repository
   - Create feature branch
   - Test on ESP32-P4 hardware
   - Submit pull request

## 📋 Roadmap

### Version 0.3.x (Current)
- ✅ Basic ESP32-P4 compatibility
- ✅ Tab5 hardware support
- ✅ I2S driver updates
- ✅ Power management fixes

### Version 0.4.x (Upcoming)
- 🔄 Enhanced USB OTG support
- 🔄 Advanced PSRAM optimizations
- 🔄 Camera controller integration
- 🔄 Performance benchmarking

### Version 1.0.x (Future)
- 📅 Full ecosystem integration
- 📅 Professional debugging tools
- 📅 Industrial applications
- 📅 Community contributions

## ❓ FAQ

**Q: Why not use standard M5Unified?**  
A: ESP32-P4's RISC-V architecture requires specific drivers and optimizations that standard M5Unified cannot provide without breaking compatibility with other platforms.

**Q: Will this work with other ESP32 devices?**  
A: No, this library is ESP32-P4 exclusive. Use standard M5Unified for other ESP32 variants.

**Q: What about ESP32-P4 boards other than Tab5?**  
A: The library will work with other ESP32-P4 boards, but pin definitions and hardware features are optimized for Tab5.

**Q: How do I report ESP32-P4 specific issues?**  
A: Use the GitHub issues with the `esp32-p4` tag and include your PlatformIO configuration.

## 📖 Documentation

- **[ESP32-P4 Migration Guide](ESP32P4_MIGRATION_GUIDE.md)**: Detailed migration instructions
- **[API Reference](docs/API.md)**: Complete API documentation
- **[Hardware Guide](docs/Hardware.md)**: Tab5 pin mappings and specifications
- **[Examples](examples/)**: ESP32-P4 specific example code

## 📊 ESP32-P4 Tab5 Hardware Reference

### M5Stack Tab5 (ESP32-P4) GPIO Configuration

| GPIO | Function | Description | Alternative | Notes |
|:----:|:--------:|:-----------:|:-----------:|:-----:|
| GPIO 0 | Boot Mode | Boot configuration | Strapping pin | Pull-up required |
| GPIO 1 | UART0_TXD | Debug/Console TX | Serial output | Default console |
| GPIO 2 | UART0_RXD | Debug/Console RX | Serial input | Default console |
| GPIO 3 | USB_DM | USB Data - | USB OTG | Host/Device mode |
| GPIO 4 | USB_DP | USB Data + | USB OTG | Host/Device mode |
| GPIO 5 | GPIO | General Purpose | M-Bus expansion | User configurable |
| GPIO 6 | PORT_C_1 | Port C Pin 1 | Grove connector | I2C/GPIO/ADC |
| GPIO 7 | PORT_C_2 | Port C Pin 2 | Grove connector | I2C/GPIO/ADC |
| GPIO 12 | I2S_BCLK | Audio Bit Clock | Speaker/Mic | I2S audio system |
| GPIO 13 | I2S_WS | Audio Word Select | Speaker/Mic | I2S audio system |
| GPIO 14 | I2S_DOUT | Audio Data Out | Speaker | I2S audio output |
| GPIO 15 | LCD_CS | Display Chip Select | Internal display | SPI interface |
| GPIO 16 | LCD_DC | Display Data/Command | Internal display | SPI interface |
| GPIO 17 | PORT_B_1 | Port B Pin 1 | Grove connector | I2C/GPIO/ADC |
| GPIO 18 | LCD_RST | Display Reset | Internal display | Reset control |
| GPIO 19 | LCD_BL | Display Backlight | Internal display | PWM brightness |
| GPIO 31 | I2C0_SDA | Internal I2C Data | System components | Touch, IMU, etc. |
| GPIO 32 | I2C0_SCL | Internal I2C Clock | System components | Touch, IMU, etc. |
| GPIO 39 | SD_MISO | SD Card Data In | MicroSD slot | SPI interface |
| GPIO 42 | SD_CS | SD Card Chip Select | MicroSD slot | SPI interface |
| GPIO 43 | SD_CLK | SD Card Clock | MicroSD slot | SPI interface |
| GPIO 44 | SD_MOSI | SD Card Data Out | MicroSD slot | SPI interface |
| GPIO 52 | PORT_B_2 | Port B Pin 2 | Grove connector | I2C/GPIO/ADC |
| GPIO 53 | I2C1_SDA | External I2C Data | User expansion | Grove/M-Bus |
| GPIO 54 | I2C1_SCL | External I2C Clock | User expansion | Grove/M-Bus |
| GPIO 55 | Touch_INT | Touch Panel Interrupt | Capacitive touch | Touch detection |
| GPIO 56 | IMU_INT | BMI270 Interrupt | Motion detection | Accelerometer |
| GPIO 57 | Button_A | Physical Button A | User input | Hardware button |
| GPIO 58 | Power_EN | System Power Control | Power management | Device power |
| GPIO 59 | BAT_ADC | Battery Voltage | Power monitoring | Analog read |

### M-Bus Connector (30-pin)

The M-Bus expansion connector provides comprehensive GPIO access:

| Pin | Signal | Function | Notes |
|:---:|:------:|:--------:|:-----:|
| 1-2 | GND | Ground | Power reference |
| 3-4 | 5V/3.3V | Power | Selectable voltage |
| 5-30 | GPIO | Various functions | See above table |

## 📄 License

MIT License - same as original M5Unified

## 🙏 Acknowledgments

- **M5Stack Team**: Original M5Unified library
- **lovyan03**: M5GFX graphics library
- **Espressif**: ESP32-P4 RISC-V platform
- **Community**: ESP32-P4 early adopters and testers

---

**Made with ❤️ for the ESP32-P4 RISC-V community**

For more information about M5Stack Tab5: [https://m5stack.com/](https://m5stack.com/)

For ESP32-P4 development: [Espressif ESP32-P4](https://www.espressif.com/en/products/socs/esp32-p4)

---

*This ESP32-P4 specific documentation focuses on M5Stack Tab5 hardware. For other M5Stack devices, please use the standard M5Unified library.*