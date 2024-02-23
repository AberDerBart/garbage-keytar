#include "display.h"

#include "hardware/i2c.h"
#include "pico-ssd1306/ssd1306.h"
#include "settings.h"
#include "stdio.h"
#include "string.h"

#define SDA_PIN 8
#define SCL_PIN 9

void draw_info();
void draw_menu();

ssd1306_t disp;
uint8_t menu_index = 0xff;

bool display_initialized = false;

void display_init() {
    printf("init display\n");
    i2c_init(i2c0, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    disp.external_vcc = false;
    display_initialized = ssd1306_init(&disp, 128, 64, 0x3c, i2c0);
    ssd1306_set_rotation(&disp, ROT_180);
    ssd1306_clear(&disp);
    ssd1306_show(&disp);

    if(display_initialized) {
      printf("display display_initialized\n");
    }else{
      printf("failed to initialize display\n");
    }
}

void display_task() {
    if(!display_initialized) {
      return;
    }
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

