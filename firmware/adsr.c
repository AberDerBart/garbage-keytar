#include "adsr.h"

#include <stdio.h>

adsr_settings_t adsr_settings;

char menu_setting_buffer[5] = {0, 0, 0, 0, 0};

char* get_setting_seconds(float scale, uint8_t value) {
  float value_f = scale * ((float)value / (float)UINT8_MAX);

  snprintf(menu_setting_buffer, 5, "%.2f", value_f);

  return menu_setting_buffer;
}

char* adsr_menu_get_attack() {
  return get_setting_seconds(2.0, adsr_settings.attack);
}

char* adsr_menu_get_decay() {
  return get_setting_seconds(2.0, adsr_settings.decay);
}

char* adsr_menu_get_sustain() {
  snprintf(menu_setting_buffer, 5, "%d", adsr_settings.sustain);
  return menu_setting_buffer;
}

char* adsr_menu_get_release() {
  return get_setting_seconds(2.0, adsr_settings.release);
}

void change_setting(uint8_t* setting, int8_t steps, uint8_t max) {
  int16_t tmp = *setting + steps;
  if (tmp > max) {
    tmp = max;
  } else if (tmp < 0) {
    tmp = 0;
  }

  *setting = tmp;
}

void change_attack_setting(int8_t steps) {
  change_setting(&adsr_settings.attack, steps, UINT8_MAX);
}

void change_decay_setting(int8_t steps) {
  change_setting(&adsr_settings.decay, steps, UINT8_MAX);
}

void change_sustain_setting(int8_t steps) {
  change_setting(&adsr_settings.sustain, steps, 127);
}

void change_release_setting(int8_t steps) {
  change_setting(&adsr_settings.release, steps, UINT8_MAX);
}
