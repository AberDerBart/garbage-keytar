#include "analog_strip.h"
#include "hardware/adc.h"
#include <stdio.h>

#define STRIP_ADC_INPUT 2
#define STRIP_PIN 28

#define MIN_VALUE 200
#define MAX_VALUE 3600

#define OFF_THRESHOLD_VALUE 4000

midi_value* analog_strip_midi_value = NULL;
bool reset_on_release = false;

void analog_strip_init() {
  adc_init();
  adc_gpio_init(STRIP_PIN);
}

void analog_strip_assign(midi_value* mv, bool reset) {
  if (analog_strip_midi_value) {
    if (reset_on_release) {
      midi_value_set(analog_strip_midi_value, analog_strip_midi_value->default_value, false);
    }
  }
  analog_strip_midi_value = mv;
  reset_on_release = reset;
}

bool analog_strip_read_uint16(uint16_t* value) {
  adc_select_input(STRIP_ADC_INPUT);
  uint32_t adc_value = adc_read();
  printf("Strip ADC: %d\n", adc_value);

  if (adc_value > OFF_THRESHOLD_VALUE) {
    return false;
  }

  if (adc_value > MAX_VALUE) {
    *value = UINT16_MAX;
    return true;
  }
  if (adc_value < MIN_VALUE) {
    *value = 0;
    return true;
  }

  *value = (adc_value - MIN_VALUE) * UINT16_MAX / (MAX_VALUE - MIN_VALUE);
  return true;
}

bool analog_strip_read_float(float* value) {
  uint32_t adc_value = adc_read();

  if (adc_value > OFF_THRESHOLD_VALUE) {
    return false;
  }
  if (adc_value > MAX_VALUE) {
    *value = 1.0;
    return true;
  }
  if (adc_value < MIN_VALUE) {
    *value = 0.;
    return true;
  }

  *value = ((float)(adc_value - MIN_VALUE)) / ((float)(MAX_VALUE - MIN_VALUE));

  return true;
}

bool analog_strip_read(midi_value* mv, bool reset_on_release) {
  float v = -1.;
  if (!analog_strip_read_float(&v)) {
    if (reset_on_release) {
      midi_value_set(mv, mv->default_value, false);
    }
    return false;
  } 

  uint16_t value = mv->min_value + ((mv->max_value - mv->min_value) * v);
  midi_value_set(mv, value, false);
  return true;
}

void analog_strip_task() {
  if (analog_strip_midi_value){
    analog_strip_read(analog_strip_midi_value, reset_on_release);
  }
}
