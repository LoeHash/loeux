#ifndef _INC_PRINTK
#define _INC_PRINTK

#include "include/stdarg.h"
#include "fonts/font.h"
#define NULL ((void *)0)

#define BACKSPACE 0x08
#define HT 0x09
#define NEWLINE 0x0A

#define PRINT_BUFFER_SIZE 1024
#define FOTMAT_LEFT 1 << 0
#define FOTMAT_PLUS 1 << 1
#define FOTMAT_SPACE 1 << 2
#define FOTMAT_SPECIAL 1 << 3
#define FOTMAT_ZERO 1 << 4

#define X_Y_CHAR_POSITION_TO_ADDR(x, y, pos) ((pos.fb_addr + y * pos.y_char_size * 1440) + x * pos.y_char_size)
#define X_Y_PIXEL_POSITION_TO_ADDR(x, y, pos) ((pos.fb_addr + y * 1440) + x)
#define GET_BIT(val, n) (((val) >> (n)) & 1)
#define NEXT_TAB_POS(x) ((((x) / 64) + 1) * 64)

typedef struct position
{
        int x_resolution;
        int y_resolution;

        unsigned int x_position;
        unsigned int y_position;

        int x_char_size;
        int y_char_size;

        unsigned int *fb_addr;
        // unsigned long *fb_length;
} position;

static position pos = {0};

void init_printing();

int vsprintf(char *buf, const char *fmt, ...);

void color_printk(int front_color, int background_color, char *fmt);

void draw_char(int front_color, int background_color, int *start, int ascii_index);

#endif