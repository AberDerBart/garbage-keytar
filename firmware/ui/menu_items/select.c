#include "./select.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack.h"

typedef struct ui_menu_item_select_t {
  ui_element_t base;
  char* label;
  size_t value;
  size_t max;
  char** options;
  void (*on_change)(size_t);
} ui_menu_item_select_t;

void ui_menu_item_select_free(ui_element_t* item) { free(item); }

void ui_menu_item_select_navigate(ui_element_t* item, ui_nav_t nav) {
  ui_menu_item_select_t* self = (ui_menu_item_select_t*)item;
  bool changed = false;
  switch (nav) {
    case LEFT:
      if (self->value > 0) {
        self->value--;
        changed = true;
      }
      break;
    case RIGHT:
      if (self->value < self->max) {
        self->value++;
        changed = true;
      }
      break;
    default:
      break;
  }

  if (changed) {
    if (self->on_change) {
      (self->on_change)(self->value);
    }
    ui_render();
  }
}

ui_pos_t ui_menu_item_select_render(ui_element_t* item, ssd1306_t* display,
                                    ui_pos_t pos, bool focus) {
  ui_menu_item_select_t* self = (ui_menu_item_select_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);

  size_t charCount = strlen(self->options[self->value]);

  ssd1306_draw_string(display, display->width - charCount * 6 - 8, pos.y, 1,
                      self->options[self->value]);

  if (focus) {
    if (self->value > 0) {
      ssd1306_draw_char(display, display->width - 18 - 6 * charCount, pos.y, 1,
                        '<');
    }
    if (self->value < self->max) {
      ssd1306_draw_char(display, display->width - 6, pos.y, 1, '>');
    }
  }

  ui_pos_t new_pos = {
    x : display->width,
    y : pos.y + 8,
  };
  return new_pos;
}

ui_element_t* ui_menu_item_select_new(char* label, size_t value, char** options,
                                      void (*on_change)(size_t)) {
  ui_menu_item_select_t* item = malloc(sizeof(ui_menu_item_select_t));
  item->base.free = ui_menu_item_select_free;
  item->base.render = ui_menu_item_select_render;
  item->base.navigate = ui_menu_item_select_navigate;
  item->label = label;
  item->value = value;
  item->max = 0;
  item->options = options;
  while (options[item->max + 1]) {
    item->max++;
  }
  item->on_change = on_change;

  return (ui_element_t*)item;
}