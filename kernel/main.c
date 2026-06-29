// 内核初始化函数
// 我们只进行一些颜色显示
#include "fonts/font.h"
#include "printk.h"
#include "include/lib.h"
#include "include/stddef.h"
#include "include/stdbool.h"
#include "include/stdint.h"
#include "gate.h"
#include "trap.h"
#include "test/kerneltest.h"

#define COLOR_RIBBON_WEITH 25
#define GET_BIT(val, n) (((val) >> (n)) & 1)

void kernel_start(void)
{

        init_printing();
        print_tests();

        // 初始化中断向量
        sys_vector_init();
        vector_tests(1);

        while (true)
        {
        }
}
