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

#include "tusb_config.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "midi_note_table.h"
#include "midi.h"
#include "pitchbend.h"
#include "control.h"
#include "display.h"
#include "offset.h"
#include "hid.h"

extern void hid_task(void);

void control_task();
void pitchbend_task();

int main(void)
{
    board_init();

    tusb_init();
    midi_init();
    pitchbend_init();
    control_init();
    display_init();

    while (1)
    {
        // tinyusb host task
        tuh_task();

        hid_task();

        // control_task();

        pitchbend_task();
    }

    return 0;
}

void pitchbend_task()
{
    uint8_t const channel = 0;

    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms)
    {
        return;
    }
    start_ms += interval_ms;

    static struct pitchbend_value last_pitchbend = {
        low : 0,
        high : 0
    };
    struct pitchbend_value pitchbend = pitchbend_read();

    if(last_pitchbend.low != pitchbend.low || last_pitchbend.high != pitchbend.high){
      midi_write(0xe0 | channel, pitchbend.low, pitchbend.high);
    }

    last_pitchbend.low = pitchbend.low;
    last_pitchbend.high = pitchbend.high;
}

void control_task() {
    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms)
    {
        return;
    }
    start_ms += interval_ms;

    static uint8_t reported_values[2] = {0,0};
    static uint8_t old_values[2] = {0,0};

    for(uint8_t i=0; i<2; i++){
        uint8_t val = read_control(i);

        if(abs(val-reported_values[i]) > 1 || val != reported_values[i] && old_values[i] != reported_values[i]){
            midi_write(0xb0, i, val);
            reported_values[i] = val;
        }

        old_values[i] = val;
    }
}
    

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
#if CFG_TUH_HID_KEYBOARD

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *p_report, uint8_t keycode)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (p_report->keycode[i] == keycode)
            return true;
    }

    return false;
}

void clear_notes();

void handle_key(uint8_t key, bool pressed)
{
    if(pressed){
      if(key == 75){
        clear_notes();
        int16_t max_note = 67 + offset + 12;
        if(max_note <= 127) {
          offset += 12;
        }
        draw_info();
      } else if(key == 78){
        clear_notes();
        int16_t min_note = 35 + offset -12;
        if(min_note > 0) {
          offset -= 12;
        }
        draw_info();
      } else if(key == 81){
        clear_notes();
        int16_t min_note = 35 + offset -1;
        if(min_note > 0) {
          offset--;
        }
        draw_info();
      } else if(key == 82){
        clear_notes();
        int16_t max_note = 67 + offset +1;
        if(max_note <= 127) {
          offset++;
        }
        draw_info();
      }
    }

    uint8_t note = key < 128 ? keycode2midi[key] + offset : 0;
    if (note != 0) {
        board_led_write(pressed);
        if (pressed) {
            midi_write(MIDI_NOTE_ON, note, 127);
        } else {
            midi_write(MIDI_NOTE_ON, note, 0);
        }
    }
}

void clear_notes() {
  midi_write(MIDI_CHANNEL_MODE, MIDI_CHANNEL_MODE_ALL_SOUND_OFF_B1, MIDI_CHANNEL_MODE_ALL_SOUND_OFF_B2);
}

static inline void process_kbd_report(hid_keyboard_report_t const *p_new_report)
{
    static hid_keyboard_report_t prev_report = {0, 0, {0}}; // previous report to check key released

    // check key presses
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t key = p_new_report->keycode[i];
        if (key && !find_key_in_report(&prev_report, key)) {
            handle_key(key, true);
        }
    }

    // check key releases
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t key = prev_report.keycode[i];
        if (key && !find_key_in_report(p_new_report, key)) {
            handle_key(key, false);
        }
    }

    prev_report = *p_new_report;
}

void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr)
{
    // application set-up
    printf("A Keyboard device (address %d) is mounted\r\n", dev_addr);

    tuh_hid_keyboard_get_report(dev_addr, &usb_keyboard_report);
}

void tuh_hid_keyboard_unmounted_cb(uint8_t dev_addr)
{
    // application tear-down
    printf("A Keyboard device (address %d) is unmounted\r\n", dev_addr);
}

// invoked ISR context
void tuh_hid_keyboard_isr(uint8_t dev_addr, xfer_result_t event)
{
    (void)dev_addr;
    (void)event;
}

#endif

void hid_task(void)
{
    uint8_t const addr = 1;
    if (tuh_hid_keyboard_is_mounted(addr))
    {
        if (!tuh_hid_keyboard_is_busy(addr))
        {
            process_kbd_report(&usb_keyboard_report);
            tuh_hid_keyboard_get_report(addr, &usb_keyboard_report);
        }
    }
}
