#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdbool.h>

#include "battery.h"
#include "keyboard_matrix.h"
#include "menu.h"

typedef struct {
  bool keyboard;
  bool bluetooth;
  bool charging;
  battery_level_t battery;
  keymap_t *keymap;
  menu_state_t menu_state;
} display_state_t;

display_state_t *display_get_state_for_update();

void display_init();

void display_task();

#endif
