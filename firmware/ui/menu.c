#include "./menu.h"

#include <stdint.h>
#include <stdlib.h>

#include "ui_stack.h"

#define W_CHARACTER 6
#define H_CHARACTER 8
#define FONT_SCALE 1

void free_menu(ui_element_t* element) {
  ui_menu_t* menu = (ui_menu_t*)element;

  for (int i = 0; i < menu->length; i++) {
    ui_element_t* item = menu->items[i];
    if (item->free) {
      (item->free)(item);
    }
  }
  free(menu->items);
  free(menu);
}

void navigate_menu(ui_element_t* item, ui_nav_t nav) {
  ui_menu_t* self = (ui_menu_t*)item;

  switch (nav) {
    case UP:
      self->index = MAX(self->index - 1, 0);
      if (self->scroll > self->index) {
        self->scroll = self->index;
      }
      break;
    case DOWN:
      self->index = MIN(self->index + 1, MAX(self->length - 1, 0));
      if (self->scroll < self->index - 8 + 1) {
        self->scroll = self->index - 8 + 1;
      }
      break;
    default:
      if (!self->items[self->index]->navigate) {
        return;
      }
      (self->items[self->index]->navigate)(self->items[self->index], nav);
      break;
  }
}

ui_pos_t render_menu(ui_element_t* item, ssd1306_t* display, ui_pos_t pos,
                     bool focus) {
  ui_menu_t* self = (ui_menu_t*)item;

  ssd1306_clear(display);

  uint8_t y = pos.y;
  for (uint8_t i_line = 0; i_line < 8 && i_line + self->scroll < self->length;
       i_line++) {
    uint8_t index = self->scroll + i_line;
    if (index == self->index) {
      ssd1306_draw_char(display, 0, y, 1, '>');
    }
    ui_element_t* item = self->items[index];
    ui_pos_t element_pos = {
      x : pos.x + 8,
      y : y,
    };
    item->render(item, display, element_pos, focus && index == self->index);
    // TODO: respect item render height
    y += 8;
  }

  ssd1306_show(display);

  ui_pos_t new_pos = {
    x : display->width,
    y : y,
  };
  return new_pos;
}

ui_menu_t* push_menu(uint8_t item_capacity) {
  ui_menu_t* menu = malloc(sizeof(ui_menu_t));
  menu->base.free = free_menu;
  menu->base.navigate = navigate_menu;
  menu->base.render = render_menu;

  menu->items = malloc(item_capacity * sizeof(ui_element_t*));
  menu->length = 0;
  menu->index = 0;
  menu->scroll = 0;

  ui_push((ui_element_t*)menu);

  return menu;
}

ui_element_t* menu_element(ui_menu_t* menu) { return (ui_element_t*)menu; };

void menu_add(ui_menu_t* menu, ui_element_t* item) {
  menu->items[menu->length] = item;
  menu->length++;
}
