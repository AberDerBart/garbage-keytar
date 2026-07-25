#pragma once
#include <stdint.h>
#include <stdbool.h>

void midi_uart_init();

bool midi_uart_write(uint8_t len, uint8_t *msg);
