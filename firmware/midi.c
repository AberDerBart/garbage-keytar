#include "midi.h"

#include "midi_ble.h"
#include "midi_ble_client.h"
#include "midi_uart.h"
#include <stdio.h>

#define CMD_NOTE_ON 0x90
#define CMD_NOTE_OFF 0x80
#define CMD_CC 0xb0
#define CMD_PC 0xc0
#define CMD_PITCHBEND 0xe0

#define CONTROL_ALL_NOTES_OFF_B1 123
#define CONTROL_ALL_NOTES_OFF_B2 0

#define CONTROL_SUSTAIN 64
#define CONTROL_SOSTENUTO 66

#define CC_MODULATION 1
#define CC_SOUND_CONTROLLER_3_RELEASE 72
#define CC_SOUND_CONTROLLER_4_ATTACK 73
#define CC_SOUND_CONTROLLER_6_DECAY 75
#define CC_SOUND_CONTROLLER_7_SUSTAIN 76

#define CONTROL_VALUE_ON 64
#define CONTROL_VALUE_OFF 0

bool send(uint8_t len, uint8_t *msg) {
  bool ok = true;
  if (!midi_uart_write(len, msg)){
    ok = false;
    printf("Failed to send midi via UART\n");
  }
  if (midi_ble_is_connected()){
    if (!midi_ble_server_write(len, msg)) {
      ok = false;
      printf("Failed to send midi via BLE server\n");
    }
  }
  if (midi_ble_client_is_connected()) {
    if (!midi_ble_client_write(len, msg)) {
      ok = false;
      printf("Failed to send midi via BLE client\n");
    }
  }
  return ok;
}

bool midi_note_on(uint8_t note) {
  uint8_t msg[3] = {CMD_NOTE_ON, note, 127};
  return send(3, msg);
}

bool midi_note_off(uint8_t note) {
  uint8_t msg[3] = {CMD_NOTE_OFF, note, 127};
  return send(3, msg);
}

bool midi_program_change(uint8_t program) {
  uint8_t msg[2] = {CMD_PC, program};
  return send(2, msg);
}

bool midi_clear_notes() {
  uint8_t msg[3] = {CMD_CC, CONTROL_ALL_NOTES_OFF_B1, CONTROL_ALL_NOTES_OFF_B2};
  return send(3, msg);
}

bool midi_pitchbend(uint8_t low, uint8_t high) {
  uint8_t msg[3] = {CMD_PITCHBEND, low, high};
  return send(3, msg);
}

bool midi_cc_mod(uint8_t mod) {
  uint8_t msg[3] = {CMD_CC, CC_MODULATION, mod};
  return send(3, msg);
}

bool midi_cc_attack(uint8_t attack) {
  uint8_t msg[3] = {CMD_CC, CC_SOUND_CONTROLLER_4_ATTACK, attack};
  return send(3, msg);
}

bool midi_cc_decay(uint8_t decay) {
  uint8_t msg[3] = {CMD_CC, CC_SOUND_CONTROLLER_6_DECAY, decay};
  return send(3, msg);
}

bool midi_cc_sustain(uint8_t sustain) {
  uint8_t msg[3] = {CMD_CC, CC_SOUND_CONTROLLER_7_SUSTAIN, sustain};
  return send(3, msg);
}

bool midi_cc_release(uint8_t release) {
  uint8_t msg[3] = {CMD_CC, CC_SOUND_CONTROLLER_3_RELEASE, release};
  return send(3, msg);
}
