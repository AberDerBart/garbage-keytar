#include "analog_strip.h"
#include "hardware/adc.h"
#include <stdio.h>

#define STRIP_ADC_INPUT 2
#define STRIP_PIN 28

#define MIN_VALUE 200
#define MAX_VALUE 3600

#define OFF_THRESHOLD_VALUE 4000

void analog_strip_init() {
  adc_init();
  adc_gpio_init(STRIP_PIN);
}

bool analog_strip_read_uint16(uint16_t* value) {
  adc_select_input(STRIP_ADC_INPUT);
  uint32_t adc_value = adc_read();

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
