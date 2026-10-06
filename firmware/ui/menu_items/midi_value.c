#include <stdio.h>
#include <stdlib.h>

#include "midi_value.h"
#include "ui/stack.h"

typedef struct ui_menu_item_midi_value_t {
  ui_element_t base;
  midi_value* mv;
} ui_menu_item_midi_value_t;

void mi_free(ui_element_t* e) {
  free(e);
}

void ui_menu_item_midi_value_navigate(ui_element_t* item, ui_nav_t nav) {
  ui_menu_item_midi_value_t* self = (ui_menu_item_midi_value_t*)item;
  bool changed = false;
  switch (nav) {
    case LEFT:
      if (self->mv->value == 0) {
        break;
      }
      midi_value_set(self->mv, self->mv->value-1, true);
      changed = true;
      break;
    case RIGHT:
      if (self->mv->value == UINT16_MAX) {
        break;
      }
      midi_value_set(self->mv, self->mv->value+1, true);
      changed = true;
      break;
    default:
      break;
  }

  if (changed) {
    ui_render();
  }
}

ui_pos_t ui_menu_item_midi_value_render(ui_element_t* item, ssd1306_t* display,
                                   ui_pos_t pos, bool focus) {
  ui_menu_item_midi_value_t* self = (ui_menu_item_midi_value_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->mv->label);

  char buf[6];
  int charCount = sprintf(buf, "%d", self->mv->value);
  ssd1306_draw_string(display, display->width - charCount * 6 - 8, pos.y, 1,
                      buf);

  if (focus) {
    if (self->mv->value > self->mv->min_value) {
      ssd1306_draw_char(display, display->width - 18 - 6 * charCount, pos.y, 1,
                        '<');
    }
    if (self->mv->value < self->mv->max_value) {
      ssd1306_draw_char(display, display->width - 6, pos.y, 1, '>');
    }
  }

  ui_pos_t new_pos = {
    .x = display->width,
    .y = pos.y + 8,
  };
  return new_pos;
}

ui_element_t* ui_menu_item_midi_value_new(midi_value* mv) {
  ui_menu_item_midi_value_t* item = malloc(sizeof(ui_menu_item_midi_value_t));
  item->base.free = mi_free;
  item->base.render = ui_menu_item_midi_value_render;
  item->base.navigate = ui_menu_item_midi_value_navigate;
  item->mv = mv;

  return (ui_element_t*) item;
}
