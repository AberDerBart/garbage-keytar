#include "control.h"

#include "hardware/adc.h"

void control_init() {
  adc_init();
  adc_gpio_init(27);
  adc_gpio_init(26);
}

uint8_t read_control(uint8_t adc_index) {
  adc_select_input(adc_index);

  uint16_t value = adc_read();
  return 0x7f & (value >> 5);
}

void control_task() {
  static uint32_t start_ms = 0;
  const uint32_t interval_ms = 10;
  if (board_millis() - start_ms < interval_ms) {
    return;
  }
  start_ms += interval_ms;

  static uint8_t reported_values[2] = {0, 0};
  static uint8_t old_values[2] = {0, 0};

  for (uint8_t i = 0; i < 2; i++) {
    uint8_t val = read_control(i);

    if (abs(val - reported_values[i]) > 1 ||
        val != reported_values[i] && old_values[i] != reported_values[i]) {
      midi_write3(0xb0, i, val);
      reported_values[i] = val;
    }

    old_values[i] = val;
  }
}
