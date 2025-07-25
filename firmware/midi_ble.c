#include "midi_ble.h"

#include "btstack.h"
#include "display.h"
#include "menu/bluetooth.h"
#include "midi-ble.h"
#include "midi_ble_client.h"
#include "midi_service_stream_handler.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "ui_stack.h"

#define APP_AD_FLAGS 0x06
const uint8_t adv_data[] = {
    // Flags general discoverable
    0x02,
    BLUETOOTH_DATA_TYPE_FLAGS,
    APP_AD_FLAGS,
    // Service class list
    0x11,
    BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS,
    0x00,
    0xc7,
    0xc4,
    0x4e,
    0xe3,
    0x6c,
    0x51,
    0xa7,
    0x33,
    0x4b,
    0xe8,
    0xed,
    0x5a,
    0x0e,
    0xb8,
    0x03,
};
const uint8_t adv_data_len = sizeof(adv_data);

const uint8_t scan_resp_data[] = {
    // Name
    0x0E, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
    'B',  'L',
    'E',  '-',
    'M',  'I',
    'D',  'I',
    ' ',  'D',
    'e',  'm',
    'o',
};
const uint8_t scan_resp_data_len = sizeof(scan_resp_data);

static hci_con_handle_t con_handle = HCI_CON_HANDLE_INVALID;

void server_packet_handler(uint8_t packet_type, uint16_t channel,
                           uint8_t *packet, uint16_t size) {
  UNUSED(size);
  UNUSED(channel);
  bd_addr_t local_addr;
  uint8_t event_type;
  // bd_addr_t addr;
  // bd_addr_type_t addr_type;
  // uint8_t status;

  if (packet_type != HCI_EVENT_PACKET) {
    return;
  }

  event_type = hci_event_packet_get_type(packet);
  switch (event_type) {
    case BTSTACK_EVENT_STATE:
      if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) {
        return;
      }
      gap_local_bd_addr(local_addr);
      printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));

      // setup advertisements
      uint16_t adv_int_min = 800;
      uint16_t adv_int_max = 800;
      uint8_t adv_type = 0;
      bd_addr_t null_addr;
      memset(null_addr, 0, 6);
      gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                    null_addr, 0x07, 0x00);
      assert(adv_data_len <= 31);  // ble limitation
      gap_advertisements_set_data(adv_data_len, (uint8_t *)adv_data);
      assert(scan_resp_data_len <= 31);  // ble limitation
      gap_scan_response_set_data(scan_resp_data_len, (uint8_t *)scan_resp_data);
      gap_advertisements_enable(1);

      break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
      printf("HCI_EVENT_DISCONNECTION_COMPLETE event\r\n");
      break;
    case HCI_EVENT_GATTSERVICE_META:
      switch (hci_event_gattservice_meta_get_subevent_code(packet)) {
        case GATTSERVICE_SUBEVENT_SPP_SERVICE_CONNECTED:
          con_handle =
              gattservice_subevent_spp_service_connected_get_con_handle(packet);
          printf("GATTSERVICE_SUBEVENT_SPP_SERVICE_CONNECTED event\r\n");
          ui_render();
          break;
        case GATTSERVICE_SUBEVENT_SPP_SERVICE_DISCONNECTED:
          printf("GATTSERVICE_SUBEVENT_SPP_SERVICE_DISCONNECTED event\r\n");
          con_handle = HCI_CON_HANDLE_INVALID;
          ui_render();
          break;
        default:
          break;
      }
      break;
    case SM_EVENT_JUST_WORKS_REQUEST:
      printf("ble-midi2usbhost: Just Works requested\n");
      sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
      break;
    default:
      break;
  }
}

bool midi_ble_is_connected() {
  return con_handle != HCI_CON_HANDLE_INVALID || midi_ble_client_is_connected();
}

static btstack_packet_callback_registration_t sm_event_callback_registration;

bool ble_server_is_initialized = false;

ble_state_t initial_state = {
    .mode = OFF,
    .connected_addr = {0, 0, 0, 0, 0, 0},
    .connected_addr_type = 0,
};

void midi_ble_init() {
  switch (initial_state.mode) {
    case SERVER:
      midi_ble_server_init();
      return;
    case CLIENT:
      midi_ble_client_init_with_addr(initial_state.connected_addr_type,
                                     initial_state.connected_addr);
      return;
    case OFF:
    default:
      return;
  }
}

void midi_ble_server_init() {
  printf("init ble midi server\n");

  if (cyw43_arch_init()) {
    printf("failed to initialise cyw43_arch\n");
    return;
  }
  l2cap_init();

  sm_init();

  att_server_init(profile_data, NULL, NULL);
  // just works, legacy pairing
  sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
  sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION |
                                     SM_AUTHREQ_BONDING);
  // register for SM events
  sm_event_callback_registration.callback = &server_packet_handler;
  sm_add_event_handler(&sm_event_callback_registration);
  midi_service_stream_init(server_packet_handler);

  hci_power_control(HCI_POWER_ON);
  ble_server_is_initialized = true;
  // menu_update_bluetooth();
}

void midi_ble_deinit() {
  printf("deinit ble midi server\n");
  midi_service_stream_deinit();
  sm_remove_event_handler(&sm_event_callback_registration);
  att_server_deinit();
  sm_deinit();
  l2cap_deinit();
  cyw43_arch_deinit();

  ble_server_is_initialized = false;
  // menu_update_bluetooth();
  con_handle = HCI_CON_HANDLE_INVALID;
}

bool midi_ble_server_is_initialized() { return ble_server_is_initialized; }

void midi_ble_server_write(uint8_t n_bytes, uint8_t *midi_stream_bytes) {
  if (con_handle == HCI_CON_HANDLE_INVALID) {
    return;
  }

  midi_service_stream_write(con_handle, n_bytes, midi_stream_bytes);
}

void midi_ble_set_initial_state(ble_state_t *state) {
  memcpy(&initial_state, state, sizeof(ble_state_t));
}

ble_state_t *midi_ble_get_initial_state() { return &initial_state; }

ble_state_t ble_state;

ble_state_t *midi_ble_get_state() {
  memset(ble_state.connected_addr, 0, sizeof(ble_state.connected_addr));
  ble_state.connected_addr_type = 0;
  ble_state.mode = OFF;

  if (midi_ble_server_is_initialized()) {
    ble_state.mode = SERVER;
  } else if (midi_ble_client_is_connected()) {
    ble_state.mode = CLIENT;
    midi_ble_get_last_connected(&ble_state.connected_addr_type,
                                ble_state.connected_addr);
  }

  return &ble_state;
}

void midi_ble_set_state_as_initial() {
  midi_ble_set_initial_state(midi_ble_get_state());
}