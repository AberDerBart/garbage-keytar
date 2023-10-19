#pragma once

#include <stdint.h>

void ble_midi_init();

void ble_midi_send_msg(uint8_t len, uint8_t* msg);
