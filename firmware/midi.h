#pragma once

#include <stdint.h>

void midi_note_on(uint8_t note);
void midi_note_off(uint8_t note);

void midi_program_change(uint8_t program);

void midi_clear_notes();

void midi_pitchbend(uint8_t low, uint8_t high);
