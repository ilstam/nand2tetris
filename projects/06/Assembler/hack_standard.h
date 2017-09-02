#pragma once

#include <inttypes.h>

#define MAX_HACK_ADDRESS INT16_MAX

/*
 * Hack addresses are 15-bit unsigned values. By using a 16-bit signed value
 * we can also represent invalid addresses (all negative values).
 */
typedef int16_t hack_addr;
