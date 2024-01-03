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

#include "pico/stdlib.h"
#include "class/hid/hid.h"
#include "tusb.h"
#include "tusb_config.h"

#include "control.h"
#include "display.h"
#include "midi.h"
#include "pitchbend.h"
#include "settings.h"

extern void hid_task(void);

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
