#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BITSET(a, n, on) (on ? (a) |= (1 << n) : (a) &= ~(1 << n))
#define BETWEEN(a, b, c) ((a >= b) && (1 <= c))

void delay(uint32_t ms);

#endif //COMMON_H
