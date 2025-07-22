#include "midi_ble_client.h"

#include "ble_midi_client.h"
#include "menu/bluetooth.h"
#include "pico/cyw43_arch.h"
#include "ui_stack.h"

bool ble_client_is_initialized = false;

void midi_ble_client_init_with_addr(int addr_type, uint8_t *addr) {
  printf("init midi ble client: ");

  ble_midi_client_set_status_change_cb(ui_render);

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

  if (addr == NULL) {
    ble_midi_client_scan_begin();
  } else {
    ble_midi_client_set_last_connected(addr_type, addr);
    ble_midi_client_request_connect(0);
  }

  ble_client_is_initialized = true;
  ui_render();
}

void midi_ble_client_init() { midi_ble_client_init_with_addr(0, NULL); }

void midi_ble_client_deinit() {
  printf("deinit midi ble client: ");
  ble_midi_client_scan_end();

  ble_midi_client_request_disconnect();

  ble_midi_client_deinit();

  cyw43_arch_deinit();

  printf("done\n");
  ble_client_is_initialized = false;
  ui_render();
}

bool midi_ble_client_is_initialized() { return ble_client_is_initialized; }

void midi_ble_client_write(uint8_t len, uint8_t *msg) {
  if (!ble_midi_client_is_connected()) {
    return;
  }
  printf("sending %d %d %d\n", msg[0], msg[1], msg[2]);

  ble_midi_client_stream_write(len, msg);
}

uint8_t midi_ble_client_device_count() {
  return ble_midi_client_get_n_midi_peripherals();
}

char *midi_ble_client_get_device_name(uint8_t index) {
  return ble_midi_client_get_midi_peripheral_name(index);
}

bool midi_ble_client_is_connected() { return ble_midi_client_is_connected(); }

void midi_ble_client_disconnect() {
  ble_midi_client_request_disconnect();
  ble_midi_client_scan_begin();
}

void midi_ble_client_connect(uint8_t index) {
  printf("requesting ble connection at index %d\n", index);
  ble_midi_client_request_connect(index);
}

void midi_ble_get_last_connected(int *addr_type_target, uint8_t *addr_target) {
  *addr_type_target = ble_midi_client_get_last_conntected(addr_target);
}
