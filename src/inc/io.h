#ifndef IO_H
#define IO_H

#include "common.h"

uint8_t ioRead(uint16_t address);
void ioWrite(uint16_t address, uint8_t data);

#endif //IO_H
