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
        M5GFX
        esp_driver_gpio
        esp_driver_i2c
        esp_driver_ledc
        esp_driver_i2s
        esp_driver_pcnt
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

