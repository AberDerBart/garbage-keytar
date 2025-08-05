#pragma once
#include "ui_element.h"

typedef struct ui_menu_t {
  ui_element_t base;
  uint8_t length;
  ui_element_t** items;
  uint8_t index;
  uint8_t scroll;
} ui_menu_t;

ui_menu_t* push_menu(uint8_t item_capacity);

ui_element_t* menu_element(ui_menu_t* menu);

void menu_add(ui_menu_t* menu, ui_element_t* element);