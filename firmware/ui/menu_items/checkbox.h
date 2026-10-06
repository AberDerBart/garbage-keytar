#pragma once

#include "../menu_items.h"

ui_element_t* ui_menu_item_checkbox_new(char* label, bool initial_value, void (*on_change)(bool newVal));

