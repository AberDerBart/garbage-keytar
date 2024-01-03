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

#include "main_host.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include "pico/platform.h"

#include "pio_usb.h"
#include "tusb.h"
#include "usb_descriptors.h"

keyboard_event_message_t keyboard_event_message;

mutex_t keyboard_event_mutex;

void set_keyboard_event_message(keyboard_event_message_type type, hid_keyboard_report_t const * report) {
  mutex_enter_blocking(&keyboard_event_mutex);

  keyboard_event_message.read = false;
  keyboard_event_message.type = type;
  if(report){
    memcpy(&(keyboard_event_message.report), report, sizeof(hid_keyboard_report_t));
  }

  mutex_exit(&keyboard_event_mutex);
}

void send_connected() {
  set_keyboard_event_message(KEYBOARD_CONNECTED, NULL);
}

void send_disconnected() {
  set_keyboard_event_message(KEYBOARD_DISCONNECTED, NULL);
}

void send_report(hid_keyboard_report_t const * report) {
  set_keyboard_event_message(KEYBOARD_REPORT, report);
}

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+



/*------------- MAIN -------------*/

#define PIO_USB_DP_PIN 2
// NOTE: The D- pin is hardwired to the configured D+ pin + 1

#define PIO_USB_CONFIG \
  { \
    PIO_USB_DP_PIN, PIO_USB_TX_DEFAULT, PIO_SM_USB_TX_DEFAULT, \
        PIO_USB_DMA_TX_DEFAULT, PIO_USB_RX_DEFAULT, PIO_SM_USB_RX_DEFAULT, \
        PIO_SM_USB_EOP_DEFAULT, NULL, PIO_USB_DEBUG_PIN_NONE, \
        PIO_USB_DEBUG_PIN_NONE, .skip_alarm_pool = false \
  }

// core1: handle host events
void core1_main() {
  sleep_ms(10);

  // Use tuh_configure() to pass pio configuration to the host stack
  // Note: tuh_configure() must be called before
  pio_usb_configuration_t pio_cfg = PIO_USB_CONFIG;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);

  // To run USB SOF interrupt in core1, init host stack for pio_usb (roothub
  // port1) on core1
  tuh_init(1);

  while (true) {
    tuh_task(); // tinyusb host task
  }
}

//--------------------------------------------------------------------+
// Host HID
//--------------------------------------------------------------------+
uint8_t connected_kbd_address = 0;

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
  (void) desc_report;
  (void) desc_len;
  (void) dev_addr;
  (void) instance;

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
    connected_kbd_address = dev_addr;
    send_connected();
    if (!tuh_hid_receive_report(dev_addr, instance) ) {
      tud_cdc_write_str("Error: cannot request report\r\n");
    }
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  (void) instance;

  if (dev_addr == connected_kbd_address) {
    send_disconnected();
    connected_kbd_address = 0;
  }
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
  (void) len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch(itf_protocol) {
    case HID_ITF_PROTOCOL_KEYBOARD:
      send_report((hid_keyboard_report_t const *) report);
      break;
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) ) {
    send_disconnected();
    //tud_cdc_write_str("Error: cannot request report\r\n");
  }
}

