#include "dtekv-lib.h"

static void delay(volatile unsigned int cycles){
    while (cycles--) { __asm__ volatile("nop"); }
}

void pong(void){
    print("\n=== DTEK-V Pong: bring-up ===\n");
    print("If you see this, JTAG UART + runtime are OK.\n");
    unsigned int beat = 0;
    while (1){
        print("heartbeat: "); print_dec(beat++); print("\n");
        delay(500000);
    }
}
