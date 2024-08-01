#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdbool.h>
#include "keyboard_matrix.h"

typedef enum {
  EMPTY = 0,
  LOW = 1,
  MEDIUM = 2,
  FULL = 3,
} battery_state_t;

typedef struct {
  bool keyboard;
  bool bluetooth;
  bool charging;
  battery_state_t battery;
  const key_mapping_t* key_mapping;
} display_state_t;

display_state_t* display_get_state_for_update();

void display_init();

void display_task();

#endif
