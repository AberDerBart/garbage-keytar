#pragma once
#include "pico-ssd1306/ssd1306.h"

typedef enum ui_nav_t {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  ENTER,
  RETURN,
  CLOSE,
} ui_nav_t;

typedef struct ui_pos_t {
  uint8_t x;
  uint8_t y;
} ui_pos_t;

typedef struct ui_element_t {
  ui_pos_t (*render)(struct ui_element_t* self, ssd1306_t* display,
                     ui_pos_t pos, bool focus);
  void (*navigate)(struct ui_element_t* self, ui_nav_t nav);
  void (*free)(struct ui_element_t* self);
} ui_element_t;