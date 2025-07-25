#include "ui_menu.h"

#include <stdint.h>
#include <stdlib.h>

#include "ui_stack.h"

#define W_CHARACTER 6
#define H_CHARACTER 8
#define FONT_SCALE 1

void free_menu(ui_element_t* item) {
  ui_menu_t* self = (ui_menu_t*)item;
  free(self);
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
    case ENTER:
      if (!self->items[self->index]->action) {
        return;
      }
      (self->items[self->index]->action)(self->items[self->index]);
      break;
    case LEFT:
    case RIGHT:
    default:
      break;
  }
}

void render_menu(ui_element_t* item, ssd1306_t* display) {
  ui_menu_t* self = (ui_menu_t*)item;

  ssd1306_clear(display);

  for (uint8_t i_line = 0; i_line < 8 && i_line + self->scroll < self->length;
       i_line++) {
    uint8_t index = self->scroll + i_line;
    uint8_t y = i_line * 8;
    if (index == self->index) {
      ssd1306_draw_char(display, 0, y, 1, '>');
    }
    ssd1306_draw_string(display, 8, y, 1, self->items[index]->label);
  }

  ssd1306_show(display);
}

void push_menu(uint8_t length, menu_item_t** items) {
  ui_menu_t* menu = malloc(sizeof(ui_menu_t));
  menu->base.free = free_menu;
  menu->base.navigate = navigate_menu;
  menu->base.render = render_menu;

  menu->length = length;
  menu->items = items;

  menu->index = 0;
  menu->scroll = 0;

  ui_push((ui_element_t*)menu);
}

void menu_add(ui_menu_t* menu, menu_item_t* item) {
  for (int i = 0; i < menu->length; i++) {
    if (!menu->items[i]) {
      menu->items[i] = item;
      return;
    }
  }
};

ui_element_t* menu_element(ui_menu_t* menu) { return (ui_element_t*)menu; };
