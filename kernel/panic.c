#include "panic.h"
#include "printk.h"
#include "fonts/color.h"
#include "include/stdarg.h"

void print_notice_flag(int panic_id)
{
        color_printk(WHITE, BLACK, "[");
        switch (panic_id)
        {
        case PANIC_ERROR: // error
                color_printk(RED, BLACK, "ERROR");
                break;
        case PANIC_WRONG: // wrong
                color_printk(ORANGE, BLACK, "WRONG");
                break;
        default:
                color_printk(RED, BLACK, "!");
                break;
        }
        color_printk(WHITE, BLACK, "] ");
}

void panic(int panic_id, char *reason, ...)
{
        print_notice_flag(panic_id);

        va_list args;
        char buf[PRINT_BUFFER_SIZE];
        va_start(args, reason);
        vsprintf(buf, reason, args); // 将 va_list 传递给 vsprintf
        va_end(args);
        color_printk(PANIC_FRONT_COLOR, PANIC_BACK_COLOR, buf);

        if (panic_id == 1)
        {
                while (1)
                        ;
        }
}