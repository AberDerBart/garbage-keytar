cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

include (pico-sdk/pico_sdk_init.cmake)
project(keytar)

pico_sdk_init()

add_subdirectory("./Pico-PIO-USB" pico_pio_usb)

set(target_name keytar)
add_executable(${target_name})
target_sources(${target_name} PRIVATE
 main_device.c
 main_host.c
 usb_descriptors.c
 display.c
 pico-ssd1306/ssd1306.c
 settings.c
 keyboard.c
 midi.c
 # can use 'tinyusb_pico_pio_usb' library later when pico-sdk is updated
 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/dcd_pio_usb.c
 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c
 )

pico_btstack_make_gatt_header(${target_name} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/midi-ble.gatt")


 # print memory usage, enable all warnings
target_link_options(${target_name} PRIVATE -Xlinker --print-memory-usage)
target_compile_options(${target_name} PRIVATE ) #-Wall -Wextra

# use tinyusb implementation
target_compile_definitions(${target_name} PRIVATE PIO_USB_USE_TINYUSB)

# needed so tinyusb can find tusb_config.h
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${target_name} PRIVATE pico_stdlib pico_pio_usb tinyusb_device tinyusb_host hardware_i2c pico_btstack_ble pico_btstack_cyw43 pico_cyw43_arch_none)
pico_add_extra_outputs(${target_name})

add_subdirectory(pico_w)
