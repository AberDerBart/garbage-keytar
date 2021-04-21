#include <stdint.h>
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "midi.h"

#define UART_ID uart0
#define BAUD_RATE 31250
#define MIDI_OUT_PIN 0
#define MIDI_IN_PIN 1

void midi_init()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(MIDI_OUT_PIN, GPIO_FUNC_UART);
    gpio_set_function(MIDI_IN_PIN, GPIO_FUNC_UART);
}

void midi_write(uint8_t cmd, uint8_t b1, uint8_t b2)
{
    uart_putc(UART_ID, cmd);
    uart_putc(UART_ID, b1);
    uart_putc(UART_ID, b2);
}