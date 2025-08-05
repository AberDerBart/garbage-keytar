#include "./bluetooth_connect.h"

#include "midi_ble_client.h"
#include "ui/menu.h"
#include "ui_stack.h"

typedef struct bluetooth_device_menu_item_t {
  menu_item_t base;
  uint8_t index;
} bluetooth_device_menu_item_t;

void bluetooth_connect_action(menu_item_t* mi) {
  bluetooth_device_menu_item_t* self = (bluetooth_device_menu_item_t*)mi;
  midi_ble_client_connect(self->index);
  ui_clear();
}

bluetooth_device_menu_item_t mi_bluetooth_devices[16];
menu_item_t* mi_bluetooth_device_list[17];

void push_menu_bluetooth_connect() {
  uint8_t device_count = MIN(midi_ble_client_device_count(), 16);
  for (uint8_t i = 0; i < device_count; i++) {
    mi_bluetooth_devices[i].base.action = bluetooth_connect_action;
    mi_bluetooth_devices[i].base.label = midi_ble_client_get_device_name(i + 1);
    mi_bluetooth_devices[i].base.free = NULL;
    mi_bluetooth_devices[i].index = i;
    mi_bluetooth_device_list[i] = (menu_item_t*)&mi_bluetooth_devices[i];
  }
  mi_bluetooth_device_list[device_count] = NULL;
  push_menu(mi_bluetooth_device_list);
}