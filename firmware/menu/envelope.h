#pragma once
#include "adsr.h"
#include "ui/menu.h"

void menu_add_envelope_entries(ui_menu_t* menu, adsr_settings_t* envelope);

void push_menu_envelope();