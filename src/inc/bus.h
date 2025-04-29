#ifndef BUS_H
#define BUS_H

#include "common.h"

uint8_t busRead(uint16_t address);
void busWrite(uint16_t address, uint8_t value);

#endif //BUS_H
