#include "midi.h"

#include "midi_ble.h"
#include "midi_ble_client.h"
#include "midi_uart.h"

#define CMD_NOTE_ON 0x90
#define CMD_NOTE_OFF 0x80
#define CMD_CC 0xb0
#define CMD_PC 0xc0
#define CMD_PITCHBEND 0xe0

#define CONTROL_ALL_NOTES_OFF_B1 123
#define CONTROL_ALL_NOTES_OFF_B2 0

#define CONTROL_SUSTAIN 64
#define CONTROL_SOSTENUTO 66

#define CONTROL_VALUE_ON 64
#define CONTROL_VALUE_OFF 0

void send(uint8_t len, uint8_t *msg) {
  midi_uart_write(len, msg);
  midi_ble_server_write(len, msg);
  midi_ble_server_write(len, msg);
}

void midi_note_on(uint8_t note) {
  uint8_t msg[3] = {CMD_NOTE_ON, note, 127};
  send(3, msg);
}

void midi_note_off(uint8_t note) {
  uint8_t msg[3] = {CMD_NOTE_OFF, note, 127};
  send(3, msg);
}

void midi_program_change(uint8_t program) {
  uint8_t msg[2] = {CMD_PC, program};
  send(2, msg);
}

void midi_clear_notes() {
  uint8_t msg[3] = {CMD_CC, CONTROL_ALL_NOTES_OFF_B1, CONTROL_ALL_NOTES_OFF_B2};
  send(3, msg);
}
