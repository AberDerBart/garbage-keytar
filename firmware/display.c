#include "display.h"

#include "hardware/i2c.h"
#include "img/gen/battery_empty_24_16.h"
#include "img/gen/battery_full_24_16.h"
#include "img/gen/battery_low_24_16.h"
#include "img/gen/battery_medium_24_16.h"
#include "img/gen/bluetooth_8_16.h"
#include "img/gen/charge_8_16.h"
#include "img/gen/keyboard_24_16.h"
#include "menu_navigation.h"
#include "pico-ssd1306/ssd1306.h"
#include "settings.h"
#include "stdio.h"
#include "string.h"

#define SDA_PIN 8
#define SCL_PIN 9

#define KEYBOARD_POS 0, 0
#define BLUETOOTH_POS 48, 0
#define CHARGE_POS 96, 0
#define BATTERY_POS 104, 0
#define KEY_MAPPING_POS 64, 0

void draw_info();
void draw_menu();

ssd1306_t disp;
uint8_t menu_index = 0xff;

bool display_initialized = false;

display_state_t state;
display_state_t prev_state;

display_state_t* display_get_state_for_update() { return &state; }

void redraw();

void display_init() {
  printf("init display\n");

  // init hardware
  i2c_init(i2c0, 400000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  disp.external_vcc = false;
  display_initialized = ssd1306_init(&disp, 128, 64, 0x3c, i2c0);

  if (!display_initialized) {
    printf("failed to initialize display\n");
    return;
  }

  ssd1306_set_rotation(&disp, ROT_180);

  // init state
  state.keyboard = false;
  state.bluetooth = false;
  state.charging = false;
  state.battery = EMPTY;
  state.keymap = default_keymap;

  memcpy(&prev_state, &state, sizeof(state));

  redraw();

  printf("display initialized\n");
}

void draw_main() {
  if (state.keyboard) {
    ssd1306_bmp_show_image_with_offset(&disp, keyboard_24_16_bmp_data,
                                       keyboard_24_16_bmp_size, KEYBOARD_POS);
  }
  if (state.bluetooth) {
    ssd1306_bmp_show_image_with_offset(&disp, bluetooth_8_16_bmp_data,
                                       bluetooth_8_16_bmp_size, BLUETOOTH_POS);
  }
  if (state.charging) {
    ssd1306_bmp_show_image_with_offset(&disp, charge_8_16_bmp_data,
                                       charge_8_16_bmp_size, CHARGE_POS);
  }
  switch (state.battery) {
    case EMPTY:
      ssd1306_bmp_show_image_with_offset(&disp, battery_empty_24_16_bmp_data,
                                         battery_empty_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case LOW:
      ssd1306_bmp_show_image_with_offset(&disp, battery_low_24_16_bmp_data,
                                         battery_low_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case MEDIUM:
      ssd1306_bmp_show_image_with_offset(&disp, battery_medium_24_16_bmp_data,
                                         battery_medium_24_16_bmp_size,
                                         BATTERY_POS);
      break;
    case FULL:
      ssd1306_bmp_show_image_with_offset(&disp, battery_full_24_16_bmp_data,
                                         battery_full_24_16_bmp_size,
                                         BATTERY_POS);
      break;
  }

  ssd1306_bmp_show_image_with_offset(&disp, state.keymap->icon_data,
                                     state.keymap->icon_size, KEY_MAPPING_POS);
}

void draw_menu() {
  if (!state.menu_state.current) {
    return;
  }
  for (uint8_t i = 0; i < menu_len(state.menu_state.current); i++) {
    menu_item_t* child = state.menu_state.current->children[i];
    ssd1306_draw_string(&disp, 8, i * 8, 1, child->label);
    if (state.menu_state.index == i) {
      ssd1306_draw_char(&disp, 0, i * 8, 1, '>');
    }
  }
};

void redraw() {
  ssd1306_clear(&disp);

  if (state.menu_state.current) {
    draw_menu();
  } else {
    draw_main();
  }

  ssd1306_show(&disp);
}

void display_task() {
  if (!display_initialized) {
    return;
  }

  if (!memcmp(&state, &prev_state, sizeof(state))) {
    // No changes, skip rendering
    return;
  }

  redraw();
  memcpy(&prev_state, &state, sizeof(state));

  return;
}
