#include "./select.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack.h"

typedef struct ui_menu_item_select_t {
  ui_element_t base;
  char* label;
  size_t index;
  size_t max;
  void** options;
  char* (*get_option_label)(void* option);
  void (*on_change)(void* option);
} ui_menu_item_select_t;

void ui_menu_item_select_free(ui_element_t* item) { free(item); }

void ui_menu_item_select_navigate(ui_element_t* item, ui_nav_t nav) {
  ui_menu_item_select_t* self = (ui_menu_item_select_t*)item;
  bool changed = false;
  switch (nav) {
    case LEFT:
      if (self->index > 0) {
        self->index--;
        changed = true;
      }
      break;
    case RIGHT:
      if (self->index < self->max) {
        self->index++;
        changed = true;
      }
      break;
    default:
      break;
  }

  if (changed) {
    if (self->on_change) {
      (self->on_change)(self->options[self->index]);
    }
    ui_render();
  }
}

ui_pos_t ui_menu_item_select_render(ui_element_t* item, ssd1306_t* display,
                                    ui_pos_t pos, bool focus) {
  ui_menu_item_select_t* self = (ui_menu_item_select_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);

  void* option = self->options[self->index];
  char* label = self->get_option_label(option);
  size_t charCount = strlen(label);

  ssd1306_draw_string(display, display->width - charCount * 6 - 8, pos.y, 1, label);

  if (focus) {
    if (self->index > 0) {
      ssd1306_draw_char(display, display->width - 18 - 6 * charCount, pos.y, 1,
                        '<');
    }
    if (self->index < self->max) {
      ssd1306_draw_char(display, display->width - 6, pos.y, 1, '>');
    }
  }

  ui_pos_t new_pos = {
    .x = display->width,
    .y = pos.y + 8,
  };
  return new_pos;
}

ui_element_t* ui_menu_item_select_new(char* label, void** options, size_t initial_index, char* (*get_option_label)(void* option),
                                      void (*on_change)(void* option)) {
  ui_menu_item_select_t* item = malloc(sizeof(ui_menu_item_select_t));
  item->base.free = ui_menu_item_select_free;
  item->base.render = ui_menu_item_select_render;
  item->base.navigate = ui_menu_item_select_navigate;
  item->label = label;
  item->index = initial_index;
  item->max = 0;
  item->options = options;
  item->get_option_label = get_option_label;
  while (options[item->max + 1]) {
    item->max++;
  }
  item->on_change = on_change;

  return (ui_element_t*)item;
}
