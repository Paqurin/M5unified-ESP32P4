# M5Unified-ESP32P4 - ESP32-P4 optimized library for M5Stack Tab5
set(COMPONENT_ADD_INCLUDEDIRS
    src
    )

# Only include source files relevant to ESP32-P4/Tab5 hardware
file(GLOB SRCS
     src/*.cpp
     src/utility/*.cpp
     src/utility/imu/*.cpp
     )
set(COMPONENT_SRCS ${SRCS})

# ESP32-P4 specific component requirements
if (IDF_VERSION_MAJOR GREATER_EQUAL 5)
    set(COMPONENT_REQUIRES M5GFX esp_adc driver esp_driver_gpio esp_driver_i2s esp_driver_pcnt esp_driver_ledc)
else()
    set(COMPONENT_REQUIRES M5GFX esp_adc_cal driver)
endif()

# Add ESP32-P4 specific compile definitions
target_compile_definitions(${COMPONENT_LIB} PUBLIC
    -DCONFIG_IDF_TARGET_ESP32P4=1
    -DM5UNIFIED_ESP32P4_ONLY=1
    -DBOARD_HAS_PSRAM=1
    -DARDUINO_M5TAB5=1
)

### Arduino-ESP32 components for ESP32-P4
list(APPEND COMPONENT_REQUIRES arduino-esp32)

message(STATUS "M5Unified-ESP32P4 use component = ${COMPONENT_REQUIRES}" )

register_component()

