#ifndef IO_H
#define IO_H

#include "common.h"

u8 io_read(u16 address);
void io_write(u16 address, u8 value);

#endif //IO_H
