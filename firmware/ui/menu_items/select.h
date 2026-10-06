#pragma once

#include "../element.h"

ui_element_t* ui_menu_item_select_new(char* label, size_t value, char** options,
                                      void (*on_change)(size_t));