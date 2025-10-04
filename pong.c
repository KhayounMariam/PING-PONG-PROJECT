#include <stdint.h>
#include "dtekv-lib.h"

/* ---- Simple types ---- */
typedef struct { int x, y, w, h; int vy; } Paddle;
typedef struct { int x, y, vx, vy, r;    } Ball;

/* ---- Drawing helpers ---- */
static inline void fill_rect(int x, int y, int w, int h, uint16_t c) {
    if (w<=0 || h<=0) return;
    int x2 = x + w;
    int y2 = y + h;
    if (x < 0) x = 0; if (y < 0) y = 0;
    if (x2 > VGA_WIDTH)  x2 = VGA_WIDTH;
    if (y2 > VGA_HEIGHT) y2 = VGA_HEIGHT;

    for (int yy = y; yy < y2; ++yy)
        for (int xx = x; xx < x2; ++xx)
            vga_putpx(xx, yy, c);
}

static inline void draw_paddle(const Paddle *p, uint16_t c) {
    fill_rect(p->x, p->y, p->w, p->h, c);
}

static inline void draw_ball(const Ball *b, uint16_t c) {
    /* square ball (2r x 2r) for speed */
    fill_rect(b->x - b->r, b->y - b->r, 2*b->r, 2*b->r, c);
}

/* ---- Game ---- */
static void update_ai(Paddle *p) {
    p->y += p->vy;
    if (p->y <= 0 || p->y + p->h >= VGA_HEIGHT) p->vy = -p->vy;
}

static void update_ball(Ball *b, const Paddle *L, const Paddle *R) {
    b->x += b->vx; b->y += b->vy;

    /* Bounce top/bottom */
    if (b->y - b->r <= 0 || b->y + b->r >= VGA_HEIGHT) b->vy = -b->vy;

    /* Paddle collisions */
    if (b->x - b->r <= L->x + L->w && b->y >= L->y && b->y <= L->y + L->h) b->vx = + (b->vx<0 ? -b->vx : b->vx);
    if (b->x + b->r >= R->x       && b->y >= R->y && b->y <= R->y + R->h) b->vx = - (b->vx>0 ?  b->vx : -b->vx);

    /* Score/reset if out of bounds */
    if (b->x < 0 || b->x >= VGA_WIDTH) {
        b->x = VGA_WIDTH/2;
        b->y = VGA_HEIGHT/2;
        b->vx = (b->vx < 0) ? +2 : -2;
        b->vy = (b->vy < 0) ? -1 : +1;
    }
}

int main(void) {
    /* Choose a color format consistent with your backend:
     * - USE_VGA_REGS: send 16-bit RGB565 (common).
     * - USE_VGA_FB (8bpp): send RGB332 for best results.
     */
#if USE_VGA_REGS
    const uint16_t BLACK = RGB565(0,0,0);
    const uint16_t WHITE = RGB565(255,255,255);
#else
  #if VGA_BPP == 1
    const uint16_t BLACK = RGB332(0,0,0);
    const uint16_t WHITE = RGB332(255,255,255);
  #else
    const uint16_t BLACK = RGB565(0,0,0);
    const uint16_t WHITE = RGB565(255,255,255);
  #endif
#endif

    Paddle L = { .x = 10,             .y = VGA_HEIGHT/2 - 30, .w = 6,  .h = 60, .vy =  2 };
    Paddle R = { .x = VGA_WIDTH - 16, .y = VGA_HEIGHT/2 - 30, .w = 6,  .h = 60, .vy = -2 };
    Ball   B = { .x = VGA_WIDTH/2, .y = VGA_HEIGHT/2, .vx = 2, .vy = 1, .r = 4 };

    for (;;) {
        vga_wait_vblank();  /* safe even if it no-ops */
        vga_clear(BLACK);

        update_ai(&L);
        update_ai(&R);
        update_ball(&B, &L, &R);

        draw_paddle(&L, WHITE);
        draw_paddle(&R, WHITE);
        draw_ball(&B, WHITE);

        dtekv_delay(90000); /* crude frame pacing */
    }
}

