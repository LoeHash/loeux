#include "../fonts/font.h"
#include "../printk.h"
#include "../include/lib.h"
#include "../include/stddef.h"
#include "../include/stdbool.h"
#include "../include/stdint.h"
#include "../gate.h"
#include "../trap.h"

void print_tests()
{
        color_printk(0xcd3333, 0x0A0C0E, "Booting The Loeux Kernel.... Please Wait...");

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
        color_printk(0xff8800, 0x0A0C0E, "Line 3: Kernel is running!\n");

        // 测试6：制表符对齐效果
        color_printk(0xffaa00, 0x0A0C0E, "\nTab Alignment Test:\n");
        color_printk(0xffaa00, 0x0A0C0E, "Name\tAge\tScore\n");
        color_printk(0xff8800, 0x0A0C0E, "Line 2: World\n");
        color_printk(0xffaa00, 0x0A0C0E, "Bob\t30\t87\n");
        color_printk(0xffaa00, 0x0A0C0E, "Charlie\t22\t92\n");

        // 测试7：长字符串测试
        color_printk(0x88ff88, 0x0A0C0E, "\nLong string test:\n");
        color_printk(0x88ff88, 0x0A0C0E, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
        color_printk(0xffaa00, 0x0A0C0E, "Alice\t25\t95\n");

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

void vsprintf_tests(void)
{
        char buf[256];
        int i;

        // ---------- 1. 纯字符串和 %% ----------
        vsprintf(buf, "hello");
        if (buf[0] != 'h' || buf[1] != 'e' || buf[2] != 'l' || buf[3] != 'l' || buf[4] != 'o' || buf[5] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: simple string\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%%");
        if (buf[0] != '%' || buf[1] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: %%\n");
                while (1)
                        ;
        }

        vsprintf(buf, "a%%b%%c");
        if (buf[0] != 'a' || buf[1] != '%' || buf[2] != 'b' || buf[3] != '%' || buf[4] != 'c' || buf[5] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: a%%b%%c\n");
                while (1)
                        ;
        }

        // ---------- 2. %c ----------
        vsprintf(buf, "%c", 'A');
        if (buf[0] != 'A' || buf[1] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: %%c 'A'\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%5c", 'A');
        if (buf[0] != ' ' || buf[1] != ' ' || buf[2] != ' ' || buf[3] != ' ' || buf[4] != 'A' || buf[5] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: %%5c\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%-5c", 'A');
        if (buf[0] != 'A' || buf[1] != ' ' || buf[2] != ' ' || buf[3] != ' ' || buf[4] != ' ' || buf[5] != '\0')
        {
                color_printk(0xffffff, 0, "FAIL: %%-5c\n");
                while (1)
                        ;
        }

        // ---------- 3. %s ----------
        vsprintf(buf, "%s", "hello");
        if (strcmp(buf, "hello") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%s hello\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%.4s", "hello");
        if (strcmp(buf, "hell") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%.4s\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%7s", "hi");
        if (strcmp(buf, "     hi") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%7s\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%-7s", "hi");
        if (strcmp(buf, "hi     ") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%-7s\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%6.3s", "hello");
        if (strcmp(buf, "   hel") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%6.3s\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%s", (char *)0);
        if (strcmp(buf, "(null)") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%s NULL\n");
                while (1)
                        ;
        }

        // ---------- 4. %d / %i ----------
        vsprintf(buf, "%d", 0);
        if (strcmp(buf, "0") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%d 0\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%d", 42);
        if (strcmp(buf, "42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%d", -42);
        if (strcmp(buf, "-42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%d -42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%8d", 42);
        if (strcmp(buf, "      42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%8d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%-8d", 42);
        if (strcmp(buf, "42      ") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%-8d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%08d", 42);
        if (strcmp(buf, "00000042") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%08d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%+d", 42);
        if (strcmp(buf, "+42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%+d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%+d", -42);
        if (strcmp(buf, "-42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%+d -42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "% d", 42);
        if (strcmp(buf, " 42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %% d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "% d", -42);
        if (strcmp(buf, "-42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %% d -42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%.5d", 42);
        if (strcmp(buf, "00042") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%.5d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%.5d", -42);
        if (strcmp(buf, "-00042") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%.5d -42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%8.5d", 42);
        if (strcmp(buf, "   00042") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%8.5d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%08.5d", 42);
        if (strcmp(buf, "   00042") != 0)
        { // 精度存在时零填充被忽略
                color_printk(0xffffff, 0, "FAIL: %%08.5d 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%.0d", 0);
        if (strcmp(buf, "") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%.0d 0\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%+.0d", 0); // 正号+精度0且值为0 => 只输出符号
        // 标准行为：+0 精度0 => 输出 "+"（符号+无数字）
        if (strcmp(buf, "+") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%+.0d 0\n");
                while (1)
                        ;
        }

        // ---------- 5. %u ----------
        vsprintf(buf, "%u", 42);
        if (strcmp(buf, "42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%u 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%u", -1u);
        if (strcmp(buf, "4294967295") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%u -1\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%08u", 42);
        if (strcmp(buf, "00000042") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%08u 42\n");
                while (1)
                        ;
        }

        // ---------- 6. %o ----------
        vsprintf(buf, "%o", 042);
        if (strcmp(buf, "42") != 0)
        { // 注意：042是八进制，但格式化为十进制？错了，数字 042 本身就是八进制整型 34 十进制。测试应使用 42
                // 重新测试：实际数字 42 的八进制是 "52"
                vsprintf(buf, "%o", 42);
                if (strcmp(buf, "52") != 0)
                {
                        color_printk(0xffffff, 0, "FAIL: %%o 42\n");
                        while (1)
                                ;
                }
        }
        else
        {
                // 上面如果错了，现在改测
                vsprintf(buf, "%o", 42);
                if (strcmp(buf, "52") != 0)
                {
                        color_printk(0xffffff, 0, "FAIL: %%o 42\n");
                        while (1)
                                ;
                }
        }
        vsprintf(buf, "%#o", 42);
        if (strcmp(buf, "052") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#o 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%#o", 0);
        if (strcmp(buf, "0") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#o 0\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%08o", 42);
        if (strcmp(buf, "00000052") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%08o 42\n");
                while (1)
                        ;
        }

        // ---------- 7. %x / %X ----------
        vsprintf(buf, "%x", 42);
        if (strcmp(buf, "2a") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%x 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%X", 42);
        if (strcmp(buf, "2A") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%X 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%#x", 42);
        if (strcmp(buf, "0x2a") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#x 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%#X", 42);
        if (strcmp(buf, "0X2A") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#X 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%#x", 0);
        if (strcmp(buf, "0") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#x 0\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%08x", 42);
        if (strcmp(buf, "0000002a") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%08x 42\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%#08x", 42);
        if (strcmp(buf, "0x00002a") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#08x 42\n");
                while (1)
                        ;
        }

        // ---------- 8. %p ----------
        vsprintf(buf, "%p", (void *)0);
        if (strcmp(buf, "0x0") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%p NULL\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%p", (void *)0x7fff1234);
        if (strcmp(buf, "0x7fff1234") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%p 0x7fff1234\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%12p", (void *)0x1234);
        if (strcmp(buf, "     0x1234") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%12p\n");
                while (1)
                        ;
        }
        color_printk(0xffffff, 0, "FAIL: %%hhd 127\n");
        vsprintf(buf, "%-12p", (void *)0x1234);
        if (strcmp(buf, "0x1234     ") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%-12p\n");
                while (1)
                        ;
        }

        // ---------- 9. 长度修饰符 ----------
        // hh
        vsprintf(buf, "%hhd", (signed char)127);
        if (strcmp(buf, "127") != 0)
        {
                while (1)
                        ;
        }
        vsprintf(buf, "%hhd", (signed char)-128);
        if (strcmp(buf, "-128") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%hhd -128\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%hhu", (unsigned char)-1);
        if (strcmp(buf, "255") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%hhu 255\n");
                while (1)
                        ;
        }

        // h
        vsprintf(buf, "%hd", (short)32767);
        if (strcmp(buf, "32767") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%hd 32767\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%hd", (short)-32768);
        if (strcmp(buf, "-32768") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%hd -32768\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%hu", (unsigned short)-1);
        if (strcmp(buf, "65535") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%hu 65535\n");
                while (1)
                        ;
        }

        // l
        vsprintf(buf, "%ld", 123456789L);
        if (strcmp(buf, "123456789") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%ld 123456789\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%ld", -123456789L);
        if (strcmp(buf, "-123456789") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%ld -123456789\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%lu", 4000000000UL);
        if (strcmp(buf, "4000000000") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%lu 4000000000\n");
                while (1)
                        ;
        }

        // ll
        vsprintf(buf, "%lld", 1000000000000LL);
        if (strcmp(buf, "1000000000000") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%lld 1e12\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%llu", 18446744073709551615ULL);
        if (strcmp(buf, "18446744073709551615") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%llu max\n");
                while (1)
                        ;
        }

        // j, z, t  (intmax_t, size_t, ptrdiff_t)
        vsprintf(buf, "%jd", (intmax_t)-123);
        if (strcmp(buf, "-123") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%jd\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%zu", (size_t)42);
        if (strcmp(buf, "42") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%zu\n");
                while (1)
                        ;
        }
        vsprintf(buf, "%td", (ptrdiff_t)(-7));
        if (strcmp(buf, "-7") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%td\n");
                while (1)
                        ;
        }

        // ---------- 10. 综合复杂格式 ----------
        vsprintf(buf, "|%+08.5d|", 123);
        // 精度5 => 00123，正号，零填充但精度存在时零填充被忽略，宽度8 => 总宽度8，左对齐？ 无- => 右对齐。总长度：符号+ 5位 = 6，补2空格 => " +00123" 但标准：+ 和 0 填充组合：精度存在时0填充无效，所以是 "  +00123" (宽度8，右对齐)
        // 期望: 空格+空格+空格？ 实际：宽度8，内容长度1(符号)+5(精度补零后的数字)=6，两个空格填充，右对齐 => "  +00123"
        if (strcmp(buf, "  +00123") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%+08.5d 123\n");
                while (1)
                        ;
        }

        vsprintf(buf, "%-+10.3d", -5);
        // 左对齐，正号? -5 应为 "-005"，宽度10 => 左侧内容 " -005"？ 符号 - 精度3 => -005，左对齐 => "-005     " 后补5空格？ 内容长度：1符号+3数字=4， total=4，宽度10，补6空格左对齐 => "-005      " (6个空格)
        if (strcmp(buf, "-005      ") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%-+10.3d -5\n");
                while (1)
                        ;
        }

        // ---------- 11. 边界大整数 ----------
        vsprintf(buf, "%#x", 0x7fffffff);
        if (strcmp(buf, "0x7fffffff") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: %%#x max int\n");
                while (1)
                        ;
        }

        // ---------- 12. 多参数混合 ----------
        vsprintf(buf, "int=%d char=%c str=%s", -100, 'Z', "test");
        if (strcmp(buf, "int=-100 char=Z str=test") != 0)
        {
                color_printk(0xffffff, 0, "FAIL: mixed\n");
                while (1)
                        ;
        }

        color_printk(0xffffff, 0, "All tests passed!\n");
        return;
}
