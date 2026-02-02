cmake_minimum_required(VERSION 3.27)

set(PICO_BOARD pico_w)

include(pico-sdk/pico_sdk_init.cmake)

project(exam)

set(FLASH_ENABLED OFF CACHE BOOL "Whether to enable the flash target") # Requires system installed picotool, flash manually if unavailable
set(FLASH_DEVICE "" CACHE STRING "The device to flash to")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

pico_sdk_init()

add_executable(main main.c)
target_link_libraries(main
    pico_stdlib
    #hardware_adc
)
pico_add_extra_outputs(main)
pico_enable_stdio_uart(main 1)
pico_enable_stdio_usb(main 1)


if(FLASH_ENABLED)
    find_program(PICOTOOL_EXECUTABLE NAMES picotool DOC "Picotool")
    if(NOT PICOTOOL_EXECUTABLE)
        message(FATAL_ERROR "Could not find picotool")
    endif()
    message("Using picotool: ${PICOTOOL_EXECUTABLE}")
    add_custom_target(flash USES_TERMINAL DEPENDS main
        COMMAND sudo ${PICOTOOL_EXECUTABLE} load -f -x $<TARGET_FILE:main>
    )
endif()

