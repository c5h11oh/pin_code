#pragma once
#include <bits/types.h>

#define PIN_CODE_RANGE 10000
typedef unsigned short pin_code_t;
// typedef int counter_t;

struct persistence_t {
    pin_code_t good_pin_codes[PIN_CODE_RANGE]; // all numbers that are good. 2B * 10000 ~= 20 KB
    pin_code_t unused_pin_codes[PIN_CODE_RANGE]; // number pool that the program can take from. 2B * 10000 ~= 20 KB
    unsigned int good_pin_codes_count; // size of good_pin_codes. 4 B
    unsigned int unused_pin_codes_count; // size of unused_pin_codes. 4 B
};
