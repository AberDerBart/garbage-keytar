#pragma once
#include "ui_element.h"

typedef struct menu_item_t {
  char* label;
  void (*action)(struct menu_item_t*);
} menu_item_t;

typedef struct ui_menu_t {
  ui_element_t base;
  uint8_t length;
  menu_item_t** items;
  uint8_t index;
  uint8_t scroll;
} ui_menu_t;

void push_menu(uint8_t length, menu_item_t** items);

ui_element_t* menu_element(ui_menu_t* menu);