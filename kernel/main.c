// 内核初始化函数
// 我们只进行一些颜色显示
#include "fonts/font.h"
#include "printk.h"

#define COLOR_RIBBON_WEITH 25
#define GET_BIT(val, n) (((val) >> (n)) & 1)

void kernel_start(void)
{

        init_printing();
        color_printk("Booting The Loeux Kernel.... Please Wait...");
        while (1)
        {
                // 空循环
        }
}
