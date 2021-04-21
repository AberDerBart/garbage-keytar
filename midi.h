#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_PITCHBEND 0xe0

void midi_init();

void midi_write(uint8_t cmd, uint8_t b1, uint8_t b2);

#endif