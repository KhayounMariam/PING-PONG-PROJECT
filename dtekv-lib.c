#include "dtekv-lib.h"

void dtekv_delay(volatile uint32_t n) {
    while (n--) __asm__ volatile("nop");
}

/* ----- Register-based backend ----- */
#if USE_VGA_REGS
void vga_putpx(int x, int y, uint16_t color) {
    if ((unsigned)x >= VGA_WIDTH || (unsigned)y >= VGA_HEIGHT) return;
    VGA_REG_X   = (uint32_t)x;
    VGA_REG_Y   = (uint32_t)y;
    VGA_REG_CLR = (uint32_t)color;
    VGA_REG_CMD = 1u;                 /* strobe a write */
}

void vga_clear(uint16_t color) {
    for (int y = 0; y < VGA_HEIGHT; ++y)
        for (int x = 0; x < VGA_WIDTH; ++x)
            vga_putpx(x, y, color);
}

void vga_wait_vblank(void) {
    /* If status not implemented, just no-op */
    volatile uint32_t tmp;
    (void)tmp;
    /* Wait for vblank rising edge if available */
    uint32_t s0 = VGA_REG_STAT & VGA_STAT_VBLANK;
    do { tmp = VGA_REG_STAT; } while (((tmp & VGA_STAT_VBLANK) == s0));
}

/* ----- Framebuffer backend ----- */
#elif defined(USE_VGA_FB)

#if VGA_BPP == 1
static volatile uint8_t  *const fb8  = (volatile uint8_t*)VGA_FB_BASE;
#elif VGA_BPP == 2
static volatile uint16_t *const fb16 = (volatile uint16_t*)VGA_FB_BASE;
#else
#error "Unsupported VGA_BPP"
#endif

void vga_putpx(int x, int y, uint16_t color) {
    if ((unsigned)x >= VGA_WIDTH || (unsigned)y >= VGA_HEIGHT) return;
#if VGA_BPP == 1
    fb8[y*VGA_WIDTH + x] = (uint8_t)color;  /* Expect RGB332 value */
#else
    fb16[y*VGA_WIDTH + x] = color;          /* RGB565 */
#endif
}

void vga_clear(uint16_t color) {
#if VGA_BPP == 1
    for (int y=0; y<VGA_HEIGHT; ++y)
        for (int x=0; x<VGA_WIDTH; ++x)
            fb8[y*VGA_WIDTH + x] = (uint8_t)color;
#else
    for (int y=0; y<VGA_HEIGHT; ++y)
        for (int x=0; x<VGA_WIDTH; ++x)
            fb16[y*VGA_WIDTH + x] = color;
#endif
}

void vga_wait_vblank(void) {
    /* No vblank info: no-op */
}

#else
#error "Select a VGA backend: define USE_VGA_REGS or USE_VGA_FB"
#endif
