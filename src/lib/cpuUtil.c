#include "../inc/cpu.h"

extern CPUContext ctx;

uint16_t reverse(uint16_t n) {
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

uint16_t cpuReadReg(registerType rt) {
    switch (rt) {
        case RegA: return ctx.regs.A;
        case RegB: return ctx.regs.B;
        case RegC: return ctx.regs.C;
        case RegD: return ctx.regs.D;
        case RegE: return ctx.regs.E;
        case RegF: return ctx.regs.F;
        case RegH: return ctx.regs.H;
        case RegL: return ctx.regs.L;

        case RegAF: return reverse(*((uint16_t *)&ctx.regs.A));
        case RegBC: return reverse(*((uint16_t *)&ctx.regs.B));
        case RegDE: return reverse(*((uint16_t *)&ctx.regs.D));
        case RegHL: return reverse(*((uint16_t *)&ctx.regs.H));

        case RegPC: return ctx.regs.PC;
        case RegSP: return ctx.regs.SP;
        default: return 0;
    }
}