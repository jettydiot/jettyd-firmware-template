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
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/led
    ${CMAKE_CURRENT_SOURCE_DIR}/jettyd-sdk/drivers/button
)

# ── Resolve IDF target: explicit IDF_TARGET overrides device.yaml default ────
# Precedence (highest first):
#   1. IDF_TARGET env var / -DIDF_TARGET=... on the command line (explicit override)
#   2. device.yaml `target:` field (the per-device DEFAULT)
#   3. esp32s3 (hard fallback)
# CI matrix legs set IDF_TARGET per leg so one device.yaml builds for every target.
# Must happen before include(project.cmake) so ESP-IDF picks up the right target.
set(DEVICE_YAML "${CMAKE_CURRENT_SOURCE_DIR}/device.yaml")

# Read the device.yaml default (used only when no explicit override is present).
set(DEVICE_TARGET "")
if(EXISTS "${DEVICE_YAML}" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build.py")
    find_program(_JETTYD_PYTHON python3
        HINTS ENV IDF_PYTHON_ENV_PATH
        NO_DEFAULT_PATH
    )
    if(NOT _JETTYD_PYTHON)
        set(_JETTYD_PYTHON python3)
    endif()
    execute_process(
        COMMAND ${_JETTYD_PYTHON} -c
            "import sys; sys.path.insert(0,'${CMAKE_CURRENT_SOURCE_DIR}'); from build import load_yaml; cfg=load_yaml('${DEVICE_YAML}'); print(cfg.get('target','esp32s3'))"
        OUTPUT_VARIABLE DEVICE_TARGET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()
if(NOT DEVICE_TARGET OR DEVICE_TARGET STREQUAL "")
    set(DEVICE_TARGET "esp32s3")
endif()

# Explicit override wins: env var first, then a -DIDF_TARGET cache value.
set(_IDF_TARGET_OVERRIDE "")
if(DEFINED ENV{IDF_TARGET} AND NOT "$ENV{IDF_TARGET}" STREQUAL "")
    set(_IDF_TARGET_OVERRIDE "$ENV{IDF_TARGET}")
elseif(IDF_TARGET AND NOT "${IDF_TARGET}" STREQUAL "")
    set(_IDF_TARGET_OVERRIDE "${IDF_TARGET}")
endif()

if(NOT "${_IDF_TARGET_OVERRIDE}" STREQUAL "")
    set(RESOLVED_TARGET "${_IDF_TARGET_OVERRIDE}")
    message(STATUS "[target] explicit IDF_TARGET override = ${RESOLVED_TARGET} (device.yaml default '${DEVICE_TARGET}' overridden)")
else()
    set(RESOLVED_TARGET "${DEVICE_TARGET}")
    message(STATUS "[target] using device.yaml default = ${RESOLVED_TARGET}")
endif()
set(IDF_TARGET "${RESOLVED_TARGET}" CACHE STRING "IDF target (device.yaml default, IDF_TARGET env overrides)" FORCE)

# ── Invalidate sdkconfig when the resolved target changes ─────────────────────
# sdkconfig is regenerated from scratch on target change; keep it in sync.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig" _SDKCONFIG_RAW)
    string(REGEX MATCH "CONFIG_IDF_TARGET=\"([^\"]*)\"" _TARGET_MATCH "${_SDKCONFIG_RAW}")
    set(_SDKCONFIG_TARGET "${CMAKE_MATCH_1}")
    if(NOT "${_SDKCONFIG_TARGET}" STREQUAL "${RESOLVED_TARGET}")
        message(STATUS "[target] target changed (${_SDKCONFIG_TARGET} → ${RESOLVED_TARGET}) — deleting stale sdkconfig")
        file(REMOVE "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig")
    endif()
endif()

# ── Auto-delete sdkconfig if sdkconfig.defaults is newer ─────────────────────
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig.defaults")
    file(TIMESTAMP "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig" TS_CONFIG)
    file(TIMESTAMP "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig.defaults" TS_DEFAULTS)
    if(TS_DEFAULTS IS_NEWER_THAN TS_CONFIG)
        message(STATUS "sdkconfig.defaults is newer — deleting stale sdkconfig")
        file(REMOVE "${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig")
    endif()
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(jettyd-device)
