#include "../inc/cpu.h"
#include "../inc/bus.h"
#include "../inc/emu.h"
#include "../inc/interrupts.h"
#include "../inc/dbg.h"
#include "../inc/timer.h"

CPUContext ctx = {0};

void cpuInit() {
    ctx.regs.PC = 0x100;
    ctx.regs.SP = 0xFFFE;
    *((short *)&ctx.regs.A) = 0xB001;
    *((short *)&ctx.regs.B) = 0x1300;
    *((short *)&ctx.regs.D) = 0xD800;
    *((short *)&ctx.regs.H) = 0x4D01;
    ctx.interruptEnableRegister = 0;
    ctx.interruptFlags = 0;
    ctx.interruptMasterEnabled = false;
    ctx.enablingIME = false;

    timerGetContext()->div = 0xABCC;
}

static void fetchInstruction() {
    ctx.currentOPCode = busRead(ctx.regs.PC++);
    ctx.currentInstruction = instructionByOpCode(ctx.currentOPCode);
}

void fetchData();

static void execute() {
    InstructionProcess process = instructionGetProcessor(ctx.currentInstruction->type);

    if (!process) {
        NOIMPL
    }

    process(&ctx);
}

bool cpuStep() {

    if (!ctx.halted) {
        uint16_t pc = ctx.regs.PC;

        fetchInstruction();
        emuCycles(1);
        fetchData();

        char flags[16];
        sprintf(flags, "%c%c%c%c",
            ctx.regs.F & (1 << 7) ? 'Z' : '-',
            ctx.regs.F & (1 << 6) ? 'N' : '-',
            ctx.regs.F & (1 << 5) ? 'H' : '-',
            ctx.regs.F & (1 << 4) ? 'C' : '-'
        );

        char inst[16];
        instructionToString(&ctx, inst);

        printf("%08lX - %04X: %-12s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: %02X%02X HL: %02X%02X\n",
            emuGetContext()->ticks,
            pc, inst, ctx.currentOPCode,
            busRead(pc + 1), busRead(pc + 2), ctx.regs.A, flags, ctx.regs.B, ctx.regs.C,
            ctx.regs.D, ctx.regs.E, ctx.regs.H, ctx.regs.L);

        if (ctx.currentInstruction == NULL) {
            printf("Unknown Instruction %02X\n", ctx.currentOPCode);
            exit(-7);
        }

        dbgUpdate();
        dbgPrint();

        execute();
    } else {
        //IF HALTED
        emuCycles(1);

        if (ctx.interruptFlags) {
            ctx.halted = false;
        }
    }

    if (ctx.interruptMasterEnabled) {
        cpuHandleInterrupts(&ctx);
        ctx.enablingIME = false;
    }

    if (ctx.enablingIME) {
        ctx.interruptMasterEnabled = true;
    }

    return true;
}

uint16_t cpuGetInterruptReg() {
    return ctx.interruptEnableRegister;
}

void cpuSetInterruptRegister(uint8_t n) {
    ctx.interruptEnableRegister = n;
}

void cpuRequestInterrupt(interruptType type) {
    ctx.interruptFlags |= type;
}