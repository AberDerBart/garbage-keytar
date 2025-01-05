#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct menu_item_t {
  char *label;
  void (*action)();
  struct menu_item_t *parent;
  struct menu_item_t *children[];
} menu_item_t;

typedef struct {
  menu_item_t *current;
  uint8_t index;
} menu_state_t;

extern menu_item_t menu_item_main;