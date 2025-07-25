#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "./keymap.h"
#include "ui_stack.h"

void bluetooth_action(menu_item_t* _) { push_menu_bluetooth(); }
menu_item_t m_i_bluetooth = {
    .label = "Bluetooth",
    .action = bluetooth_action,
};

void keymap_action(menu_item_t* _) { push_menu_keymap(); }
menu_item_t m_i_keymap = {
    .label = "Keymap",
    .action = keymap_action,
};

void envelope_action(menu_item_t* _) { push_menu_envelope(); }
menu_item_t m_i_envelope = {
    .label = "Envelope",
    .action = envelope_action,
};

menu_item_t* main_menu_items[] = {
    &m_i_bluetooth,
    &m_i_keymap,
    &m_i_envelope,
};

void push_menu_main() { push_menu(3, main_menu_items); }