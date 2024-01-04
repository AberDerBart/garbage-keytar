#ifndef MAIN_HOST_H
#define MAIN_HOST_H

#include "tusb.h"
#include "pico/util/queue.h"


typedef enum {
  UNKNOWN = 0,
  USB_READY = 1,
  KEYBOARD_CONNECTED = 2,
  KEYBOARD_DISCONNECTED = 3,
  KEYBOARD_REPORT = 4,
} keyboard_event_message_type;

typedef struct {
  keyboard_event_message_type type;
  hid_keyboard_report_t report;
  bool read;
} keyboard_event_message_t;

extern keyboard_event_message_t keyboard_event_message;

extern mutex_t keyboard_event_mutex;

#endif
