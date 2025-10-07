#include "../inc/dbg.h"
#include "../inc/bus.h"

static char dbgMSG[1024] = {0};
static uint32_t msgSize = 0;

void dbgUpdate() {
    if (busRead(0xFF02) == 0x81) {
        uint8_t c = busRead(0xFF01);

        dbgMSG[msgSize++] = c;

        busWrite(0xFF02, 0);
    }
}

void dbgPrint() {
    if (dbgMSG[0]) {
        printf("DBG: %s\n", dbgMSG);
    }
}