#include "keyboard_matrix.h"
#include "key_positions.h"

uint8_t piano_lut[4][14] = {
	{0,  0, 49, 51,  0, 54, 56, 58, 61, 63,  0, 66, 68,  0},
	  {0, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69},
	    {0,  0, 37, 39,  0, 42, 44, 46,  0, 49, 51,  0,  0,  0},
	      {0, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52,  0,  0,  0},
};

uint8_t note_by_lut(uint8_t keycode, uint8_t lut[4][14]) {
	if (keycode >= 128) {
		return 0;
	}
	int8_t row = keycode2row[keycode];
	int8_t col = keycode2column[keycode];

	if (row < 1 || col < 0 || row > 4 || col) {
		return 0;
	}

	return lut[4-row][col];
}

uint8_t note_by_pattern(uint8_t keycode, uint8_t offset, int8_t col_step, int8_t row_step) {
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

uint8_t piano(uint8_t keycode) {
	return note_by_lut(keycode, piano_lut);
}

uint8_t accordion_c(uint8_t keycode) {
	return note_by_pattern(keycode, 32, 3, -2);
}

uint8_t accordion_b(uint8_t keycode) {
	return note_by_pattern(keycode, 32, 3, -1);
}

uint8_t accordion_6plus6(uint8_t keycode) {
	return note_by_pattern(keycode, 32, 2, -1);
}

uint8_t wicki_hayden(uint8_t keycode) {
	return note_by_pattern(keycode, 32, 2, 7);
}
