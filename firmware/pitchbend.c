#include "pitchbend.h"

#include "bsp/board.h"
#include "hardware/adc.h"
#include "midi.h"

#define PITCHBEND_ADC_INPUT 2
#define PITCHBEND_PIN 28

#define CENTER_VALUE_MIN 2040
#define CENTER_VALUE_MAX 2060

#define MIN_VALUE 20
#define MAX_VALUE 3900

#define OFF_THRESHOLD_VALUE 4000

// use a uint16_t to store a uint14_t
typedef uint16_t uint14_t;

#define UINT14_MIN 0
#define UINT14_MAX 16384

#define PITCHBEND_CENTER 8192

struct pitchbend_value {
  uint8_t high;
  uint8_t low;
};

static struct pitchbend_value last_pitchbend;

void pitchbend_init() {
  adc_init();
  adc_gpio_init(PITCHBEND_PIN);

  last_pitchbend = to_pitchbend(PITCHBEND_CENTER);
}

struct pitchbend_value to_pitchbend(uint16_t value) {
  struct pitchbend_value result = {
    low : value & 0x7f,
    high : (value >> 7) & 0x7f
  };

  return result;
}

struct pitchbend_value pitchbend_read() {
  adc_select_input(PITCHBEND_ADC_INPUT);

  uint32_t adc_value = adc_read();

  // if pitchbend is not pressed, it is pulled high, so send centered pitchbend
  // NOTE: if picthbend is pulled all the way up, it is still beneath the
  // threshold as there is an additional resistor in place, see schematic
  if (adc_value > OFF_THRESHOLD_VALUE) {
    return to_pitchbend(PITCHBEND_CENTER);
  }

  if (adc_value < MIN_VALUE) {
    // max bend down
    return to_pitchbend(UINT14_MIN);
  }

  if (adc_value > MAX_VALUE) {
    // max bend up
    return to_pitchbend(UINT14_MAX);
  }

  if (adc_value < CENTER_VALUE_MIN) {
    // bending down, adc_value is between MIN_VALUE and CENTER_VALUE_MIN
    uint32_t diff_min = adc_value - MIN_VALUE;
    uint32_t pitchbend_range = PITCHBEND_CENTER - UINT14_MIN;
    uint32_t adc_range = CENTER_VALUE_MIN - MIN_VALUE;
    uint32_t v = diff_min * pitchbend_range / adc_range;

    return to_pitchbend(v);
  }

  if (adc_value > CENTER_VALUE_MAX) {
    // bending up, adc_value is between CENTER_VALUE_MAX and MAX_VALUE
    uint32_t diff_center = adc_value - MIN_VALUE;
    uint32_t pitchbend_range = UINT14_MAX - PITCHBEND_CENTER;
    uint32_t adc_range = CENTER_VALUE_MIN - MIN_VALUE;
    uint32_t v = diff_center * pitchbend_range / adc_range;

    return to_pitchbend(v);
  }

  // not bending, adc_value is between CENTER_VALUE_MIN and CENTER_VALUE_MAX
  return to_pitchbend(PITCHBEND_CENTER);
}

void pitchbend_task() {
  struct pitchbend_value pitchbend = pitchbend_read();

  if (last_pitchbend.low != pitchbend.low ||
      last_pitchbend.high != pitchbend.high) {
    midi_pitchbend(pitchbend.low, pitchbend.high);
  }

  last_pitchbend = pitchbend;
}
