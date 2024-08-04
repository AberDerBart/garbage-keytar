#include "keyboard.h"


#include "midi.h"
#include "settings.h"
#include "midi_note_table.h"
#include "main_host.h"
#include "pico/flash.h"
#include "pico/multicore.h"
#include "keyboard_matrix.h"

#include "display.h"

// core1: handle host events
extern void core1_main();

void process_kbd_report(hid_keyboard_report_t const *p_new_report);

void keyboard_init() {
  printf("init keyboard\n");
  tusb_init();
}

void keyboard_task() {
  tuh_task();
}

void handle_key(uint8_t key, bool pressed) {
    if(pressed) {
      printf("key %d pressed\n", key);
    }else{
      printf("key %d released\n", key);
    }

    if (pressed) {
	if (key >= 58 && key <= 61) {
                midi_clear_notes();
		set_key_mapping(key-58);
	} else if (key >= 58 && key <= 69) {
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

    uint8_t raw_note = key_mapping->lookup(key);
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

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    /* Ask for a report only if this is a keyboard device */
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
    if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
        printf("USB keyboard connected\n");
        display_get_state_for_update()->keyboard = true;
        tuh_hid_receive_report(dev_addr, instance);
    } else {
        printf("unkonwn USB device connected\n");
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
