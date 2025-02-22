#pragma once

#include <stdint.h>

typedef struct {
  uint8_t attack;
  uint8_t decay;
  uint8_t sustain;
  uint8_t release;
} adsr_settings_t;

extern adsr_settings_t adsr_settings;

char* adsr_menu_get_attack();
char* adsr_menu_get_decay();
char* adsr_menu_get_sustain();
char* adsr_menu_get_release();

void change_attack_setting(int8_t steps);
void change_decay_setting(int8_t steps);
void change_sustain_setting(int8_t steps);
void change_release_setting(int8_t steps);