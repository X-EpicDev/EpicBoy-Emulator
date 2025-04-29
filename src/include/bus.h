#ifndef BUS_H
#define BUS_H

#include "common.h"

uint8_t bus_read(uint16_t address);
void bus_write(uint16_t address, uint8_t data);

#endif //BUS_H
