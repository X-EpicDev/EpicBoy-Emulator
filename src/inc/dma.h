#ifndef DMA_H
#define DMA_H

#include "common.h"

void dmaStart(uint8_t start);
void dmaTick();

bool dmaTransferring();

#endif //DMA_H
