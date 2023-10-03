#include "keyboard.h"


#include "bsp/board.h"

#include "midi.h"
#include "display.h"
#include "settings.h"
#include "midi_note_table.h"
#include "main_host.h"

void process_kbd_report(hid_keyboard_report_t const *p_new_report);

void keyboard_init() {

}

void keyboard_task() {
  keyboard_event_message msg;

  while(queue_try_remove(&keyboard_event_queue, &msg)){
    switch(msg.type) {
    case KEYBOARD_CONNECTED:
      display_debug("KBD CONN");
      break;
    case KEYBOARD_DISCONNECTED:
      display_debug("KBD DC");
      break;
    case KEYBOARD_REPORT:
      process_kbd_report(&msg.report);
      break;
    default:
      display_debug("UNKNOWN MSG");
      break;
    }
  }
}

void handle_key(uint8_t key, bool pressed) {
#ifdef DEBUG_KEYS
    char msg[16];
    if (pressed) {
        sprintf(msg, "KEY %d %d PRE", key, keycode2midi[key]);
    } else {
        sprintf(msg, "KEY %d %d REL", key, keycode2midi[key]);
    }
    display_debug(msg);
#endif

    if (pressed) {
        if (key >= 58 && key <= 69) {
            program = key - 58;
            midi_write2(MIDI_PROGRAM_CHANGE, program);
        }
        if (key == 75) {
            int16_t max_note = 67 + offset + 12;
            if (max_note <= 127) {
                clear_notes();
                offset += 12;
            }
        } else if (key == 78) {
            int16_t min_note = 35 + offset - 12;
            if (min_note > 0) {
                clear_notes();
                offset -= 12;
            }
        } else if (key == 81) {
            int16_t min_note = 35 + offset - 1;
            if (min_note > 0) {
                clear_notes();
                offset--;
            }
        } else if (key == 82) {
            int16_t max_note = 67 + offset + 1;
            if (max_note <= 127) {
                clear_notes();
                offset++;
            }
        }
    }

    uint8_t raw_note = key < 128 ? keycode2midi[key] : 0;
    if (raw_note != 0) {
        if (pressed) {
            midi_write3(MIDI_NOTE_ON, raw_note + offset, 127);
        } else {
            midi_write3(MIDI_NOTE_ON, raw_note + offset, 0);
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
