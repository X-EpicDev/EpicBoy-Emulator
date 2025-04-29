#include "../inc/cpu.h"

CPUContext ctx = {0};

void cpuInit() {

}

static void fetchInstruction() {

}

static void fetchData() {

}

static void execute() {

}

bool cpuStep() {

    if (!ctx.halted) {
        fetchInstruction();
        fetchData();
        execute();
    }

    return false;
}