#ifndef DTEKV_LIB_H
#define DTEKV_LIB_H

#include <stdint.h>

/* ============= PLATFORM CONSTANTS (TWEAK THESE IF NEEDED) ============== */
/* Pick ONE VGA backend: define exactly one of USE_VGA_REGS or USE_VGA_FB.  */
#ifndef USE_VGA_REGS
#define USE_VGA_REGS 1   /* default: XY+COLOR registers */
#endif
/* #define USE_VGA_FB 1 */ /* alternative: linear framebuffer */

/* Screen geometry (typical 640x480 VGA) */
#ifndef VGA_WIDTH
#define VGA_WIDTH   640
#endif
#ifndef VGA_HEIGHT
#define VGA_HEIGHT  480
#endif

/* --- If using register-based pixel writes --- */
#if USE_VGA_REGS
/* Base and offsets for simple pixel write engine.
 * CHANGE these to match your SoC’s MMIO map if needed.
 */
#ifndef VGA_BASE
#define VGA_BASE     0x10000000u
#endif
#define VGA_REG_X    (*(volatile uint32_t*)(VGA_BASE + 0x00))
#define VGA_REG_Y    (*(volatile uint32_t*)(VGA_BASE + 0x04))
#define VGA_REG_CLR  (*(volatile uint32_t*)(VGA_BASE + 0x08))
#define VGA_REG_CMD  (*(volatile uint32_t*)(VGA_BASE + 0x0C)) /* write strobe */
#define VGA_REG_STAT (*(volatile uint32_t*)(VGA_BASE + 0x10)) /* optional status (vsync) */
#define VGA_STAT_VBLANK (1u<<0)
#endif

/* --- If using a linear framebuffer --- */
#ifdef USE_VGA_FB
/* For an 8-bit RGB332 framebuffer at this base. Adjust to your design. */
#ifndef VGA_FB_BASE
#define VGA_FB_BASE  0x20000000u
#endif
#ifndef VGA_BPP
#define VGA_BPP 1            /* 1=8bpp (RGB332) ; 2=RGB565 */
#endif
#endif
/* ======================================================================= */

#ifdef __cplusplus
extern "C" {
#endif

/* Tiny delay (busy wait) */
void dtekv_delay(volatile uint32_t n);

/* Pixel + clear (implemented per backend in dtekv-lib.c) */
void vga_putpx(int x, int y, uint16_t color);
void vga_clear(uint16_t color);
void vga_wait_vblank(void);

/* Color helpers */
static inline uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
}
static inline uint8_t RGB332(uint8_t r, uint8_t g, uint8_t b) {
    return (uint8_t)(((r>>5)<<5) | ((g>>5)<<2) | (b>>6));
}

#ifdef __cplusplus
}
#endif

#endif /* DTEKV_LIB_H */

