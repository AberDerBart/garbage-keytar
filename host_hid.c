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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

void print_greeting(void);
void led_blinking_task(void);
extern void hid_task(void);

int main(void)
{
    board_init();
    print_greeting();

    tusb_init();

    while (1)
    {
        // tinyusb host task
        tuh_task();
        // led_blinking_task();

#if CFG_TUH_HID_KEYBOARD
        hid_task();
#endif
    }

    return 0;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
#if CFG_TUH_HID_KEYBOARD

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;
uint8_t const keycode2ascii[128][2] = {HID_KEYCODE_TO_ASCII};

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

static inline void process_kbd_report(hid_keyboard_report_t const *p_new_report)
{
    static hid_keyboard_report_t prev_report = {0, 0, {0}}; // previous report to check key released

    //------------- example code ignore control (non-printable) key affects -------------//
    for (uint8_t i = 0; i < 6; i++)
    {
        if (p_new_report->keycode[i])
        {
            if (find_key_in_report(&prev_report, p_new_report->keycode[i]))
            {
                // exist in previous report means the current key is holding
            }
            else
            {
                // not existed in previous report means the current key is pressed
                bool const is_shift =
                    p_new_report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
                uint8_t ch = keycode2ascii[p_new_report->keycode[i]][is_shift ? 1 : 0];
                putchar(ch);
                if (ch == '\r')
                    putchar('\n'); // added new line for enter key

                fflush(stdout); // flush right away, else nanolib will wait for newline
            }
        }
        // TODO example skips key released
    }

    prev_report = *p_new_report;
}

void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr)
{
    // application set-up
    printf("A Keyboard device (address %d) is mounted\r\n", dev_addr);
    board_led_on();

    tuh_hid_keyboard_get_report(dev_addr, &usb_keyboard_report);
}

void tuh_hid_keyboard_unmounted_cb(uint8_t dev_addr)
{
    // application tear-down
    printf("A Keyboard device (address %d) is unmounted\r\n", dev_addr);
    board_led_off();
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

#if CFG_TUH_HID_KEYBOARD
    if (tuh_hid_keyboard_is_mounted(addr))
    {
        if (!tuh_hid_keyboard_is_busy(addr))
        {
            process_kbd_report(&usb_keyboard_report);
            tuh_hid_keyboard_get_report(addr, &usb_keyboard_report);
        }
    }
#endif
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    const uint32_t interval_ms = 250;
    static uint32_t start_ms = 0;

    static bool led_state = false;

    // Blink every interval ms
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// HELPER FUNCTION
//--------------------------------------------------------------------+
void print_greeting(void)
{
    printf("This Host demo is configured to support:\n");
    if (CFG_TUH_HID_KEYBOARD)
        puts("  - HID Keyboard");
}
