#include "util.h"

uint16_t clamp_u16(uint16_t v, uint16_t min_v, uint16_t max_v) {
  if (v < min_v) {
    return min_v;
  }
  if (v > max_v) {
    return max_v;
  }
  return v;
}
