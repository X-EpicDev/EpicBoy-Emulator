#ifndef TIMER_H
#define TIMER_H

#include "common.h"

typedef struct {
    uint16_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
} timerContext;

void timerInit();
void timerTick();

void timerWrite(uint16_t address, uint8_t value);
uint8_t timerRead(uint16_t address);

timerContext *timerGetContext();

#endif //TIMER_H
