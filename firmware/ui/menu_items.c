#include "./menu_items.h"

#include <stdlib.h>

void navigate_default_menu_item(ui_element_t* item, ui_nav_t nav) {
  if (nav != ENTER) {
    return;
  }
  ui_menu_item_default_t* self = (ui_menu_item_default_t*)item;
  self->action();
}

ui_pos_t ui_menu_item_default_render(ui_element_t* item, ssd1306_t* display,
                                     ui_pos_t pos) {
  ui_menu_item_default_t* self = (ui_menu_item_default_t*)item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);
  ui_pos_t new_pos = {
    x : display->width,
    y : pos.y + 8,
  };
  return new_pos;
}

void ui_menu_item_default_free(ui_element_t* item) { free(item); }

ui_element_t* make_default_menu_item(char* label, void (*action)()) {
  ui_menu_item_default_t* item = malloc(sizeof(ui_menu_t));
  item->base.free = ui_menu_item_default_free;
  item->base.render = ui_menu_item_default_render;
  item->base.navigate = navigate_default_menu_item;
  item->label = label;
  item->action = action;

  return (ui_element_t*)item;
}