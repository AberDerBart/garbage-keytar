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

typedef struct ui_element_t {
  void (*render)(struct ui_element_t* self, ssd1306_t* display);
  void (*navigate)(struct ui_element_t* self, ui_nav_t nav);
  void (*free)(struct ui_element_t* self);
} ui_element_t;