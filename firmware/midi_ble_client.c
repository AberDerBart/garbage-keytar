#include "midi_ble_client.h"

#include "ble_midi_client.h"
#include "menu/bluetooth.h"
#include "pico/cyw43_arch.h"

bool ble_client_is_initialized = false;

void midi_ble_client_init() {
  printf("init midi ble client: ");

  if (cyw43_arch_init()) {
    printf("failed to initialise cyw43_arch\n");
    return;
  }

  const char client_name[] = "Keytar";
  ble_midi_client_init(client_name, strlen(client_name),
                       IO_CAPABILITY_DISPLAY_ONLY,
                       SM_AUTHREQ_SECURE_CONNECTION |
                           SM_AUTHREQ_MITM_PROTECTION | SM_AUTHREQ_BONDING);

  printf("done\n");

  ble_midi_client_scan_begin();
  ble_client_is_initialized = true;
  menu_update_bluetooth();
}

void midi_ble_client_deinit() {
  printf("deinit midi ble client: ");
  ble_midi_client_scan_end();

  ble_midi_client_request_disconnect();

  ble_midi_client_deinit();

  cyw43_arch_deinit();

  printf("done\n");
  ble_client_is_initialized = false;
  menu_update_bluetooth();
}

bool midi_ble_client_is_initialized() { return ble_client_is_initialized; }

void midi_ble_client_write(uint8_t len, uint8_t* msg) {
  if (!ble_midi_client_is_connected) {
    return;
  }

  ble_midi_client_stream_write(len, msg);
}

uint8_t midi_ble_client_device_count() {
  return ble_midi_client_get_n_midi_peripherals();
}

char* midi_ble_client_get_device_name(uint8_t index) {
  return ble_midi_client_get_midi_peripheral_name(index);
}
