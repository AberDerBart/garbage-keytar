/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>

#include "analog_strip.h"
#include "battery.h"
#include "config.h"
#include "display.h"
#include "keyboard.h"
#include "midi_ble.h"
#include "midi_uart.h"
#include "pico/stdio.h"
#include "midi_value.h"

extern void hid_task(void);

int main(void) {
  stdio_init_all();

  printf("---------\n");
  printf("startup\n");
  printf("---------\n");

  keyboard_init();

  midi_uart_init();
  battery_init();

  analog_strip_init();
  display_init();

  config_init();

  midi_ble_init();

  printf("all set up\n");

  while (1) {
    keyboard_task();

    analog_strip_read(&mv_pitchbend, true);
  }

  return 0;
}
