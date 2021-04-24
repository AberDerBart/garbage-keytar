#include "control.h"
#include "hardware/adc.h"

void control_init() {
    adc_init();
    adc_gpio_init(27);
    adc_gpio_init(26);
}

uint8_t read_control(uint8_t adc_index) {
    adc_select_input(adc_index);

    uint16_t value = adc_read();
    return 0x7f & (value >> 5);
}
