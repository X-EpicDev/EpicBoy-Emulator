#include "../inc/cpu.h"
#include "../inc/bus.h"
#include "../inc/emu.h"
#include "../inc/interrupt.h"

CPUContext ctx = {0};

void cpuInit() {
    ctx.regs.A = 0x01;
    ctx.regs.F = 0xB0;
    ctx.regs.B = 0x00;
    ctx.regs.C = 0x13;
    ctx.regs.D = 0x00;
    ctx.regs.E = 0xD8;
    ctx.regs.H = 0x01;
    ctx.regs.L = 0x4D;
    ctx.regs.SP = 0xFFFE;
    ctx.regs.PC = 0x0100;
    ctx.interruptRegister = 0;
    ctx.interruptEnabled = false;
    ctx.enableIME = false;
}

static void fetchInstruction() {
    ctx.currentOPCode = busRead(ctx.regs.PC++);
    ctx.CurrentInstruction = instructionByOpCode(ctx.currentOPCode);
}

void fetchData();

static void execute() {
    InstructionProcess process = instructionGetProcessor(ctx.CurrentInstruction->type);

    if (!process) {
        //NO IMPL
    }

    process(&ctx);
}

// uint64_t checker = 0; - Keeping this here for checking if needed

bool cpuStep() {

    if (!ctx.halted) {
        uint16_t pc = ctx.regs.PC;

        fetchInstruction();
        fetchData();

        char flags[16];
        sprintf(flags, "%c%c%c%c",
            ctx.regs.F & (1 << 7) ? 'Z' : '-',
            ctx.regs.F & (1 << 6) ? 'N' : '-',
            ctx.regs.F & (1 << 5) ? 'H' : '-',
            ctx.regs.F & (1 << 4) ? 'C' : '-');


        printf("%08lX - PC: %04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X D: %02X E: %02X F: %02X H: %02X L: %02X SP: %04X FLAGS: %s\n",
            emuGetContext()->ticks, pc, instructionName(ctx.CurrentInstruction->type), ctx.currentOPCode,
            busRead(pc+1), busRead(pc+2), ctx.regs.A, ctx.regs.B, ctx.regs.C, ctx.regs.D, ctx.regs.E, ctx.regs.F, ctx.regs.H, ctx.regs.L, ctx.regs.SP, flags);

        // checker++;
        // if (checker == 0xA0) {
        //     exit(1);
        // }
        // LEGACY ADDRESS STOPPING CODE (DO NOT DELETE INCASE OF EMERGENCY)

        if (ctx.CurrentInstruction == NULL) {
            printf("Unknown Instruction %02X\n", ctx.currentOPCode);
            exit(-7);
        }

        execute();
    } else {
        emuCycles(1);

        if (ctx.interruptFlags) {
            ctx.halted = false;
        }
    }

    if (ctx.interruptEnabled) {
        cpuHandleInterrupt(&ctx);
        ctx.enableIME = false;
    }

    if (ctx.enableIME) {
        ctx.interruptEnabled = true;
    }

    return true;
}

uint16_t cpuGetInterruptReg() {
    return ctx.interruptRegister;
}
void cpuSetInterruptRegister(uint8_t n) {
    ctx.interruptRegister = n;
}