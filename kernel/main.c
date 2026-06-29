// 内核初始化函数
// 我们只进行一些颜色显示
#include "fonts/font.h"
#include "printk.h"
#include "include/lib.h"
#include "include/stddef.h"
#include "include/stdbool.h"
#include "include/stdint.h"
#include "mm/mminfo.h"
#include "test/kerneltest.h"
#include "gate.h"
#include "trap.h"

#define COLOR_RIBBON_WEITH 25
#define GET_BIT(val, n) (((val) >> (n)) & 1)

void kernel_start(void)
{

        // 初始化打印操作
        init_printing();

        printk(0xcd3333, 0x0A0C0E, "Booting The Loeux Kernel.... Please Wait...\n");

        // 初始化中断向量
        init_sys_vector();

        // 初始化内存模块
        init_memory();

        while (true)
        {
        }
}
