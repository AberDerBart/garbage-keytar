#include "display.h"

#include "hardware/i2c.h"
#include "offset.h"
#include "pico-ssd1306/ssd1306.h"
#include "stdio.h"
#include "string.h"

#define SDA_PIN 8
#define SCL_PIN 9

void draw_info();
void draw_menu();
void debug_render();

ssd1306_t disp;
uint8_t menu_index = 0xff;

void display_init() {
    i2c_init(i2c0, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3c, i2c0);

    draw_info();
}

void draw_info() {
    const uint8_t keyb_h = 32;
    const uint8_t key_w = 12;

    const uint8_t keyb_x = 4;
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

    ssd1306_draw_char(&disp, 92, 32, 2, 'O');
    ssd1306_draw_char(&disp, 105, 32, 2, 'C');
    ssd1306_draw_char(&disp, 118, 32, 2, 'T');

    int8_t octave = offset_get_octave();
    if (octave < 0) {
        ssd1306_draw_char(&disp, 99, 48, 2, '-');
        octave = -octave;
    } else if (octave > 0) {
        ssd1306_draw_char(&disp, 99, 48, 2, '+');
    }
    ssd1306_draw_char(&disp, 112, 48, 2, octave + '0');
    debug_render();
    ssd1306_show(&disp);
}

char debug_lines[4][17];
uint8_t debug_offset = 0;

void debug(char* str) {
    uint8_t n = strncpy(debug_lines[debug_offset], str, 16);
    debug_lines[debug_offset][n] = 0;
    debug_offset = (debug_offset + 1) % 4;
}

void debug_render() {
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t line_index = (i + debug_offset) % 4;
        ssd1306_draw_string(&disp, 0, 8 * i, 1, debug_lines[line_index]);
    }
}
