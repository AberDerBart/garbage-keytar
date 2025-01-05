#pragma once

#include <stdint.h>

void midi_ble_client_init();

void midi_ble_write(uint8_t len, uint8_t *msg);