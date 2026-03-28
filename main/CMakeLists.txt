# ── Step 1: Run build.py to generate driver_registry.c + driver_requires.cmake
# Must run BEFORE idf_component_register so REQUIRES reflects active drivers.
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

# ── Step 2: Load the driver REQUIRES list that build.py just wrote.
# On the very first clean build this file is freshly generated above,
# so JETTYD_DRIVER_REQUIRES will always reflect the current device.yaml.
set(JETTYD_DRIVER_REQUIRES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/driver_requires.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/driver_requires.cmake")
endif()

# ── Step 3: Register the main component with the correct driver deps.
idf_component_register(
    SRCS
        "main.c"
        "driver_registry.c"
    INCLUDE_DIRS "."
    REQUIRES jettyd ${JETTYD_DRIVER_REQUIRES}
)
