#include "./profiles.h"

#include <stdio.h>
#include <stdlib.h>

#include "./envelope.h"
#include "profile.h"
#include "ui/menu.h"
#include "ui/menu_items.h"

typedef struct {
  ui_element_t base;
  uint8_t profile_index;
} profile_item_t;

ui_pos_t profile_item_render(ui_element_t* item, ssd1306_t* display,
                             ui_pos_t pos, bool focus) {
  profile_item_t* self = (profile_item_t*)item;
  char buf[4];
  sprintf(buf, "F%d", self->profile_index);
  ssd1306_draw_string(display, pos.x, pos.y, 1, buf);

  ui_pos_t new_pos = {
    x : display->width,
    y : pos.y + 8,
  };
  return new_pos;
}

void profile_item_free(ui_element_t* item) { free(item); }

void profile_item_navigate(ui_element_t* item, ui_nav_t nav) {
  if (nav != ENTER) {
    return;
  }
  profile_item_t* self = (profile_item_t*)item;

  ui_menu_t* menu = push_menu(5);

  profile_t* profile = &profiles[self->profile_index];
  menu_add_envelope_entries(menu, &profile->adsr);
}

void push_menu_profiles() {
  ui_menu_t* menu = push_menu(12);
  for (int i = 0; i < 12; i++) {
    profile_item_t* item = malloc(sizeof(profile_item_t));
    item->base.free = profile_item_free;
    item->base.render = profile_item_render;
    item->base.navigate = profile_item_navigate;
    item->profile_index = i;
    menu_add(menu, item);
  }
}