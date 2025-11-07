#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BITSET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n);}
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

uint32_t getTicks();
void delay(uint32_t ms);

#define NOIMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(-5); }

#endif //COMMON_H
