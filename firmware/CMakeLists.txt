cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Make a GATT header file from a BTstack GATT file
# Pass the target library name library type and path to the GATT input file
# To add additional directories to the gatt #import path, one "-I[custom_path]"
# argument for each custom_path to the end of the argument list.
# TODO if pull request #1445 to the pico-sdk project is approved, then
# remove this function and use pico_btstack_make_gatt_header() instead
function(my_pico_btstack_make_gatt_header TARGET_LIB TARGET_TYPE GATT_FILE)
        find_package (Python3 REQUIRED COMPONENTS Interpreter)
        get_filename_component(GATT_NAME "${GATT_FILE}" NAME_WE)
        get_filename_component(GATT_PATH "${GATT_FILE}" PATH)
        set(GATT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
        set(GATT_HEADER "${GATT_BINARY_DIR}/${GATT_NAME}.h")
        set(TARGET_GATT "${TARGET_LIB}_gatt_header")

        add_custom_target(${TARGET_GATT} DEPENDS ${GATT_HEADER})
        add_custom_command(
                OUTPUT ${GATT_HEADER}
                DEPENDS ${GATT_FILE}
                WORKING_DIRECTORY ${GATT_PATH}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${GATT_BINARY_DIR} &&
                        ${Python3_EXECUTABLE} ${PICO_SDK_PATH}/lib/btstack/tool/compile_gatt.py ${GATT_FILE} ${GATT_HEADER} ${ARGN}
                VERBATIM)
        add_dependencies(${TARGET_LIB}
                ${TARGET_GATT}
                )
        target_include_directories(${TARGET_LIB} ${TARGET_TYPE}
                ${GATT_BINARY_DIR}
                )
endfunction()

set(PICO_BOARD pico_w)

include (pico-sdk/pico_sdk_init.cmake)
project(keytar)

pico_sdk_init()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/pico-w-ble-midi-lib")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/ring_buffer_lib")

set(target_name keytar)
add_executable(${target_name})
target_sources(${target_name} PRIVATE
 main.c
 display.c
 pico-ssd1306/ssd1306.c
 settings.c
 keyboard.c
 keyboard_matrix.c
 midi.c
 midi_ble.c
 midi_uart.c
 midi_ble_client.c
 menu.c
 menu_navigation.c
 menu/bluetooth.c
 )

my_pico_btstack_make_gatt_header(${target_name} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/midi-ble.gatt" "-I${CMAKE_CURRENT_LIST_DIR}/lib/pico-w-ble-midi-lib")


 # print memory usage, enable all warnings
target_link_options(${target_name} PRIVATE -Xlinker --print-memory-usage)
target_compile_options(${target_name} PRIVATE ) #-Wall -Wextra

# needed so tinyusb can find tusb_config.h
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(${target_name} PRIVATE 
  tinyusb_host
  pico_stdlib
  hardware_i2c
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  ble_midi_service_lib
  ring_buffer_lib
  ble_midi_client_lib
)

pico_enable_stdio_uart(${target_name} 1)


pico_add_extra_outputs(${target_name})

