#ifndef _INC_PRINTK
#define _INC_PRINTK

#include "include/stdarg.h"
#include "fonts/font.h"
#define NULL ((void *)0)
#define CHAR_B 0x08
#define CHAR_N 0x0A
#define CHAR_T 0x09
// #define X_Y_CHAR_POSITION_TO_ADDR(x, y, pos) ((pos.fb_addr + y * pos.y_char_size * 1440) + x * pos.y_char_size)
#define X_Y_PIXEL_POSITION_TO_ADDR(x, y, pos) ((pos.fb_addr + y * 1440) + x)
#define GET_BIT(val, n) (((val) >> (n)) & 1)
#define NEXT_TAB_POS(x) (((x) / 8 / 8) + 1) * 8 * 8

typedef struct position
{
        int x_resolution;
        int y_resolution;

        int x_position;
        int y_position;

        int x_char_size;
        int y_char_size;

        unsigned int *fb_addr;
        // unsigned long *fb_length;
} position;

static position pos = {0};

void init_printing();

void color_printk(char *fmt);

void draw_char(int *start, int ascii_index);

#endif