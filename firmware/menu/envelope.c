#include <stddef.h>

#include "../adsr.h"
#include "ui_menu.h"

menu_item_t mi_attack = {
    .label = "Attack",
    .action = NULL,

};

menu_item_t mi_decay = {
    .label = "Decay",
    .action = NULL,

};

menu_item_t mi_sustain = {
    .label = "Sustain",
    .action = NULL,
};

menu_item_t mi_release = {
    .label = "Release",
    .action = NULL,
};

menu_item_t* envelope_menu_items[] = {
    &mi_attack, &mi_decay, &mi_sustain, &mi_release, NULL,
};

void push_menu_envelope() { push_menu(4, envelope_menu_items); }