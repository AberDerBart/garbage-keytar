#include "display.h"

#include "hardware/i2c.h"
#include "pico-ssd1306/ssd1306.h"
#include "settings.h"
#include "stdio.h"
#include "string.h"

#include "img/gen/keyboard_24_16.h"
#include "img/gen/bluetooth_8_16.h"
#include "img/gen/charge_8_16.h"
#include "img/gen/battery_empty_24_16.h"
#include "img/gen/battery_low_24_16.h"
#include "img/gen/battery_medium_24_16.h"
#include "img/gen/battery_full_24_16.h"


#define SDA_PIN 8
#define SCL_PIN 9

#define KEYBOARD_POS 0,0
#define BLUETOOTH_POS 48,0
#define CHARGE_POS 96,0
#define BATTERY_POS 104,0
#define KEY_MAPPING_POS 64,0

void draw_info();
void draw_menu();

ssd1306_t disp;
uint8_t menu_index = 0xff;

bool display_initialized = false;

display_state_t state;
display_state_t prev_state;

display_state_t* display_get_state_for_update() {
  return &state;
}

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

    if(!display_initialized) {
      printf("failed to initialize display\n");
      return;
    }

    ssd1306_set_rotation(&disp, ROT_180);

    // init state
    state.keyboard = false;
    state.bluetooth = false;
    state.charging = false;
    state.battery = EMPTY;
    state.key_mapping = key_mapping;

    memcpy(&prev_state, &state, sizeof(state));

    redraw();

    printf("display initialized\n");
}


void redraw() {
  ssd1306_clear(&disp);

  if(state.keyboard){
    ssd1306_bmp_show_image_with_offset(&disp, keyboard_24_16_bmp_data, keyboard_24_16_bmp_size, KEYBOARD_POS);
  }
  if(state.bluetooth){
    ssd1306_bmp_show_image_with_offset(&disp, bluetooth_8_16_bmp_data, bluetooth_8_16_bmp_size, BLUETOOTH_POS);
  }
  if(state.charging){
    ssd1306_bmp_show_image_with_offset(&disp, charge_8_16_bmp_data, charge_8_16_bmp_size, CHARGE_POS);
  }
  switch (state.battery) {
  case EMPTY:
    ssd1306_bmp_show_image_with_offset(&disp, battery_empty_24_16_bmp_data, battery_empty_24_16_bmp_size, BATTERY_POS);
    break;
  case LOW:
    ssd1306_bmp_show_image_with_offset(&disp, battery_low_24_16_bmp_data, battery_low_24_16_bmp_size, BATTERY_POS);
    break;
  case MEDIUM:
    ssd1306_bmp_show_image_with_offset(&disp, battery_medium_24_16_bmp_data, battery_medium_24_16_bmp_size, BATTERY_POS);
    break;
  case FULL:
    ssd1306_bmp_show_image_with_offset(&disp, battery_full_24_16_bmp_data, battery_full_24_16_bmp_size, BATTERY_POS);
    break;
  }

  ssd1306_bmp_show_image_with_offset(&disp, state.key_mapping->icon_data, state.key_mapping->icon_size, KEY_MAPPING_POS);

  ssd1306_show(&disp);
}

void display_task() {
    if(!display_initialized) {
      return;
    }

    if(!memcmp(&state, &prev_state, sizeof(state))) {
      // No changes, skip rendering
      return;
    }

    redraw();
    memcpy(&prev_state, &state, sizeof(state));

    return;
    const uint8_t keyb_h = 32;
    const uint8_t key_w = 8;

    const uint8_t keyb_x = 0;
    const uint8_t keyb_y = 32;

    const uint8_t trans = offset_get_trans();

    ssd1306_clear(&disp);

    ssd1306_draw_square(&disp, keyb_x, keyb_y, key_w * 7, 1);
    ssd1306_draw_square(&disp, keyb_x, keyb_y + keyb_h - 1, key_w * 7, 1);
    for (int i = 0; i < 8; i++) {
        if (i == 1 || i == 2 || i >= 4 && i <= 6) {
            // black keys
            ssd1306_draw_square(&disp, keyb_x + i * key_w, keyb_y + keyb_h / 2, 1, keyb_h - keyb_h / 2);
            ssd1306_draw_square(&disp, keyb_x + i * key_w - key_w / 4, keyb_y, 1, keyb_h / 2);
            ssd1306_draw_square(&disp, keyb_x + i * key_w + key_w / 4, keyb_y, 1, keyb_h / 2);
            ssd1306_draw_square(&disp, keyb_x + i * key_w - key_w / 4, keyb_y + keyb_h / 2 - 1, key_w / 4 * 2, 1);
        } else {
            // white keys
            ssd1306_draw_square(&disp, keyb_x + i * key_w, keyb_y, 1, keyb_h);
        }

        // highlight key
        if (i == 1 && trans == 1 || i == 2 && trans == 3 || i == 4 && trans == 6 || i == 5 && trans == 8 || i == 6 && trans == 10) {
            ssd1306_draw_square(&disp, keyb_x + i * key_w - key_w / 4, keyb_y, key_w / 4 * 2, keyb_h / 2);
        } else if (i == 0 && trans == 0 || i == 1 && trans == 2 || i == 2 && trans == 4 || i == 3 && trans == 5 || i == 4 && trans == 7 || i == 5 && trans == 9 || i == 6 && trans == 11) {
            ssd1306_draw_square(&disp, keyb_x + i * key_w, keyb_y + keyb_h / 2, key_w, keyb_h - keyb_h / 2);

            if (i == 0 || i == 3) {
                ssd1306_draw_square(&disp, keyb_x + i * key_w, keyb_y, key_w - key_w / 4, keyb_h / 2);
            } else if (i == 2 || i == 6) {
                ssd1306_draw_square(&disp, keyb_x + i * key_w + key_w / 4, keyb_y, key_w - key_w / 4, keyb_h / 2);
            } else {
                ssd1306_draw_square(&disp, keyb_x + i * key_w + key_w / 4, keyb_y, key_w - 2 * key_w / 4, keyb_h / 2);
            }
        }
    }

    ssd1306_draw_char(&disp, 62, 32, 2, 'O');
    ssd1306_draw_char(&disp, 75, 32, 2, 'C');
    ssd1306_draw_char(&disp, 88, 32, 2, 'T');

    int8_t octave = offset_get_octave();
    if (octave < 0) {
        ssd1306_draw_char(&disp, 102, 32, 2, '-');
        octave = -octave;
    } else if (octave > 0) {
        ssd1306_draw_char(&disp, 102, 32, 2, '+');
    }
    ssd1306_draw_char(&disp, 115, 32, 2, octave + '0');

    ssd1306_draw_char(&disp, 62, 48, 2, 'P');
    ssd1306_draw_char(&disp, 75, 48, 2, 'R');
    ssd1306_draw_char(&disp, 88, 48, 2, 'G');

    if (program > 9) {
        ssd1306_draw_char(&disp, 102, 48, 2, '1');
    }
    ssd1306_draw_char(&disp, 115, 48, 2, (program % 10) + '0');

    ssd1306_show(&disp);
}

