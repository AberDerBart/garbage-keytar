#include "midi_ble_client.h"

#include "ble_midi_client.h"
#include "menu/bluetooth.h"
#include "pico/cyw43_arch.h"

bool ble_client_is_initialized = false;

void midi_ble_client_init_with_addr(int addr_type, uint8_t *addr) {
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

  if (addr == NULL) {
    ble_midi_client_scan_begin();
  } else {
    ble_midi_client_set_last_connected(addr_type, addr);
    ble_midi_client_request_connect(0);
  }

  ble_client_is_initialized = true;
  menu_update_bluetooth();
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
  menu_update_bluetooth();
}

bool midi_ble_client_is_initialized() { return ble_client_is_initialized; }

void midi_ble_client_write(uint8_t len, uint8_t *msg) {
  if (!ble_midi_client_is_connected()) {
    return;
  }
  printf("sending");

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

void midi_ble_client_connect_1() { ble_midi_client_request_connect(1); }
void midi_ble_client_connect_2() { ble_midi_client_request_connect(2); }
void midi_ble_client_connect_3() { ble_midi_client_request_connect(3); }
void midi_ble_client_connect_4() { ble_midi_client_request_connect(4); }
void midi_ble_client_connect_5() { ble_midi_client_request_connect(5); }
void midi_ble_client_connect_6() { ble_midi_client_request_connect(6); }
void midi_ble_client_connect_7() { ble_midi_client_request_connect(7); }
void midi_ble_client_connect_8() { ble_midi_client_request_connect(8); }
void midi_ble_client_connect_9() { ble_midi_client_request_connect(9); }
void midi_ble_client_connect_10() { ble_midi_client_request_connect(10); }
void midi_ble_client_connect_11() { ble_midi_client_request_connect(11); }
void midi_ble_client_connect_12() { ble_midi_client_request_connect(12); }
void midi_ble_client_connect_13() { ble_midi_client_request_connect(13); }
void midi_ble_client_connect_14() { ble_midi_client_request_connect(14); }
void midi_ble_client_connect_15() { ble_midi_client_request_connect(15); }
void midi_ble_client_connect_16() { ble_midi_client_request_connect(16); }

void (*midi_ble_client_connect[16])() = {
    &midi_ble_client_connect_1,  &midi_ble_client_connect_2,
    &midi_ble_client_connect_3,  &midi_ble_client_connect_4,
    &midi_ble_client_connect_5,  &midi_ble_client_connect_6,
    &midi_ble_client_connect_7,  &midi_ble_client_connect_8,
    &midi_ble_client_connect_9,  &midi_ble_client_connect_10,
    &midi_ble_client_connect_11, &midi_ble_client_connect_12,
    &midi_ble_client_connect_13, &midi_ble_client_connect_14,
    &midi_ble_client_connect_15, &midi_ble_client_connect_16,
};

void midi_ble_get_last_connected(int *addr_type_target, uint8_t *addr_target) {
  *addr_type_target = ble_midi_client_get_last_conntected(addr_target);
}
