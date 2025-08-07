#pragma once

#include <stdint.h>

void midi_note_on(uint8_t note);
void midi_note_off(uint8_t note);

void midi_program_change(uint8_t program);

void midi_clear_notes();

void midi_pitchbend(uint8_t low, uint8_t high);

void midi_cc_attack(uint8_t attack);
void midi_cc_decay(uint8_t decay);
void midi_cc_sustain(uint8_t sustain);
void midi_cc_release(uint8_t release);
