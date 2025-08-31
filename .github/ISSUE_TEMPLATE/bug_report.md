---
name: Bug Report
about: Report a bug specific to ESP32-P4 or M5Stack Tab5
title: "[BUG] "
labels: bug, esp32-p4
assignees: ''

---

## Bug Description
A clear and concise description of the bug.

## Environment
**Hardware:**
- Device: [ ] M5Stack Tab5 [ ] Other ESP32-P4 board (specify: _______)
- ESP32-P4 Revision: [ ] ECO0 [ ] ECO1 [ ] Unknown

**Software:**
- M5Unified-ESP32P4 Version: [e.g., v0.3.0-esp32p4]
- Arduino Core Version: [e.g., 3.0.0]
- ESP-IDF Version: [if using ESP-IDF directly]
- IDE: [ ] PlatformIO [ ] Arduino IDE [ ] ESP-IDF

**Platform Configuration:**
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

## Reproduction Steps
1. Step 1...
2. Step 2...
3. Step 3...

## Expected Behavior
What should happen on ESP32-P4/Tab5.

## Actual Behavior
What actually happens.

## Code Sample
```cpp
#include <M5Unified.h>

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    // Your code that demonstrates the issue
}

void loop() {
    M5.update();
}
```

## Error Messages/Logs
```
Paste compilation errors, runtime errors, or debug output here
```

## Hardware-Specific Details
**PSRAM Usage:**
- Free PSRAM: [e.g., 31.5MB]
- PSRAM allocation pattern: [if relevant]

**GPIO Configuration:**
- Affected pins: [e.g., GPIO32, GPIO31 for I2C]
- External hardware connected: [if any]

**Power Management:**
- Power source: [ ] USB [ ] Battery [ ] External
- Deep sleep usage: [ ] Yes [ ] No

## Additional Context
- Does this work with standard M5Unified on other ESP32 variants?
- Is this related to ESP32-P4 RISC-V architecture?
- Any workarounds found?

## Checklist
- [ ] I have tested on actual M5Stack Tab5 hardware
- [ ] I have verified ESP32-P4 specific build flags are set
- [ ] I have checked existing issues for duplicates
- [ ] I have provided complete reproduction steps