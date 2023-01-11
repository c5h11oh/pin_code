#pragma once
#include <bits/types.h>

// change PIN_CODE_DIGITS_COUNT and PIN_CODE_RANGE simultaneously
#define PIN_CODE_DIGITS_COUNT 4
#define PIN_CODE_RANGE 10000

typedef unsigned short pin_code_t;

// type of function that determines whether the pin code is good.
// returns true if the argument is a good pin code in this functions perspective.
typedef bool (*good_pin_code_func_t)(pin_code_t);

// data structure that is persisted to the storage
struct persistence_t {
    pin_code_t good_pin_codes[PIN_CODE_RANGE]; // all numbers that are good. 2B * 10000 ~= 20 KB
    pin_code_t unused_pin_codes[PIN_CODE_RANGE]; // number pool that the program can take from. 2B * 10000 ~= 20 KB
    unsigned int good_pin_codes_count; // size of good_pin_codes. 4 B
    unsigned int unused_pin_codes_count; // size of unused_pin_codes. 4 B
};
