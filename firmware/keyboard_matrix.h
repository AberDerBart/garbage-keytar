#pragma once
#include <stdint.h>
typedef struct {
  uint8_t (*lookup)(uint8_t);
  unsigned long icon_size;
  unsigned char* icon_data;
} keymap_t;

extern keymap_t keymap_piano;
extern keymap_t keymap_accordion_b;
extern keymap_t keymap_accordion_c;
extern keymap_t keymap_accordion_6plus6;
extern keymap_t keymap_wicki_hayden;
extern keymap_t keymap_guitar;

void set_keymap_piano();

void set_keymap_accordion_b();

void set_keymap_accordion_c();

void set_keymap_accordion_6plus6();

void set_keymap_wicki_hayden();

void set_keymap(keymap_t* keymap);

uint8_t get_keymap_index();

void set_keymap_by_index(uint8_t index);

keymap_t* keymap_get();
