#pragma once

#include <stdbool.h>
#include <stdint.h>

void midi_ble_init();

void midi_ble_server_init();
void midi_ble_deinit();

bool midi_ble_server_is_initialized();

void midi_ble_server_write(uint8_t len, uint8_t* msg);

typedef enum {
  OFF = 0,
  CLIENT = 1,
  SERVER = 2,
} ble_mode_t;

typedef struct {
  ble_mode_t mode;
  uint8_t connected_addr[6];
  int connected_addr_type;
} ble_state_t;

ble_state_t* midi_ble_get_state();
