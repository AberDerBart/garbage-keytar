#include "keyboard.h"


#include "bsp/board.h"

#include "midi.h"
#include "settings.h"
#include "midi_note_table.h"
#include "main_host.h"

void process_kbd_report(hid_keyboard_report_t const *p_new_report);

void keyboard_init() {
  printf("init keyboard\n");
  mutex_init(&keyboard_event_mutex);
}

void keyboard_task() {
  keyboard_event_message_t message;

  mutex_enter_blocking(&keyboard_event_mutex);

  if(keyboard_event_message.read) {
    mutex_exit(&keyboard_event_mutex);
    return;
  }

  memcpy(&message, &keyboard_event_message, sizeof(keyboard_event_message_t));
  keyboard_event_message.read = true;

  mutex_exit(&keyboard_event_mutex);

  switch(message.type) {
    case KEYBOARD_CONNECTED:
      printf("keyboard connected\n");
      break;
    case KEYBOARD_DISCONNECTED:
      printf("keyboard disconnected\n");
      break;
    case KEYBOARD_REPORT:
      process_kbd_report(&message.report);
      break;
    default:
      printf("unknown keyboard message\n");
      break;
  }
}

void handle_key(uint8_t key, bool pressed) {
    if(pressed) {
      printf("key %d pressed\n", key);
    }else{
      printf("key %d released\n", key);
    }

    if (pressed) {
        if (key >= 58 && key <= 69) {
            program = key - 58;
            midi_program_change(program);
        }
        if (key == 75) {
            int16_t max_note = 67 + offset + 12;
            if (max_note <= 127) {
                midi_clear_notes();
                offset += 12;
            }
        } else if (key == 78) {
            int16_t min_note = 35 + offset - 12;
            if (min_note > 0) {
                midi_clear_notes();
                offset -= 12;
            }
        } else if (key == 81) {
            int16_t min_note = 35 + offset - 1;
            if (min_note > 0) {
                midi_clear_notes();
                offset--;
            }
        } else if (key == 82) {
            int16_t max_note = 67 + offset + 1;
            if (max_note <= 127) {
                midi_clear_notes();
                offset++;
            }
        }
    }

    uint8_t raw_note = key < 128 ? keycode2midi[key] : 0;
    if (raw_note != 0) {
        if (pressed) {
            midi_note_on(raw_note + offset);
        } else {
            midi_note_off(raw_note + offset);
        }
    }
}

static inline bool find_key_in_report(hid_keyboard_report_t const *p_report, uint8_t keycode) {
    for (uint8_t i = 0; i < 6; i++) {
        if (p_report->keycode[i] == keycode)
            return true;
    }

    return false;
}

void process_kbd_report(hid_keyboard_report_t const *p_new_report) {
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
