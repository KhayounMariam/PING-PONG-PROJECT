/* boot.S — single reset entry point */
    .section .init
    .globl _start
_start:
    /* Set up stack */
    la   sp, _stack_top

    /* Optional: set machine trap-vector base (points at trap_vector below) */
    la   t0, trap_vector
    csrw mtvec, t0

    /* Zero .bss */
    la   a0, __bss_start
    la   a1, __bss_end
1:
    bge  a0, a1, 2f
    sw   zero, 0(a0)
    addi a0, a0, 4
    j    1b
2:
    /* Call C main() */
    call main

halt:
    wfi
    j    halt

    .align 2
    .globl trap_vector
trap_vector:
    j trap_handler      /* simple all-traps -> handler */

    .weak trap_handler
trap_handler:
    mret
