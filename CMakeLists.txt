cmake_minimum_required(VERSION 3.16)

# jettyd SDK — run `make setup` to clone it, or manually:
#   git clone https://github.com/jettydiot/jettyd-firmware.git jettyd-sdk

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/jettyd/CMakeLists.txt")
    message(FATAL_ERROR
        "\n\n  jettyd SDK not found. Run:\n"
        "    make setup\n"
        "  or:\n"
        "    git clone https://github.com/jettydiot/jettyd-firmware.git jettyd-sdk\n\n"
    )
endif()

set(EXTRA_COMPONENT_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/jettyd
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/dht22
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/ds18b20
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/bme280
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/relay
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/soil_moisture
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/pwm_output
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/hcsr04
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/ina219
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(jettyd-device)
