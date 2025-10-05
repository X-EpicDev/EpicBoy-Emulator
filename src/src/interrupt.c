#include "../inc/cpu.h"
#include "../inc/interrupt.h"
#include "../inc/stack.h"

void interruptHandle(CPUContext *ctx, uint16_t address) {
    stackPush16(ctx->regs.PC);
    ctx->regs.PC = address;
}

bool interruptCheck(CPUContext *ctx, uint16_t address, interruptType it) {
    if (ctx->interruptFlags & it && ctx->interruptRegister & it) {
        interruptHandle(ctx, address);
        ctx->interruptFlags &= ~it;
        ctx->halted = false;
        ctx->interruptEnabled = false;

        return true;
    }

    return false;
}

void cpuHandleInterrupt(CPUContext *ctx) {
    if (interruptCheck(ctx, 0x40, InterruptVBLANK)) {

    } else if (interruptCheck(ctx, 0x48, InterruptLCDSTAT)) {

    } else if (interruptCheck(ctx, 0x50, InterruptTIMER)) {

    }  else if (interruptCheck(ctx, 0x58, InterruptSERIAL)) {

    }  else if (interruptCheck(ctx, 0x60, InterruptJOYPAD)) {

    }
}