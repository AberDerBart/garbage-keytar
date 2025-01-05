#pragma once

#include <stdbool.h>
#include <stdint.h>

void midi_ble_init();
void midi_ble_deinit();

bool midi_ble_server_is_initialized();

void midi_ble_server_write(uint8_t len, uint8_t *msg);
