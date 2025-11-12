#ifndef DMA_H
#define DMA_H

#include "common.h"

void dma_start(u8 start);
void dma_tick();

bool dma_transferring();

#endif //DMA_H
