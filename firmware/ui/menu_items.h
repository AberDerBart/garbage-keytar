#pragma once
#include "./menu.h"

typedef struct ui_menu_item_default_t {
  ui_element_t base;
  char* label;
  void (*action)();
} ui_menu_item_default_t;

ui_pos_t ui_menu_item_default_render(ui_element_t* item, ssd1306_t* display,
                                     ui_pos_t pos);

ui_element_t* make_default_menu_item(char* label, void (*action)());