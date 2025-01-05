#pragma once
#include <stdint.h>
typedef struct {
  uint8_t (*lookup)(uint8_t);
  char *name;
  unsigned long icon_size;
  unsigned char *icon_data;
} key_mapping_t;

extern const key_mapping_t KEY_MAPPINGS[5];

uint8_t piano(uint8_t keycode);

uint8_t accordion_c(uint8_t keycode);

uint8_t accordion_b(uint8_t keycode);

uint8_t accordion_6plus6(uint8_t keycode);

uint8_t wicki_hayden(uint8_t keycode);
