#pragma once
#include "ui_element.h"

void ui_init(ssd1306_t* display, ui_element_t* element);

void ui_push(ui_element_t* element);
void ui_pop();
void ui_clear();

void ui_render();
void ui_navigate(ui_nav_t nav);
