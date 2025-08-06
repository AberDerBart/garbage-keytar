#include "./menu_items.h"

#include <stdio.h>
#include <stdlib.h>

#include "ui_stack.h"

void navigate_default_menu_item(ui_element_t* item, ui_nav_t nav) {
  if (nav != ENTER) {
    return;
  }
  ui_menu_item_default_t* self = (ui_menu_item_default_t*)item;
  self->action();
}

ui_pos_t ui_menu_item_default_render(ui_element_t* item, ssd1306_t* display,
                                     ui_pos_t pos, bool focus) {
  ui_menu_item_default_t* self = (ui_menu_item_default_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);
  ui_pos_t new_pos = {
    x : display->width,
    y : pos.y + 8,
  };
  return new_pos;
}

void ui_menu_item_free(ui_element_t* item) { free(item); }

ui_element_t* make_default_menu_item(char* label, void (*action)()) {
  ui_menu_item_default_t* item = malloc(sizeof(ui_menu_t));
  item->base.free = ui_menu_item_free;
  item->base.render = ui_menu_item_default_render;
  item->base.navigate = navigate_default_menu_item;
  item->label = label;
  item->action = action;

  return (ui_element_t*)item;
}

typedef struct ui_menu_item_uint8_t {
  ui_element_t base;
  char* label;
  uint8_t* value;
  uint8_t min;
  uint8_t max;
  void (*on_change)();
} ui_menu_item_uint8_t;

void ui_menu_item_uint8_navigate(ui_element_t* item, ui_nav_t nav) {
  ui_menu_item_uint8_t* self = (ui_menu_item_uint8_t*)item;
  bool changed = false;
  switch (nav) {
    case LEFT:
      if (*(self->value) > self->min) {
        (*(self->value))--;
        changed = true;
      }
      break;
    case RIGHT:
      if (*(self->value) < self->max) {
        (*(self->value))++;
        changed = true;
      }
      break;
    default:
      break;
  }

  if (changed) {
    if (self->on_change) {
      (self->on_change)();
    }
    ui_render();
  }
}

ui_pos_t ui_menu_item_uint8_render(ui_element_t* item, ssd1306_t* display,
                                   ui_pos_t pos, bool focus) {
  ui_menu_item_uint8_t* self = (ui_menu_item_uint8_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);

  char buf[4];
  int charCount = sprintf(buf, "%d", *self->value);
  ssd1306_draw_string(display, display->width - charCount * 6 - 8, pos.y, 1,
                      buf);

  if (focus) {
    ssd1306_draw_char(display, display->width - 6, pos.y, 1, '>');
    ssd1306_draw_char(display, display->width - 18 - 6 * charCount, pos.y, 1,
                      '<');
  }

  ui_pos_t new_pos = {
    x : display->width,
    y : pos.y + 8,
  };
  return new_pos;
}

ui_element_t* ui_menu_item_uint8_new(char* label, uint8_t* value, uint8_t min,
                                     uint8_t max, void (*on_change)()) {
  ui_menu_item_uint8_t* item = malloc(sizeof(ui_menu_item_uint8_t));
  item->base.free = ui_menu_item_free;
  item->base.render = ui_menu_item_uint8_render;
  item->base.navigate = ui_menu_item_uint8_navigate;
  item->label = label;
  item->value = value;
  item->min = min;
  item->max = max;
  item->on_change = on_change;

  return (ui_element_t*)item;
}