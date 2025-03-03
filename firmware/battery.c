#include "battery.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"

#define CHARGING_PIN 14
#define STANDBY_PIN 15
#define VOLTAGE_PIN 26

#define VOLTAGE_ADC_INPUT 0

#define MAX_ADC_VALUE ((1 << 12) - 1)

void battery_init() {
  gpio_init(CHARGING_PIN);
  gpio_set_dir(CHARGING_PIN, GPIO_IN);
  gpio_pull_up(CHARGING_PIN);

  gpio_init(STANDBY_PIN);
  gpio_set_dir(STANDBY_PIN, GPIO_IN);
  gpio_pull_up(STANDBY_PIN);

  adc_init();
  adc_gpio_init(VOLTAGE_PIN);
}

bool battery_is_charging() { return !gpio_get(CHARGING_PIN); }

bool battery_is_fully_charged() { return !gpio_get(STANDBY_PIN); }

float battery_get_voltage() {
  adc_select_input(VOLTAGE_ADC_INPUT);

  uint16_t adc_value = adc_read();
  float input_voltage = 3.3 * adc_value / MAX_ADC_VALUE;
  return input_voltage * 2.;
}

battery_level_t battery_get_level() {
  float batt_voltage = battery_get_voltage();

  if (batt_voltage < 3.2) {
    return EMPTY;
  } else if (batt_voltage < 3.83) {
    return LOW;
  } else if (batt_voltage < 4.03) {
    return MEDIUM;
  }

  return FULL;
}