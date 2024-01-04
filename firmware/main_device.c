/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *                    sekigon-gonnoc
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

// This example runs both host and device concurrently. The USB host receive
// reports from HID device and print it out over USB Device CDC interface.
// For TinyUSB roothub port0 is native usb controller, roothub port1 is
// pico-pio-usb.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "pico/cyw43_arch.h"

#include "pio_usb.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "midi.h"
#include "display.h"
#include "keyboard.h"
#include "settings.h"
#include "midi_ble.h"
#include "midi_uart.h"
#include "sync.h"

#include "main_host.h"

/*------------- MAIN -------------*/

uint32_t ms_since_boot() {
  return to_ms_since_boot(get_absolute_time());
}

// core0: handle device events
int main(void) {
  // default 125MHz is not appropreate. Sysclock should be multiple of 12MHz.
  set_sys_clock_khz(120000, true);

  sync_init();

  stdio_init_all();

  printf("---------\n");
  printf("startup\n");
  printf("---------\n");

  midi_uart_init();
  midi_ble_init();

  sleep_ms(10);
  keyboard_init();

  sleep_ms(10);

  // init device stack on native usb (roothub port0)
  //tud_init(0);

  //display_init();

  printf("all set up\n");

  uint32_t last_msg = ms_since_boot();
  bool led_state = false;

  while (true) {
    if (ms_since_boot() > last_msg + 1000) {
      midi_note_on(10);
      last_msg += 1000;
      led_state = !led_state;
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
    }
    //tud_task(); // tinyusb device task
    //tud_cdc_write_flush();
    keyboard_task();
    //display_task();
    sleep_ms(1);
  }

  return 0;
}

//--------------------------------------------------------------------+
// Device CDC
//--------------------------------------------------------------------+

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  (void) itf;

  char buf[64];
  uint32_t count = tud_cdc_read(buf, sizeof(buf));

  // TODO control LED on keyboard of host stack
  (void) count;
}

