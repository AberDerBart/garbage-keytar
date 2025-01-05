#include "pitchbend.h"

#include "bsp/board.h"
#include "hardware/adc.h"
#include "midi.h"

struct pitchbend_value {
  uint8_t high;
  uint8_t low;
};

void pitchbend_init() {
  adc_init();
  adc_gpio_init(28);
}

struct pitchbend_value to_pitchbend(int16_t value) {
  if (value > (1 << 13) - 1) {
    return to_pitchbend((1 << 13) - 1);
  }

  if (value < -(1 << 13)) {
    return to_pitchbend(-(1 << 13));
  }

  uint16_t offset_value = value + 0x2000;

  struct pitchbend_value result = {
    low : offset_value & 0x7f,
    high : (offset_value >> 7) & 0x7f
  };

  return result;
}

struct pitchbend_value pitchbend_read() {
  adc_select_input(2);
  uint16_t const ACTIVATION_THRESHOLD = 4000;
  uint16_t const MAX_VALUE = 0x0fff;
  uint32_t const ADC_RANGE = 2048;
  uint16_t const INACTIVE = 0xffff;

  static uint32_t start_value = INACTIVE;

  uint32_t adc_value = adc_read();

  if (start_value == INACTIVE) {
    if (adc_value < ACTIVATION_THRESHOLD) {
      start_value = adc_read();
      return to_pitchbend(0);
    }
  } else {
    if (adc_value > ACTIVATION_THRESHOLD) {
      start_value = INACTIVE;
      return to_pitchbend(0);
    } else {
      int32_t value =
          (int32_t)(1 << 14) * (adc_value - start_value) / ADC_RANGE;
      return to_pitchbend(value);
    }
  }
  // just in case...
  return to_pitchbend(0);
}

void pitchbend_task() {
  uint8_t const channel = 0;

  static uint32_t start_ms = 0;
  const uint32_t interval_ms = 10;
  if (board_millis() - start_ms < interval_ms) {
    return;
  }
  start_ms += interval_ms;

  static struct pitchbend_value last_pitchbend = {low : 0, high : 0};
  struct pitchbend_value pitchbend = pitchbend_read();

  if (last_pitchbend.low != pitchbend.low ||
      last_pitchbend.high != pitchbend.high) {
    midi_write3(0xe0 | channel, pitchbend.low, pitchbend.high);
  }

  last_pitchbend.low = pitchbend.low;
  last_pitchbend.high = pitchbend.high;
}
