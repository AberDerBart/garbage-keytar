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
#include <string.h>

#include "bsp/board.h"
#include "class/hid/hid.h"
#include "control.h"
#include "display.h"
#include "midi.h"
#include "midi_note_table.h"
#include "pico/stdlib.h"
#include "pitchbend.h"
#include "settings.h"
#include "tusb.h"
#include "tusb_config.h"

extern void hid_task(void);

void control_task();
void pitchbend_task();

int main(void) {
    board_init();

    tusb_init();
    midi_init();
    pitchbend_init();
    control_init();
    display_init();

    debug("INIT");
    draw_info();

    while (1) {
        // tinyusb host task
        tuh_task();

        // control_task();

        pitchbend_task();
    }

    return 0;
}

void pitchbend_task() {
    uint8_t const channel = 0;

    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    static struct pitchbend_value last_pitchbend = {
        low : 0,
        high : 0
    };
    struct pitchbend_value pitchbend = pitchbend_read();

    if (last_pitchbend.low != pitchbend.low || last_pitchbend.high != pitchbend.high) {
        midi_write3(0xe0 | channel, pitchbend.low, pitchbend.high);
    }

    last_pitchbend.low = pitchbend.low;
    last_pitchbend.high = pitchbend.high;
}

void control_task() {
    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    static uint8_t reported_values[2] = {0, 0};
    static uint8_t old_values[2] = {0, 0};

    for (uint8_t i = 0; i < 2; i++) {
        uint8_t val = read_control(i);

        if (abs(val - reported_values[i]) > 1 || val != reported_values[i] && old_values[i] != reported_values[i]) {
            midi_write3(0xb0, i, val);
            reported_values[i] = val;
        }

        old_values[i] = val;
    }
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *p_report, uint8_t keycode) {
    for (uint8_t i = 0; i < 6; i++) {
        if (p_report->keycode[i] == keycode)
            return true;
    }

    return false;
}

void clear_notes() {
    midi_write3(MIDI_CONTROL_CHANGE, MIDI_CONTROL_ALL_NOTES_OFF_B1, MIDI_CONTROL_ALL_NOTES_OFF_B2);
}

void handle_key(uint8_t key, bool pressed) {
#ifdef DEBUG_KEYS
    char msg[16];
    if (pressed) {
        sprintf(msg, "KEY %d %d PRE", key, keycode2midi[key]);
    } else {
        sprintf(msg, "KEY %d %d REL", key, keycode2midi[key]);
    }
    debug(msg);
    draw_info();
#endif

    if (pressed) {
        if (key >= 58 && key <= 69) {
            program = key - 58;
            midi_write2(MIDI_PROGRAM_CHANGE, program);
            draw_info();
        }
        if (key == 75) {
            int16_t max_note = 67 + offset + 12;
            if (max_note <= 127) {
                clear_notes();
                offset += 12;
                draw_info();
            }
        } else if (key == 78) {
            int16_t min_note = 35 + offset - 12;
            if (min_note > 0) {
                clear_notes();
                offset -= 12;
                draw_info();
            }
        } else if (key == 81) {
            int16_t min_note = 35 + offset - 1;
            if (min_note > 0) {
                clear_notes();
                offset--;
                draw_info();
            }
        } else if (key == 82) {
            int16_t max_note = 67 + offset + 1;
            if (max_note <= 127) {
                clear_notes();
                offset++;
                draw_info();
            }
        }
    }

    uint8_t raw_note = key < 128 ? keycode2midi[key] : 0;
    if (raw_note != 0) {
        board_led_write(pressed);
        if (pressed) {
            midi_write3(MIDI_NOTE_ON, raw_note + offset, 127);
        } else {
            midi_write3(MIDI_NOTE_ON, raw_note + offset, 0);
        }
    }
}

static inline void process_kbd_report(hid_keyboard_report_t const *p_new_report) {
    static hid_keyboard_report_t prev_report = {0, 0, {0}};  // previous report to check key released

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

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    /* Ask for a report only if this is a keyboard device */
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
    if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
        debug("KBD MOUNT");
        draw_info();
        tuh_hid_receive_report(dev_addr, instance);
    } else {
        debug("??? MOUNT");
        draw_info();
    }
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    switch (tuh_hid_interface_protocol(dev_addr, instance)) {
        case HID_ITF_PROTOCOL_KEYBOARD:
            process_kbd_report((hid_keyboard_report_t const *)report);
            tuh_hid_receive_report(dev_addr, instance);
            break;
    }
}
