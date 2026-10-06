#pragma once

#include "../element.h"

ui_element_t* ui_menu_item_select_new(char* label, void** options, size_t initial_index, char* (*get_option_label)(void* option),
                                      void (*on_change)(void* option));
