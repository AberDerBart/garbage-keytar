#include "./status.h"

#include "battery.h"
#include "img/gen/battery_empty_24_16.h"
#include "img/gen/battery_full_24_16.h"
#include "img/gen/battery_low_24_16.h"
#include "img/gen/battery_medium_24_16.h"
#include "img/gen/bluetooth_8_16.h"
#include "img/gen/charge_8_16.h"
#include "img/gen/keyboard_24_16.h"
#include "keyboard.h"
#include "keyboard_matrix.h"
#include "menu/main.h"
#include "midi_ble.h"
#include "pico-ssd1306/ssd1306.h"
#include "ui/element.h"

ui_pos_t render_status(ui_element_t* status, ssd1306_t* display, ui_pos_t pos,
                       bool focus) {
  ssd1306_clear(display);
  if (keyboard_get_connected()) {
    ssd1306_bmp_show_image_with_offset(display, keyboard_24_16_bmp_data,
                                       keyboard_24_16_bmp_size, pos.x, pos.y);
  }
  if (midi_ble_is_connected()) {
    ssd1306_bmp_show_image_with_offset(display, bluetooth_8_16_bmp_data,
                                       bluetooth_8_16_bmp_size, pos.x + 48,
                                       pos.y);
  }
  if (battery_is_charging()) {
    ssd1306_bmp_show_image_with_offset(display, charge_8_16_bmp_data,
                                       charge_8_16_bmp_size, pos.x + 96, pos.y);
  }
  switch (battery_get_level()) {
    case EMPTY:
      ssd1306_bmp_show_image_with_offset(display, battery_empty_24_16_bmp_data,
                                         battery_empty_24_16_bmp_size,
                                         pos.x + 104, pos.y);
      break;
    case LOW:
      ssd1306_bmp_show_image_with_offset(display, battery_low_24_16_bmp_data,
                                         battery_low_24_16_bmp_size,
                                         pos.x + 104, pos.y);
      break;
    case MEDIUM:
      ssd1306_bmp_show_image_with_offset(display, battery_medium_24_16_bmp_data,
                                         battery_medium_24_16_bmp_size,
                                         pos.x + 104, pos.y);
      break;
    case FULL:
      ssd1306_bmp_show_image_with_offset(display, battery_full_24_16_bmp_data,
                                         battery_full_24_16_bmp_size,
                                         pos.x + 104, pos.y);
      break;
  }

  keymap_t* keymap = keymap_get();
  ssd1306_bmp_show_image_with_offset(display, keymap->icon_data,
                                     keymap->icon_size, pos.x + 64, pos.y);

  // char buffer[16];
  // snprintf(buffer, 15, "%.2fV", battery_get_voltage());
  // ssd1306_draw_string(display, 0, 20, 1, buffer);
  ssd1306_draw_string(display, 0, 20, 1, "test");

  ui_pos_t new_pos = {
    x : display->width,
    y : display->height,
  };
  return new_pos;
}

void navigate_status(ui_element_t* status, ui_nav_t nav) {
  switch (nav) {
    case CLOSE:
    case RETURN:
      push_menu_main();
      break;
    default:
      break;
  }

  return;
}

ui_element_t ui_status = {
    .free = NULL,
    .render = render_status,
    .navigate = navigate_status,
};
