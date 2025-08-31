# Contributing to M5Unified-ESP32P4

Thank you for your interest in contributing to the M5Unified-ESP32P4 library! This project focuses exclusively on ESP32-P4 RISC-V architecture and M5Stack Tab5 hardware compatibility.

## 🎯 Project Focus

This library is specifically designed for:
- **ESP32-P4 RISC-V Architecture**: Dual-core RISC-V processor support
- **M5Stack Tab5 Hardware**: Native support for Tab5's advanced capabilities
- **Performance Optimization**: RISC-V specific optimizations and 32MB PSRAM utilization
- **Developer Experience**: Professional Arduino and PlatformIO integration

## 🚀 How to Contribute

### 1. Setting Up Development Environment

#### Prerequisites
- **Hardware**: M5Stack Tab5 device for testing (highly recommended)
- **Software**: PlatformIO or Arduino IDE with ESP32-P4 support
- **Tools**: Git, ESP32 Arduino Core >=3.0.0

#### Development Setup
```bash
# Clone the repository
git clone https://github.com/m5stack/M5Unified-ESP32P4.git
cd M5Unified-ESP32P4

# Set up PlatformIO environment
pio lib install
pio run -e m5tab5

# Or use the existing platformio_esp32p4.ini
pio run -c platformio_esp32p4.ini
```

### 2. Areas We Need Help

#### High Priority
- **I2S Audio System**: ESP32-P4 I2S driver integration and optimization
- **USB OTG Support**: Host/device mode switching and device management
- **Camera Controller**: ESP32-P4 camera controller integration
- **Power Management**: Advanced power features and optimization

#### Medium Priority
- **Grove Module Support**: Extended Grove sensor and actuator modules
- **Performance Optimization**: RISC-V specific code optimizations
- **Memory Management**: Advanced PSRAM usage patterns
- **Documentation**: Code examples and hardware guides

#### Low Priority
- **Debugging Tools**: Professional debugging and diagnostic features
- **Testing Framework**: Automated testing on ESP32-P4 hardware
- **Build System**: Enhanced build configuration and optimization

### 3. Contribution Guidelines

#### Code Style
- **C++ Standard**: Use modern C++11/14 features where appropriate
- **Naming**: Follow existing M5Unified naming conventions
- **Comments**: Document ESP32-P4 specific implementations
- **Headers**: Include ESP32-P4 compatibility checks where needed

#### Example Code Style
```cpp
#if !defined(CONFIG_IDF_TARGET_ESP32P4)
#error "M5Unified-ESP32P4 requires ESP32-P4 target. Use standard M5Unified for other platforms."
#endif

namespace m5 {
    class ESP32P4_SpecificFeature {
    public:
        // ESP32-P4 RISC-V optimized implementation
        bool initializeForTab5();
        
    private:
        // Tab5 hardware specific configurations
        static constexpr gpio_num_t TAB5_SPECIFIC_PIN = GPIO_NUM_32;
    };
}
```

#### Hardware Compatibility
- **ESP32-P4 Exclusive**: All code must be ESP32-P4 specific
- **Tab5 Optimized**: Primary focus on M5Stack Tab5 pin mappings
- **Forward Compatible**: Consider future ESP32-P4 based M5Stack devices

#### Testing Requirements
- **Hardware Testing**: Test on actual M5Stack Tab5 hardware
- **Compilation**: Verify compilation with ESP32-P4 flags
- **Memory Usage**: Monitor PSRAM and heap utilization
- **Performance**: Benchmark critical code paths

### 4. Submission Process

#### Pull Request Workflow
1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/esp32p4-enhancement`)
3. **Develop** your changes with ESP32-P4 focus
4. **Test** on M5Stack Tab5 hardware
5. **Document** your changes
6. **Submit** a pull request

#### Pull Request Template
```markdown
## ESP32-P4 Enhancement: [Feature Name]

### Description
Brief description of the ESP32-P4 specific enhancement.

### Hardware Tested
- [ ] M5Stack Tab5
- [ ] Other ESP32-P4 board (specify)

### Changes Made
- [ ] Core library modifications
- [ ] GPIO/pin mapping updates
- [ ] Driver integration
- [ ] Documentation updates

### Performance Impact
- Memory usage: [increase/decrease/neutral]
- Compilation time: [increase/decrease/neutral]
- Runtime performance: [improvement/regression/neutral]

### ESP32-P4 Specific Notes
Any ESP32-P4 RISC-V or Tab5 hardware specific considerations.
```

### 5. Development Focus Areas

#### ESP32-P4 RISC-V Optimizations
```cpp
// Example: RISC-V specific optimization
#ifdef CONFIG_IDF_TARGET_ESP32P4
    // Use RISC-V specific instructions or optimizations
    __asm__ volatile ("" ::: "memory"); // Memory barrier
#endif
```

#### Tab5 Hardware Integration
```cpp
// Example: Tab5 specific pin configuration
namespace m5::tab5 {
    constexpr gpio_num_t INTERNAL_I2C_SCL = GPIO_NUM_32;
    constexpr gpio_num_t INTERNAL_I2C_SDA = GPIO_NUM_31;
    constexpr gpio_num_t EXTERNAL_I2C_SCL = GPIO_NUM_54;
    constexpr gpio_num_t EXTERNAL_I2C_SDA = GPIO_NUM_53;
}
```

#### Memory Optimization
```cpp
// Example: PSRAM optimization for ESP32-P4
void* allocatePSRAMBuffer(size_t size) {
#ifdef CONFIG_SPIRAM_USE_MALLOC
    void* buffer = ps_malloc(size);
    if (buffer) {
        // ESP32-P4 specific PSRAM initialization
    }
    return buffer;
#endif
}
```

### 6. Documentation Standards

#### Code Documentation
- **Header Comments**: Explain ESP32-P4 specific functionality
- **Inline Comments**: Document RISC-V specific code sections
- **API Documentation**: Include Tab5 hardware examples

#### Example Documentation
```cpp
/**
 * @brief Initialize I2S audio system for ESP32-P4
 * 
 * This function configures the I2S peripheral using ESP32-P4 specific drivers
 * optimized for M5Stack Tab5 hardware configuration.
 * 
 * @param sample_rate Audio sample rate (8000-48000 Hz)
 * @return true if initialization successful
 * @note ESP32-P4 uses different I2S driver architecture than ESP32 classic
 */
bool initializeESP32P4Audio(uint32_t sample_rate);
```

### 7. Issue Reporting

#### Bug Reports
When reporting bugs, please include:
- **Hardware**: M5Stack Tab5 or other ESP32-P4 board
- **Environment**: PlatformIO/Arduino IDE version
- **ESP32-P4 Core**: Arduino core version
- **Build Flags**: ESP32-P4 specific build configuration
- **Reproduction**: Steps to reproduce on ESP32-P4 hardware

#### Feature Requests
For new features, consider:
- **ESP32-P4 Relevance**: How does this enhance ESP32-P4 support?
- **Tab5 Integration**: Does this improve Tab5 hardware utilization?
- **Performance Impact**: What are the RISC-V performance implications?

### 8. Community Guidelines

#### Communication
- **Be Respectful**: Maintain professional and helpful communication
- **Be Specific**: Focus on ESP32-P4 and Tab5 specific issues
- **Be Constructive**: Provide actionable feedback and suggestions

#### ESP32-P4 Focus
- **Platform Specific**: Remember this is ESP32-P4 exclusive
- **Hardware Oriented**: Consider actual Tab5 hardware limitations
- **Future Focused**: Think about ESP32-P4 ecosystem growth

## 🏆 Recognition

Contributors who provide significant ESP32-P4 enhancements will be:
- **Acknowledged**: Listed in CHANGELOG.md and README.md
- **Credited**: Mentioned in release notes
- **Promoted**: Featured in ESP32-P4 community discussions

## 📞 Getting Help

### Development Support
- **GitHub Discussions**: General development questions
- **GitHub Issues**: Bug reports and feature requests
- **ESP32-P4 Tag**: Use `esp32-p4` tag for platform-specific issues

### Resources
- **ESP32-P4 Documentation**: [Espressif ESP32-P4 Guide](https://docs.espressif.com/)
- **M5Stack Tab5**: [M5Stack Official Documentation](https://docs.m5stack.com/)
- **RISC-V Resources**: [RISC-V International](https://riscv.org/)

---

**Thank you for contributing to the ESP32-P4 RISC-V ecosystem!**

Together, we're building the foundation for next-generation embedded development on RISC-V architecture.