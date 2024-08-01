#include "settings.h"
#include "display.h"

uint8_t program = 0;

int8_t offset = 0;

const key_mapping_t* key_mapping = &(KEY_MAPPINGS[0]);

void set_key_mapping(uint8_t index) {
	if (index > 4) {
		return;
	}
	key_mapping = &KEY_MAPPINGS[index];
	display_state_t* display_state = display_get_state_for_update();
	display_state->key_mapping = key_mapping;
}

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
