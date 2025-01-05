#include <menu.h>
#include <stddef.h>

#include "display.h"

menu_item_t menu_item_main;

menu_item_t menu_item_bluetooth = {
  label : "Bluetooth",
  parent : &menu_item_main,
  children : NULL,
};

menu_item_t menu_item_keymap;

menu_item_t menu_item_keymap_piano = {
  label : "Piano",
  parent : &menu_item_keymap,
  children : NULL,
};

menu_item_t menu_item_keymap = {
  label : "Keymap",
  parent : &menu_item_main,
  children : {&menu_item_keymap_piano, NULL},
};

menu_item_t menu_item_main = {
  label : NULL,
  parent : NULL,
  children : {
      &menu_item_bluetooth,
      &menu_item_keymap,
      NULL,
  },
};

uint8_t menu_len(menu_item_t* item) {
  if (!item->children) {
    return 0;
  }

  uint8_t index = 0;
  while (item->children[index]) {
    if (index == 255) {
      return 0;
    }
    index++;
  }
  return index;
}

menu_state_t* get_menu_state() {
  return &display_get_state_for_update()->menu_state;
}

void menu_open() {
  menu_state_t* state = get_menu_state();
  if (state->current) {
    return;
  }

  state->current = &menu_item_main;
  state->index = 0;
};

void menu_close() {
  menu_state_t* state = get_menu_state();
  state->current = NULL;
  state->index = 0;
};

void menu_toggle() {
  if (get_menu_state()->current) {
    menu_close();
  } else {
    menu_open();
  }
}

void menu_prev() {
  menu_state_t* state = get_menu_state();
  if (!state->current) {
    return;
  }
  uint8_t len = menu_len(state->current);
  if (len == 0) {
    return;
  }

  if (state->index == 0) {
    state->index = len - 1;
  } else {
    state->index -= 1;
  }
};

void menu_next() {
  menu_state_t* state = get_menu_state();
  if (!state->current) {
    return;
  }

  uint8_t len = menu_len(state->current);
  if (len == 0) {
    return;
  }

  if (state->index == len - 1) {
    state->index = 0;
  } else {
    state->index += 1;
  }
};

void menu_parent() {
  menu_state_t* state = get_menu_state();
  if (!state->current) {
    return;
  }
  if (!state->current->parent) {
    menu_close();
    return;
  };
  state->current = state->current->parent;
  state->index = 0;
}

void menu_select() {
  menu_state_t* state = get_menu_state();
  if (!state->current) {
    return;
  }

  menu_item_t* target = state->current->children[state->index];
  if (menu_len(target)) {
    state->current = target;
    state->index = 0;
  }
}
