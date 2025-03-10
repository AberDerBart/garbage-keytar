#include "keyboard_matrix.h"

#include <stddef.h>

#include "config.h"
#include "display.h"
#include "img/gen/accordion_b_32_16.h"
#include "img/gen/accordion_c_32_16.h"
#include "img/gen/guitar_32_16.h"
#include "img/gen/piano_32_16.h"
#include "img/gen/six_plus_six_32_16.h"
#include "img/gen/wicki_hayden_32_16.h"
#include "key_positions.h"

uint8_t note_by_lut(uint8_t keycode, uint8_t lut[4][14]) {
  if (keycode >= 128) {
    return 0;
  }
  int8_t row = keycode2row[keycode];
  int8_t col = keycode2column[keycode];

  if (row <= 0 || col < 0 || row > 4 || col >= 14) {
    return 0;
  }

  return lut[4 - row][col];
}

uint8_t note_by_pattern(uint8_t keycode, uint8_t offset, int8_t col_step,
                        int8_t row_step) {
  if (keycode >= 128) {
    return 0;
  }
  int8_t row = keycode2row[keycode];
  int8_t col = keycode2column[keycode];

  if (row < 0 || col < 0) {
    return 0;
  }

  return col * col_step + row * row_step + offset;
}

void set_keymap(keymap_t* keymap) {
  display_state_t* display_state = display_get_state_for_update();
  display_state->keymap = keymap;
}

uint8_t piano_lut[4][14] = {
    {0, 0, 49, 51, 0, 54, 56, 58, 61, 63, 0, 66, 68, 0},
    {0, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69},
    {0, 0, 37, 39, 0, 42, 44, 46, 0, 49, 51, 0, 0, 0},
    {0, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 0, 0, 0},
};

uint8_t lookup_piano(uint8_t keycode) {
  return note_by_lut(keycode, piano_lut);
}

keymap_t keymap_piano = {
  lookup : &lookup_piano,
  icon_size : piano_32_16_bmp_size,
  icon_data : piano_32_16_bmp_data
};

void set_keymap_piano() { set_keymap(&keymap_piano); }

uint8_t lookup_accordion_c(uint8_t keycode) {
  return note_by_pattern(keycode, 36, 3, -2);
}

keymap_t keymap_accordion_c = {
  lookup : &lookup_accordion_c,
  icon_size : accordion_c_32_16_bmp_size,
  icon_data : accordion_c_32_16_bmp_data
};

void set_keymap_accordion_c() { set_keymap(&keymap_accordion_c); }

uint8_t lookup_accordion_b(uint8_t keycode) {
  return note_by_pattern(keycode, 38, 3, -1);
}

keymap_t keymap_accordion_b = {
  lookup : &lookup_accordion_b,
  icon_size : accordion_b_32_16_bmp_size,
  icon_data : accordion_b_32_16_bmp_data
};

void set_keymap_accordion_b() { set_keymap(&keymap_accordion_b); }

uint8_t lookup_accordion_6plus6(uint8_t keycode) {
  return note_by_pattern(keycode, 32, 2, -1);
}

keymap_t keymap_accordion_6plus6 = {
  lookup : &lookup_accordion_6plus6,
  icon_size : six_plus_six_32_16_bmp_size,
  icon_data : six_plus_six_32_16_bmp_data
};

void set_keymap_accordion_6plus6() { set_keymap(&keymap_accordion_6plus6); }

uint8_t lookup_wicki_hayden(uint8_t keycode) {
  return note_by_pattern(keycode, 32, 2, 7);
}

keymap_t keymap_wicki_hayden = {
  lookup : &lookup_wicki_hayden,
  icon_size : wicki_hayden_32_16_bmp_size,
  icon_data : wicki_hayden_32_16_bmp_data
};

void set_keymap_wicki_hayden() { set_keymap(&keymap_wicki_hayden); }

uint8_t lookup_guitar(uint8_t keycode) {
  return note_by_pattern(keycode, 40, 1, 5);
}

keymap_t keymap_guitar = {
  lookup : &lookup_guitar,
  icon_size : guitar_32_16_bmp_size,
  icon_data : guitar_32_16_bmp_data,
};

void set_keymap_guitar() { set_keymap(&keymap_guitar); }

keymap_t* default_keymap = &keymap_piano;

#define N_KEYMAPS 6

keymap_t* keymaps[N_KEYMAPS] = {
    &keymap_piano,        &keymap_accordion_c,
    &keymap_accordion_b,  &keymap_accordion_6plus6,
    &keymap_wicki_hayden, &keymap_guitar,
};

uint8_t get_keymap_index() {
  keymap_t* current_keymap = display_get_state_for_update()->keymap;
  for (uint8_t i = 0; i < N_KEYMAPS; i++) {
    if (keymaps[i] == current_keymap) {
      return i;
    }
  }
  return 0;
}

void set_keymap_by_index(uint8_t index) {
  if (index >= N_KEYMAPS) {
    return;
  }

  set_keymap(keymaps[index]);
}