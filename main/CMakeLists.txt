# Include the driver REQUIRES list emitted by build.py (from device.yaml).
# Falls back to empty list if not yet generated (first clean build regenerates it).
set(JETTYD_DRIVER_REQUIRES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/driver_requires.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/driver_requires.cmake")
endif()

idf_component_register(
    SRCS
        "main.c"
        "driver_registry.c"
    INCLUDE_DIRS "."
    REQUIRES jettyd ${JETTYD_DRIVER_REQUIRES}
)

# Auto-generate driver_registry.c from device.yaml before the build
# Uses the ESP-IDF Python environment (has PyYAML pre-installed)
set(DEVICE_YAML   "${CMAKE_CURRENT_SOURCE_DIR}/../device.yaml")
set(DRIVER_REG_C  "${CMAKE_CURRENT_SOURCE_DIR}/driver_registry.c")
set(BUILD_PY      "${CMAKE_CURRENT_SOURCE_DIR}/../build.py")

if(EXISTS "${DEVICE_YAML}" AND EXISTS "${BUILD_PY}")
    find_program(IDF_PYTHON python3
        HINTS ENV IDF_PYTHON_ENV_PATH
        NO_DEFAULT_PATH
    )
    if(NOT IDF_PYTHON)
        set(IDF_PYTHON python3)
    endif()
    execute_process(
        COMMAND ${IDF_PYTHON} ${BUILD_PY} ${DEVICE_YAML} ${DRIVER_REG_C}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
        RESULT_VARIABLE CODEGEN_RESULT
        OUTPUT_VARIABLE CODEGEN_OUTPUT
        ERROR_VARIABLE  CODEGEN_ERROR
    )
    if(NOT CODEGEN_RESULT EQUAL 0)
        message(WARNING "[build.py] Codegen failed: ${CODEGEN_ERROR}\nUsing existing driver_registry.c")
    else()
        message(STATUS "${CODEGEN_OUTPUT}")
    endif()
endif()
