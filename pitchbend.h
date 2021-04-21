#ifndef _PITCHBEND_H_
#define _PITCHBEND_H_

#include <stdint.h>

struct pitchbend_value
{
    uint8_t high;
    uint8_t low;
};

void pitchbend_init();

struct pitchbend_value pitchbend_read();

#endif