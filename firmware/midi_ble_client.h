#pragma once

#include <stdbool.h>
#include <stdint.h>

void midi_ble_client_init();
void midi_ble_client_deinit();

bool midi_ble_client_is_initialized();

void midi_ble_client_write(uint8_t len, uint8_t *msg);

void midi_ble_client_scan_start();
void midi_ble_client_scan_stop();
void midi_ble_client_dump_devices();
