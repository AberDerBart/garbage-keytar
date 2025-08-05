#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "./keymap.h"
#include "ui_stack.h"

void bluetooth_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    push_menu_bluetooth();
  }
}
menu_item_t m_i_bluetooth = {
  label : "Bluetooth",
  navigate : bluetooth_nav,
  free : NULL,
};

void keymap_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    push_menu_keymap();
  }
}
menu_item_t m_i_keymap = {
  label : "Keymap",
  navigate : keymap_nav,
  free : NULL,
};

void envelope_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    push_menu_envelope();
  }
}
menu_item_t m_i_envelope = {
  label : "Envelope",
  navigate : envelope_nav,
  free : NULL,
};

menu_item_t* main_menu_items[] = {
    &m_i_bluetooth,
    &m_i_keymap,
    &m_i_envelope,
    NULL,
};

void push_menu_main() { push_menu(main_menu_items); }