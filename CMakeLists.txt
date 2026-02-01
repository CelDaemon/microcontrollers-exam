cmake_minimum_required(VERSION 3.27)

set(PICO_BOARD pico2_w)

include(pico-sdk/pico_sdk_init.cmake)

project(exam)

set(FLASH_ENABLED OFF CACHE BOOL "Whether to enable the flash target") # Not usable for most people, flash manually instead
set(FLASH_DEVICE "" CACHE STRING "The device to mount for flashing")
set(FLASH_MOUNT_POINT "" CACHE STRING "The mount point to use for flashing")

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
    if(NOT FLASH_DEVICE OR NOT FLASH_MOUNT_POINT)
        message(FATAL_ERROR "Ensure FLASH_DEVICE and FLASH_MOUNT_POINT are set")
    endif()
    add_custom_target(flash USES_TERMINAL DEPENDS main
        COMMAND sudo mount "${FLASH_DEVICE}" "${FLASH_MOUNT_POINT}"
        COMMAND sudo cp "$<TARGET_FILE_DIR:main>/$<TARGET_FILE_BASE_NAME:main>.uf2" "${FLASH_MOUNT_POINT}"
        COMMAND sudo umount "${FLASH_MOUNT_POINT}"
    )
endif()

