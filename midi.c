#include "midi.h"

#include <stdint.h>
#include <stdio.h>

#include "display.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 31250
#define MIDI_OUT_PIN 0
#define MIDI_IN_PIN 1

void midi_init() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(MIDI_OUT_PIN, GPIO_FUNC_UART);
    gpio_set_function(MIDI_IN_PIN, GPIO_FUNC_UART);
}

void midi_write3(uint8_t cmd, uint8_t b1, uint8_t b2) {
#ifdef DEBUG_MIDI
    char msg[16];
    sprintf(msg, "MIDI %02X %02X %02X", cmd, b1, b2);
    display_debug(msg);
#endif
    uart_putc(UART_ID, cmd);
    uart_putc(UART_ID, b1);
    uart_putc(UART_ID, b2);
}

void midi_write2(uint8_t cmd, uint8_t b1) {
#ifdef DEBUG_MIDI
    char msg[16];
    sprintf(msg, "MIDI %02X %02X", cmd, b1);
    display_debug(msg);
#endif
    uart_putc(UART_ID, cmd);
    uart_putc(UART_ID, b1);
}

void clear_notes() {
    midi_write3(MIDI_CONTROL_CHANGE, MIDI_CONTROL_ALL_NOTES_OFF_B1, MIDI_CONTROL_ALL_NOTES_OFF_B2);
}
