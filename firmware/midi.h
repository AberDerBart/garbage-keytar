#pragma once

#include <stdint.h>
#include <stdbool.h>

bool midi_note_on(uint8_t note);
bool midi_note_off(uint8_t note);

bool midi_program_change(uint8_t program);

bool midi_clear_notes();

bool midi_pitchbend(uint8_t low, uint8_t high);

bool midi_cc_mod(uint8_t mod);

bool midi_cc_attack(uint8_t attack);
bool midi_cc_decay(uint8_t decay);
bool midi_cc_sustain(uint8_t sustain);
bool midi_cc_release(uint8_t release);
