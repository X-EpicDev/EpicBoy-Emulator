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

//fetch current instruction
static void fetchData() {
    ctx.memoryDestination = 0;
    ctx.destinationIsMemory = false;

    if (ctx.CurrentInstruction == NULL) {
        return;
    }

    //address modes switch
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
            printf("Unknown Addressing Mode %d (%02X)\n", ctx.CurrentInstruction->mode, ctx.currentOPCode);
            exit(-7);
    }
}

static void execute() {
    InstructionProcess process = instructionGetProcessor(ctx.CurrentInstruction->type);

    if (!process) {
        //NO IMPL
    }

    process(&ctx);
}

bool cpuStep() {

    if (!ctx.halted) {
        uint16_t pc = ctx.regs.PC;

        fetchInstruction();
        fetchData();

        printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
            pc, instructionName(ctx.CurrentInstruction->type), ctx.currentOPCode,
            busRead(pc+1), busRead(pc+2), ctx.regs.A, ctx.regs.B, ctx.regs.C);

        if (ctx.CurrentInstruction == NULL) {
            printf("Unknown Instruction %02X\n", ctx.currentOPCode);
            exit(-7);
        }

        execute();
    }

    return true;
}