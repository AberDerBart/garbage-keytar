#include "pitchbend.h"
#include "hardware/adc.h"

void pitchbend_init()
{
    adc_init();
    adc_gpio_init(28);
}

struct pitchbend_value to_pitchbend(int16_t value)
{
    if (value > (1 << 13) - 1)
    {
        return to_pitchbend((1 << 13) - 1);
    }

    if (value < -(1 << 13))
    {
        return to_pitchbend(-(1 << 13));
    }

    uint16_t offset_value = value + 0x2000;

    struct pitchbend_value result = {
        low : offset_value & 0x7f,
        high : (offset_value >> 7) & 0x7f
    };

    return result;
}

struct pitchbend_value pitchbend_read()
{
    adc_select_input(2);
    uint16_t const ACTIVATION_THRESHOLD = 3276;
    uint16_t const LOWER_BOUND = 30;
    uint16_t const UPPER_BOUND = 2700;
    uint16_t const INACTIVE = 0xffff;

    static uint16_t start_value = INACTIVE;

    uint16_t adc_value = adc_read();

    if (start_value == INACTIVE)
    {
        if (adc_value < ACTIVATION_THRESHOLD)
        {
            start_value = adc_read();
            return to_pitchbend(0);
        }
    }
    else
    {
        if (adc_value > ACTIVATION_THRESHOLD)
        {
            start_value = INACTIVE;
            return to_pitchbend(0);
        }
        else
        {
            int32_t value = (int32_t)(1 << 14) * (adc_value - start_value) / (UPPER_BOUND - LOWER_BOUND);
            return to_pitchbend(value);
        }
    }
    // just in case...
    return to_pitchbend(0);
}