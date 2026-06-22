// 内核初始化函数
// 我们只进行一些颜色显示
#include "fonts/font.h"
#include "printk.h"
#define COLOR_RIBBON_WEITH 25
#define GET_BIT(val, n) (((val) >> (n)) & 1)

static void print_tests();

void kernel_start(void)
{

        init_printing();
        print_tests();
        while (1)
        {
                // 空循环
        }
}

void print_tests()
{
        color_printk(0x00ff00, 0x0A0C0E, "Booting The Loeux Kernel.... Please Wait...");

        // 测试2：制表符测试
        color_printk(0x00ffff, 0x0A0C0E, "\n\nTab Test:\n");
        color_printk(0xffffff, 0x0A0C0E, "Start\tEnd\n");
        color_printk(0xffffff, 0x0A0C0E, "1\t2\t3\t4\n");
        color_printk(0xffffff, 0x0A0C0E, "AAAAA\tB\tC\tD\tE\n");

        // 测试3：颜色测试
        color_printk(0xff0000, 0x0A0C0E, "\nColor Test:\n");
        color_printk(0xff0000, 0x0A0C0E, "Red ");
        color_printk(0x00ff00, 0x0A0C0E, "Green ");
        color_printk(0x0000ff, 0x0A0C0E, "Blue ");
        color_printk(0xffff00, 0x0A0C0E, "Yellow ");
        color_printk(0xff00ff, 0x0A0C0E, "Magenta ");
        color_printk(0x00ffff, 0x0A0C0E, "Cyan\n");

        // 测试4：数字和符号测试
        color_printk(0xffffff, 0x0A0C0E, "\nNumbers & Symbols:\n");
        color_printk(0xffffff, 0x0A0C0E, "0123456789\n");
        color_printk(0xffffff, 0x0A0C0E, "!@#$%%^&*()_+{}|:\"<>?~`\n");

        // 测试5：多行输出
        color_printk(0xff8800, 0x0A0C0E, "\nMulti-line test:\n");
        color_printk(0xff8800, 0x0A0C0E, "Line 1: Hello\n");
        color_printk(0xff8800, 0x0A0C0E, "Line 2: World\n");
        color_printk(0xff8800, 0x0A0C0E, "Line 3: Kernel is running!\n");

        // 测试6：制表符对齐效果
        color_printk(0xffaa00, 0x0A0C0E, "\nTab Alignment Test:\n");
        color_printk(0xffaa00, 0x0A0C0E, "Name\tAge\tScore\n");
        color_printk(0xffaa00, 0x0A0C0E, "Alice\t25\t95\n");
        color_printk(0xffaa00, 0x0A0C0E, "Bob\t30\t87\n");
        color_printk(0xffaa00, 0x0A0C0E, "Charlie\t22\t92\n");

        // 测试7：长字符串测试
        color_printk(0x88ff88, 0x0A0C0E, "\nLong string test:\n");
        color_printk(0x88ff88, 0x0A0C0E, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n");

        // 测试8：边界测试（制表符在行尾）
        color_printk(0xffffff, 0x0A0C0E, "\nTab at end of line:\n");
        color_printk(0xffffff, 0x0A0C0E, "12345678901234567890123456789012345678901234567890123456789012345678901234567890\tEnd\n");

        // 测试9：多个制表符
        color_printk(0x88ddff, 0x0A0C0E, "\nMultiple tabs:\n");
        color_printk(0x88ddff, 0x0A0C0E, "\t\t\tThree tabs\n");
        color_printk(0x88ddff, 0x0A0C0E, "\t\t\t\tFour tabs\n");

        // 测试10：最终状态
        color_printk(0x00ff00, 0x0A0C0E, "\n\n[OK] All tests completed!\n");
        color_printk(0xffff00, 0x0A0C0E, "[INFO] Kernel is running...\n");
}
