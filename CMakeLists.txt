cmake_minimum_required(VERSION 3.21)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi.cmake)
endif()

project(stm_course C ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Which example directory to build (each holds a main.c)
set(APP led_toggle_addr CACHE STRING "Application directory to build")

# ---- MCU settings (STM32F411, Cortex-M4F) ----
set(MCU_FLAGS -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/vendor/startup/STM32F411RETX_FLASH.ld)

# ---- Every directory under vendor/ that contains a header ----
file(GLOB_RECURSE VENDOR CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/*.h)
set(CHIP_INCLUDE_DIRS)
foreach(header ${VENDOR})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND CHIP_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES CHIP_INCLUDE_DIRS)

# ---- Firmware ----
file(GLOB APP_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${APP}/*.c)

add_executable(${APP}
    ${APP_SOURCES}
    vendor/startup/system_stm32f4xx.c
    vendor/startup/startup_stm32f411retx.s
)
set_target_properties(${APP} PROPERTIES SUFFIX .elf)

target_include_directories(${APP} PRIVATE ${CHIP_INCLUDE_DIRS} ${APP})
target_compile_definitions(${APP} PRIVATE STM32F411xE)

target_compile_options(${APP} PRIVATE
    ${MCU_FLAGS}
    -Wall -Wextra
    -ffunction-sections -fdata-sections
    "$<$<CONFIG:Debug>:-Og;-g3>"
    "$<$<CONFIG:Release>:-Os;-g>"
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
)

target_link_options(${APP} PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nano.specs --specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${APP}.map,--cref
    -Wl,--print-memory-usage
)
set_property(TARGET ${APP} APPEND PROPERTY LINK_DEPENDS ${LINKER_SCRIPT})

# Produce .bin/.hex next to the .elf
add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${APP}> ${APP}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${APP}> ${APP}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${APP}>
)
