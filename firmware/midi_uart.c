#include "midi.h"

#include <stdint.h>
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "midi_uart.h"

#define UART_ID uart1
#define BAUD_RATE 31250
#define MIDI_OUT_PIN 4
#define MIDI_IN_PIN 5

bool midi_uart_initialized = false;

void midi_uart_init() {
    printf("init midi uart: ");
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(MIDI_OUT_PIN, GPIO_FUNC_UART);
    gpio_set_function(MIDI_IN_PIN, GPIO_FUNC_UART);
    midi_uart_initialized = uart_is_enabled(UART_ID);
    if(midi_uart_initialized){
      printf("done\n");
    }else{
      printf("failed\n");
    }
}

void midi_uart_write(uint8_t len, uint8_t* msg) {
  if(!midi_uart_initialized) {
    return;
  }
  for(uint8_t i = 0; i < len; i++) {
    uart_putc(UART_ID, msg[i]);
  }
}
