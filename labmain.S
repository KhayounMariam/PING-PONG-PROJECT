/* labmain.S — keep for extra assembly helpers or ISRs.
 * IMPORTANT: No _start symbol here (only in boot.S).
 */

    .section .text
    .globl dummy_asm_helper
dummy_asm_helper:
    /* Example pass-through: int dummy_asm_helper(int x) { return x+1; } */
    addi a0, a0, 1
    ret
