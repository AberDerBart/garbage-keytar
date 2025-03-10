#pragma once

#include <stdbool.h>
#include <stdint.h>

void midi_ble_client_init_with_addr(int addr_type, uint8_t* addr);

void midi_ble_client_init();
void midi_ble_client_deinit();

bool midi_ble_client_is_initialized();

void midi_ble_client_write(uint8_t len, uint8_t* msg);

uint8_t midi_ble_client_device_count();
char* midi_ble_client_get_device_name(uint8_t index);

bool midi_ble_client_is_connected();
void midi_ble_client_disconnect();

extern void (*midi_ble_client_connect[16])();

void midi_ble_get_last_connected(int* addr_type_target, uint8_t* addr_target);
