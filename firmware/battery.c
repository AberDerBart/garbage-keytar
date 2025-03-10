#include "battery.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"

#define CHARGING_PIN 14
#define STANDBY_PIN 15
#define VOLTAGE_PIN 26

#define VOLTAGE_ADC_INPUT 0

#define MAX_ADC_VALUE ((1 << 12) - 1)

#define VOLTAGE_THRESHOLD_LOW 3.2
#define VOLTAGE_THRESHOLD_MEDIUM 3.6
#define VOLTAGE_THRESHOLD_FULL 3.9

#define VOLTAGE_HISTERESIS 0.07

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

const float voltage_thresholds[3] = {
    VOLTAGE_THRESHOLD_LOW,
    VOLTAGE_THRESHOLD_MEDIUM,
    VOLTAGE_THRESHOLD_FULL,
};

battery_level_t battery_get_level() {
  static float last_level = FULL;

  // shortcut if charging circuit reports fully charged
  if (battery_is_fully_charged()) {
    last_level = FULL;
    return FULL;
  }

  float v = battery_get_voltage();

  for (battery_level_t target = EMPTY; target < FULL; target++) {
    // if we are below the threshold of the next level, return target level
    if (v < voltage_thresholds[target + 1]) {
      last_level = target;
      return target;
    }
    // if charging, VOLTAGE_HISTERESIS has to be surpassed as well
    if (last_level == target &&
        v < voltage_thresholds[target + 1] + VOLTAGE_HISTERESIS) {
      last_level = target;
      return target;
    }
  }

  // if the level is above anything above, return FULL
  last_level = FULL;
  return FULL;
}