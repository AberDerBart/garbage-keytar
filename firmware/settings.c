#include "settings.h"

uint8_t program = 0;

int8_t offset = 0;

int8_t offset_get_octave() {
    if (offset >= 0) {
        return offset / 12;
    } else {
        return (offset - 11) / 12;
    }
}

uint8_t offset_get_trans() {
    if (offset >= 0) {
        return offset % 12;
    } else {
        return (12 + offset % 12) % 12;
    }
}
