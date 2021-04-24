cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(host_hid host_hid.c midi.c pitchbend.c control.c)

target_include_directories(host_hid PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(host_hid pico_stdlib tinyusb_host tinyusb_board hardware_adc)
pico_add_extra_outputs(host_hid)
