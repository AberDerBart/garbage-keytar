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
#include "ui_element.h"

#define KEYBOARD_POS 0, 0
#define BLUETOOTH_POS 48, 0
#define CHARGE_POS 96, 0
#define BATTERY_POS 104, 0
#define KEY_MAPPING_POS 64, 0

void render_status(ui_element_t* status, ssd1306_t* display) {
  ssd1306_clear(display);
  if (keyboard_get_connected()) {
    ssd1306_bmp_show_image_with_offset(display, keyboard_24_16_bmp_data,
                                       keyboard_24_16_bmp_size, KEYBOARD_POS);
  }
  if (midi_ble_is_connected()) {
    ssd1306_bmp_show_image_with_offset(display, bluetooth_8_16_bmp_data,
                                       bluetooth_8_16_bmp_size, BLUETOOTH_POS);
  }
  if (battery_is_charging()) {
    ssd1306_bmp_show_image_with_offset(display, charge_8_16_bmp_data,
                                       charge_8_16_bmp_size, CHARGE_POS);
  }
  switch (battery_get_level()) {
    case EMPTY:
      ssd1306_bmp_show_image_with_offset(display, battery_empty_24_16_bmp_data,
                                         battery_empty_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case LOW:
      ssd1306_bmp_show_image_with_offset(display, battery_low_24_16_bmp_data,
                                         battery_low_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case MEDIUM:
      ssd1306_bmp_show_image_with_offset(display, battery_medium_24_16_bmp_data,
                                         battery_medium_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case FULL:
      ssd1306_bmp_show_image_with_offset(display, battery_full_24_16_bmp_data,
                                         battery_full_24_16_bmp_size,
                                         BATTERY_POS);
      break;
  }

  keymap_t* keymap = keymap_get();
  ssd1306_bmp_show_image_with_offset(display, keymap->icon_data,
                                     keymap->icon_size, KEY_MAPPING_POS);

  // char buffer[16];
  // snprintf(buffer, 15, "%.2fV", battery_get_voltage());
  // ssd1306_draw_string(display, 0, 20, 1, buffer);
  ssd1306_draw_string(display, 0, 20, 1, "test");

  ssd1306_show(display);
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
