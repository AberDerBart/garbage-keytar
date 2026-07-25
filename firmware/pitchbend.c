#include "pitchbend.h"

#include "analog_strip.h"
#include "midi.h"

// use a uint16_t to store a uint14_t
typedef uint16_t uint14_t;

#define UINT14_MIN 0
#define UINT14_MAX 0x3fff

#define UINT16_CENTER 0x8000

struct pitchbend_value {
  uint8_t high;
  uint8_t low;
};

static struct pitchbend_value last_pitchbend;

struct pitchbend_value to_pitchbend(uint16_t value) {
  struct pitchbend_value result = {
    .low =  (value >> 2) & 0x7f,
    .high =  (value >> 9) & 0x7f
  };

  return result;
}

void pitchbend_init() { last_pitchbend = to_pitchbend(UINT16_CENTER); }

struct pitchbend_value pitchbend_read() {
  uint16_t read_value;
  if (analog_strip_read_uint16(&read_value)) {
    return to_pitchbend(read_value);
  }
  return to_pitchbend(UINT16_CENTER);
}

void pitchbend_task() {
  struct pitchbend_value pitchbend = pitchbend_read();

  if (last_pitchbend.low != pitchbend.low ||
      last_pitchbend.high != pitchbend.high) {
    if(midi_pitchbend(pitchbend.low, pitchbend.high)) {
      last_pitchbend = pitchbend;
    }
  }

}
