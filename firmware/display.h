#pragma once
#include <stdbool.h>

#include "battery.h"
#include "keyboard_matrix.h"
#include "pico-ssd1306/ssd1306.h"

void display_init();

void display_task();

ssd1306_t *get_display();
