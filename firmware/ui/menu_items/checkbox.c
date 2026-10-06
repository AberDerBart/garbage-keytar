#include "checkbox.h"

#include <stdlib.h>

#include "img/gen/check_off_8_8.h"
#include "img/gen/check_on_8_8.h"

typedef struct {
  ui_element_t base;
  char* label;
  bool value;
  void (*on_change)(bool newVal);
} checkbox_t;

static void navigate(ui_element_t* item, ui_nav_t nav) {
  checkbox_t* self = (checkbox_t*) item;
  if (nav == ENTER) {
    self->value = !self->value;
    if (self->on_change) {
      self->on_change(self->value);
    }
  }
}

static void cb_free(ui_element_t* e) {
  free(e);
}

static ui_pos_t render(ui_element_t* item, ssd1306_t* display, ui_pos_t pos, bool focus) {
  checkbox_t * self = (checkbox_t*) item;
  ssd1306_draw_string(display, pos.x, pos.y, 1, self->label);

  if (*self->value) {
    ssd1306_bmp_show_image_with_offset(display, check_on_8_8_bmp_data, check_on_8_8_bmp_size, display->width-8, pos.y);
  } else {
    ssd1306_bmp_show_image_with_offset(display, check_off_8_8_bmp_data, check_off_8_8_bmp_size, display->width-8, pos.y);
  }

  ui_pos_t new_pos = {
    .x = display->width,
    .y = pos.y + 8,
  };
  return new_pos;
}

ui_element_t* ui_menu_item_checkbox_new(char* label, bool initial_value, void (*on_change)(bool newValue)) {
  checkbox_t* checkbox = malloc(sizeof(checkbox_t));
  checkbox->base.free = cb_free;
  checkbox->base.render = render;
  checkbox->base.navigate = navigate;
  checkbox->label = label;
  checkbox->value = initial_value;
  checkbox->on_change = on_change;

  return (ui_element_t*)checkbox;
}
