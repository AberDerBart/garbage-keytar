#pragma once
#include "ui_element.h"

typedef struct ui_stack_layer_t {
  ui_element_t* element;
  struct ui_stack_layer_t* up;
} ui_stack_layer_t;

typedef struct ui_stack_t {
  ui_stack_layer_t* current;
  uint8_t stack_size;
  ui_element_t* default_element;
} ui_stack_t;

extern ui_stack_t ui_stack;

void ui_set_default_element(ui_element_t* element);

void ui_push(ui_element_t* element);
void ui_pop();
void ui_clear();

void ui_render(ssd1306_t* display);
void ui_navigate(ui_nav_t nav);
