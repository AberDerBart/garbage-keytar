#include "keyboard_matrix.h"
#include "key_positions.h"

uint8_t keycode_to_note(uint8_t keycode, uint8_t offset, int8_t col_step, int8_t row_step) {
	if (keycode >= 128) {
		return 0;
	}
	int8_t row = keycode2row[keycode];
	int8_t col = keycode2column[keycode];

	if (row < 0 || col < 0) {
		return 0;
	}

	return col * col_step + row * row_step;
}

uint8_t accordion_c(uint8_t keycode) {
	return keycode_to_note(keycode, 32, 3, -2);
}

uint8_t accordion_b(uint8_t keycode) {
	return keycode_to_note(keycode, 32, 3, -1);
}

uint8_t accordion_6plus6(uint8_t keycode) {
	return keycode_to_note(keycode, 32, 2, -1);
}

uint8_t wicki_hayden(uint8_t keycode) {
	return keycode_to_note(keycode, 32, 2, 7);
}
