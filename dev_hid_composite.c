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

#include "usb_descriptors.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void midi_task(void);
void midi_task2(void);

#define UART_ID uart0
#define BAUD_RATE 31250
#define MIDI_OUT_PIN 0
#define MIDI_IN_PIN 1

/*------------- MAIN -------------*/
int main(void) {
    board_init();
    tusb_init();

    adc_init();
    adc_gpio_init(28);

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(MIDI_OUT_PIN, GPIO_FUNC_UART);
    gpio_set_function(MIDI_IN_PIN, GPIO_FUNC_UART);

    while (1) {
        tud_task(); // tinyusb device task
        led_blinking_task();

        midi_task2();
    }

    return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// MIDI Task
//--------------------------------------------------------------------+

struct pitchbend_value {
    uint8_t high;
    uint8_t low;
};

struct pitchbend_value read_pitchbend(void) {
    adc_select_input(2);
    uint16_t const ACTIVATION_THRESHOLD = 3276;
    uint16_t const LOWER_BOUND = 30;
    uint16_t const UPPER_BOUND = 2700;
    uint16_t const INACTIVE = 0xffff;

    static uint16_t start_value = INACTIVE;

    uint16_t adc_value = adc_read();

    struct pitchbend_value to_pitchbend(int16_t value) {
        if(value > (1 << 13) - 1) {
            return to_pitchbend((1 << 13) -1);
        }

        if (value < -(1 <<13)) {
            return to_pitchbend(-(1 << 13));
        }

        uint16_t offset_value = value + 0x2000;

        struct pitchbend_value result = {
            low: offset_value & 0x7f,
            high: (offset_value >> 7) & 0x7f
        };

        return result;
    }

    if (start_value == INACTIVE) {
        if (adc_value < ACTIVATION_THRESHOLD) {
            start_value = adc_read();
            return to_pitchbend(0);
        }
    } else {
        if (adc_value > ACTIVATION_THRESHOLD) {
            start_value = INACTIVE;
            return to_pitchbend(0);
        } else {
            int32_t value = (int32_t) (1 << 14) * (adc_value - start_value) / (UPPER_BOUND-LOWER_BOUND);
            return to_pitchbend(value);
        }
    } 
    // just in case...
    return to_pitchbend(0); 
}

void midi_write(uint8_t cmd[3]) {
    uart_putc(UART_ID, cmd[0]);
    uart_putc(UART_ID, cmd[1]);
    uart_putc(UART_ID, cmd[2]);
}

void midi_task2(void) {
    uint8_t const channel = 0;

    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    static struct pitchbend_value last_pitchbend = {
        low: 0,
        high: 0
    };
    struct pitchbend_value pitchbend = read_pitchbend();

    uint8_t cmd[3];
    cmd[0] = 0xe0|channel;
    cmd[1] = pitchbend.low;
    cmd[2] = pitchbend.high;
    midi_write(cmd);
}

void midi_task(void) {
    uint8_t const cable_num = 0;
    uint8_t const channel = 0;

    // Flush MIDI input
    uint8_t packet[4];
    while(tud_midi_available()) tud_midi_receive(packet);


    static uint32_t start_ms = 0;
    const uint32_t interval_ms = 10;
    if (board_millis() - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    static struct pitchbend_value last_pitchbend = {
        low: 0,
        high: 0
    };
    struct pitchbend_value pitchbend = read_pitchbend();

    // if (pitchbend.low != last_pitchbend.low || pitchbend.high != last_pitchbend.high) {
        tudi_midi_write24(cable_num, 0xe0 | channel, pitchbend.low, pitchbend.high);
    // }

    // static uint16_t last_adc_value = 0;
    // adc_select_input(2);
    // uint16_t adc_value = adc_read();

    // if (adc_value != last_adc_value) {
    //     uint8_t pitch_bend_low = 0x7f & (adc_value);
    //     uint8_t pitch_bend_high = 0x7f & (adc_value >> 7) + 0x40;

    //     tudi_midi_write24(cable_num, 0xe0 | channel, pitch_bend_low, pitch_bend_high);
    // }

    // last_adc_value = adc_value;

    // Poll every 10ms TODO: remove this code?
    // static uint32_t start_ms = 0;
    // const uint32_t interval_ms = 1000;
    // if (board_millis() - start_ms < interval_ms) return; // not enough time
    // start_ms += interval_ms;

    // static velocity = 0;

    // velocity = velocity ? 0 : 127;
    // tudi_midi_write24(cable_num, 0x90 | channel, 10, velocity);
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // Blink every interval ms
    // if (board_millis() - start_ms < blink_interval_ms) return; // not enough time
    // start_ms += blink_interval_ms;

    // board_led_write(led_state);
    // led_state = 1 - led_state; // toggle
}
