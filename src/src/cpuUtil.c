#include "../inc/cpu.h"
#include "../inc/bus.h"

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

void cpuSetReg(registerType rt, uint16_t value) {
    switch (rt) {
        case RegA: ctx.regs.A = value & 0xFF; break;
        case RegF: ctx.regs.F = value & 0xFF; break;
        case RegB: ctx.regs.B = value & 0xFF; break;
        case RegC: ctx.regs.C = value & 0xFF; break;
        case RegD: ctx.regs.D = value & 0xFF; break;
        case RegE: ctx.regs.E = value & 0xFF; break;
        case RegH: ctx.regs.H = value & 0xFF; break;
        case RegL: ctx.regs.L = value & 0xFF; break;

        case RegAF: *((uint16_t *)&ctx.regs.A) = reverse(value); break;
        case RegBC: *((uint16_t *)&ctx.regs.B) = reverse(value); break;
        case RegDE: *((uint16_t *)&ctx.regs.D) = reverse(value); break;
        case RegHL: *((uint16_t *)&ctx.regs.H) = reverse(value); break;

        case RegPC: ctx.regs.PC = value; break;
        case RegSP: ctx.regs.SP = value; break;
        case NONE: break;
    }
}

uint8_t cpuReadReg8(registerType rt) {
    switch(rt) {
        case RegA: return ctx.regs.A;
        case RegF: return ctx.regs.F;
        case RegB: return ctx.regs.B;
        case RegC: return ctx.regs.C;
        case RegD: return ctx.regs.D;
        case RegE: return ctx.regs.E;
        case RegH: return ctx.regs.H;
        case RegL: return ctx.regs.L;
        case RegHL: {
            return busRead(cpuReadReg(RegHL));
        }
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
            exit(-1);
    }
}

void cpuSetReg8(registerType rt, uint8_t value) {
    switch(rt) {
        case RegA: ctx.regs.A = value & 0xFF; break;
        case RegF: ctx.regs.F = value & 0xFF; break;
        case RegB: ctx.regs.B = value & 0xFF; break;
        case RegC: ctx.regs.C = value & 0xFF; break;
        case RegD: ctx.regs.D = value & 0xFF; break;
        case RegE: ctx.regs.E = value & 0xFF; break;
        case RegH: ctx.regs.H = value & 0xFF; break;
        case RegL: ctx.regs.L = value & 0xFF; break;
        case RegHL: busWrite(cpuReadReg(RegHL), value); break;
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
            exit(-1);
    }
}

Registers *cpuGetRegisters() {
    return &ctx.regs;
}

uint8_t cpuGetInterruptFlags() {
    return ctx.interruptFlags;
}
void cpuSetInterruptFlags(uint8_t flags) {
    ctx.interruptFlags = flags;
}