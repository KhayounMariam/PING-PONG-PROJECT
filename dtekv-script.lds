/* Minimal RISC-V linker script for a RAM-only image.
 * Adjust ORIGIN/LENGTH to your SoC if needed.
 */

OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 64K
}

SECTIONS
{
  . = ORIGIN(RAM);

  .init : {
    KEEP(*(.init))
    KEEP(*(.vectors))
  } > RAM

  .text : ALIGN(4) {
    *(.text.boot*)
    *(.text*)
    *(.rodata*)
  } > RAM

  .data : ALIGN(4) {
    *(.data*)
    *(.sdata*)
  } > RAM

  .bss (NOLOAD) : ALIGN(4) {
    __bss_start = .;
    *(.sbss*)
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } > RAM

  .stack (NOLOAD) : ALIGN(16) {
    . += 4K;           /* 4 KB stack; grow if you need */
    _stack_top = .;
  } > RAM
}
