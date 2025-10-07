#include "../inc/cpu.h"
#include "../inc/stack.h"
#include "../inc/interrupts.h"

void interruptHandle(CPUContext *ctx, uint16_t address) {
    stackPush16(ctx->regs.PC);
    ctx->regs.PC = address;
}

bool interruptCheck(CPUContext *ctx, uint16_t address, interruptType it) {
    if (ctx->interruptFlags & it && ctx->interruptEnableRegister & it) {
        interruptHandle(ctx, address);
        ctx->interruptFlags &= ~it;
        ctx->halted = false;
        ctx->interruptMasterEnabled = false;

        return true;
    }

    return false;
}

void cpuHandleInterrupts(CPUContext *ctx) {
    if (interruptCheck(ctx, 0x40, Interrupt_VBLANK)) {

    } else if (interruptCheck(ctx, 0x48, Interrupt_LCDSTAT)) {

    } else if (interruptCheck(ctx, 0x50, Interrupt_TIMER)) {

    }  else if (interruptCheck(ctx, 0x58, Interrupt_SERIAL)) {

    }  else if (interruptCheck(ctx, 0x60, Interrupt_JOYPAD)) {

    }
}