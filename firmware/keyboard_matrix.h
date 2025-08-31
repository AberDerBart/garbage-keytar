#pragma once
#include <stddef.h>
#include <stdint.h>

#define N_KEYMAPS 6

typedef struct {
  uint8_t (*lookup)(uint8_t);
  unsigned long icon_size;
  unsigned char* icon_data;
  char* label;
} keymap_t;

extern keymap_t keymap_piano;
extern keymap_t keymap_accordion_b;
extern keymap_t keymap_accordion_c;
extern keymap_t keymap_accordion_6plus6;
extern keymap_t keymap_wicki_hayden;
extern keymap_t keymap_guitar;

extern keymap_t* keymaps[];

void set_keymap(keymap_t* keymap);

size_t get_keymap_index();

void set_keymap_by_index(size_t index);
keymap_t* get_keymap_by_index(size_t index);
size_t get_n_keymaps();

keymap_t* keymap_get();
