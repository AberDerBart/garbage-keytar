#pragma once
#include <stdint.h>
typedef struct {
  uint8_t (*lookup)(uint8_t);
  unsigned long icon_size;
  unsigned char* icon_data;
} keymap_t;

extern keymap_t* default_keymap;

void set_keymap_piano();

void set_keymap_accordion_b();

void set_keymap_accordion_c();

void set_keymap_accordion_6plus6();

void set_keymap_wicki_hayden();

void set_keymap_guitar();

uint8_t get_keymap_index();

void set_keymap_by_index(uint8_t index);
