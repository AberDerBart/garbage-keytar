#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CONTROL_CHANGE 0xb0
#define MIDI_PROGRAM_CHANGE 0xc0
#define MIDI_PITCHBEND 0xe0

#define MIDI_CONTROL_ALL_NOTES_OFF_B1 123
#define MIDI_CONTROL_ALL_NOTES_OFF_B2 0

#define MIDI_CONTROL_SUSTAIN 64
#define MIDI_CONTROL_SOSTENUTO 66

#define MIDI_CONTROL_VALUE_ON 64
#define MIDI_CONTROL_VALUE_OFF 0

#define DEBUG_MIDI

void midi_init();

void midi_write3(uint8_t cmd, uint8_t b1, uint8_t b2);
void midi_write2(uint8_t cmd, uint8_t b1);

void clear_notes();

#endif
