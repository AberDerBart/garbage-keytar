#include "keyboard.h"


#include "bsp/board.h"

#include "midi.h"
#include "display.h"
#include "settings.h"
#include "midi_note_table.h"

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
