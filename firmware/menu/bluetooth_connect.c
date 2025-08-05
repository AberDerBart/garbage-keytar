#include "./bluetooth_connect.h"

#include <stdlib.h>

#include "midi_ble_client.h"
#include "ui/menu.h"
#include "ui/menu_items.h"
#include "ui_stack.h"

typedef struct bluetooth_device_menu_item_t {
  ui_menu_item_default_t base;
  uint8_t index;
} bluetooth_device_menu_item_t;

void bluetooth_connect_nav(ui_element_t* mi, ui_nav_t nav) {
  if (nav != ENTER) {
    return;
  }
  bluetooth_device_menu_item_t* self = (bluetooth_device_menu_item_t*)mi;
  midi_ble_client_connect(self->index);
  ui_clear();
}

void bluetooth_connect_free(ui_element_t* element) { free(element); }

void push_menu_bluetooth_connect() {
  uint8_t device_count = MIN(midi_ble_client_device_count(), 16);
  ui_menu_t* menu = push_menu(device_count);

  for (uint8_t i = 0; i < device_count; i++) {
    bluetooth_device_menu_item_t* item =
        malloc(sizeof(bluetooth_device_menu_item_t));

    item->index = i;
    item->base.label = midi_ble_client_get_device_name(i + 1);
    // NOTE: action is unused
    item->base.action = NULL;
    item->base.base.free = bluetooth_connect_free;
    item->base.base.navigate = bluetooth_connect_nav;
    item->base.base.render = ui_menu_item_default_render;
    menu_add(menu, (ui_element_t*)item);
  }
}