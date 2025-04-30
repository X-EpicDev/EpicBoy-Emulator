#include "../inc/cpu.h"
#include "../inc/bus.h"
#include "../inc/emu.h"

CPUContext ctx = {0};

void cpuInit() {
    ctx.regs.PC = 0x100;
    ctx.regs.A = 0x01;
}

static void fetchInstruction() {
    ctx.currentOPCode = busRead(ctx.regs.PC++);
    ctx.CurrentInstruction = instructionByOpCode(ctx.currentOPCode);
}

static void fetchData() {
    ctx.memoryDestination = 0;
    ctx.destinationIsMemory = false;

    if (ctx.CurrentInstruction == NULL) {

        printf("Unknown Instruction %02X\n", ctx.currentOPCode);
        exit(-7);
        return;
    }

    switch(ctx.CurrentInstruction->mode) {
        //Implied
        case IMPL:
            return;

        //Register
        case REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg1);
            return;

        case REG_D8:
            ctx.fetchData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case REG_D16:
        case D16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            ctx.fetchData = lo | (hi << 8);
            ctx.regs.PC += 2;
            return;
        }

        default:
            printf("Unknown Addressing Mode %d", ctx.CurrentInstruction->mode);
            exit(-7);
            return;

    }
}

static void execute() {
    printf("Executing instruction %02X  PC: %04X\n", ctx.currentOPCode,ctx.regs.PC);

    InstructionProcess process = instructionGetProcessor(ctx.CurrentInstruction->type);

    if (!process) {
        //NO IMPL
    }

    process(&ctx);
}

bool cpuStep() {

    if (!ctx.halted) {
        fetchInstruction();
        fetchData();
        execute();
    }

    return true;
}