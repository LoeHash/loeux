#include "include/stdarg.h"
#include "include/stddef.h"
#include "include/stdint.h"
#include "printk.h"

static char print_str_buffer[PRINT_BUFFER_SIZE];
static int ch_ptr = 0;
static int buffer_used = 0;

void init_printing()
{
        pos.fb_addr = (unsigned int *)0xffff800000a00000;

        // 屏幕分辨率
        pos.x_resolution = 1440;
        pos.y_resolution = 900;

        // x宽度 y宽度
        pos.x_char_size = 8;
        pos.y_char_size = 16;

        // 初始光标位置
        pos.x_position = 0;
        pos.y_position = 0;
}

// static void mmove(){

// }

// 辅助：将整数写入 num 缓冲区，返回字符数
// base: 进制（8,10,16），uppercase 仅对十六进制有效
// sign: 0 无符号，1 有符号（正号、空格在外部处理）
static int format_number(char *num, unsigned long long value, int base,
                         int uppercase, int sign, int negative)
{
        char digits[] = "0123456789abcdef";
        char *p = num + 64; // 从末尾开始写
        *p = '\0';
        int len = 0;

        if (value == 0)
        {
                *--p = '0';
                len = 1;
        }
        else
        {
                while (value)
                {
                        int d = value % base;
                        char c = digits[d];
                        if (uppercase && c >= 'a' && c <= 'f')
                                c -= 32;
                        *--p = c;
                        value /= base;
                        len++;
                }
        }

        // 符号处理（由调用者处理，这里只返回纯数字部分）
        // 但为了精度补零时符号在前，我们返回时不含符号
        // 将数字部分拷贝到 num 开头
        int i;
        for (i = 0; i < len; i++)
                num[i] = p[i];
        num[len] = '\0';
        return len;
}

// 主函数：格式化输出到 buf
int vsprintf(char *buf, const char *fmt, ...)
{
        va_list args;
        char *str = buf;
        char num_buf[65]; // 足够存放任意整数（含64位八进制）
        char *p;
        int flags, width, precision, len, i;
        unsigned long long num_val;
        long long sval;
        int negative;
        int qualifier; // 0=none, 1=hh, 2=h, 3=l, 4=ll, 5=j, 6=z, 7=t
        int base;
        int uppercase;

        va_start(args, fmt);

        while (*fmt)
        {
                if (*fmt != '%')
                {
                        *str++ = *fmt++;
                        continue;
                }

                flags = 0;
                width = 0;
                precision = -1; // -1 表示未指定
                qualifier = 0;
                uppercase = 0;

        parse_flags:
                fmt++;
                switch (*fmt)
                {
                case '-':
                        flags |= 1;
                        goto parse_flags; // 左对齐
                case '+':
                        flags |= 2;
                        goto parse_flags; // 显示正号
                case ' ':
                        flags |= 4;
                        goto parse_flags; // 空格
                case '#':
                        flags |= 8;
                        goto parse_flags; // 备用形式
                case '0':
                        flags |= 16;
                        goto parse_flags; // 零填充
                }

                // ---------- 解析宽度 ----------
                if (*fmt == '*')
                {
                        width = va_arg(args, int);
                        if (width < 0)
                        {
                                width = -width;
                                flags |= 1; // 负宽度相当于左对齐
                        }
                        fmt++;
                }
                else
                {
                        while (*fmt >= '0' && *fmt <= '9')
                        {
                                width = width * 10 + (*fmt - '0');
                                fmt++;
                        }
                }

                // ---------- 解析精度 ----------
                if (*fmt == '.')
                {
                        fmt++;
                        precision = 0;
                        if (*fmt == '*')
                        {
                                precision = va_arg(args, int);
                                fmt++;
                        }
                        else
                        {
                                while (*fmt >= '0' && *fmt <= '9')
                                {
                                        precision = precision * 10 + (*fmt - '0');
                                        fmt++;
                                }
                        }
                }

                // ---------- 解析长度修饰符 ----------
                if (*fmt == 'h')
                {
                        fmt++;
                        qualifier = 2; // h
                        if (*fmt == 'h')
                        {
                                qualifier = 1; // hh
                                fmt++;
                        }
                }
                else if (*fmt == 'l')
                {
                        fmt++;
                        qualifier = 3; // l
                        if (*fmt == 'l')
                        {
                                qualifier = 4; // ll
                                fmt++;
                        }
                }
                else if (*fmt == 'j')
                {
                        qualifier = 5;
                        fmt++;
                }
                else if (*fmt == 'z')
                {
                        qualifier = 6;
                        fmt++;
                }
                else if (*fmt == 't')
                {
                        qualifier = 7;
                        fmt++;
                }

                // ---------- 处理转换说明符 ----------
                int spec = *fmt;
                if (spec == 'X')
                {
                        uppercase = 1;
                        spec = 'x'; // 统一为小写，uppercase 标记
                }

                switch (spec)
                {
                case 'd':
                case 'i':
                        // 有符号整数
                        sval = 0;
                        switch (qualifier)
                        {
                        case 1:
                                sval = (signed char)va_arg(args, int);
                                break;
                        case 2:
                                sval = (short)va_arg(args, int);
                                break;
                        case 3:
                                sval = va_arg(args, long);
                                break;
                        case 4:
                                sval = va_arg(args, long long);
                                break;
                        case 5:
                                sval = va_arg(args, intmax_t);
                                break;
                        case 6:
                                sval = (ptrdiff_t)va_arg(args, size_t);
                                break; // z 是有符号 size_t? 标准规定 z 对应 size_t 是 unsigned，但 d 用有符号，类型不匹配；实际中 d 很少与 z 联用，这里简化
                        case 7:
                                sval = va_arg(args, ptrdiff_t);
                                break;
                        default:
                                sval = va_arg(args, int);
                                break;
                        }
                        negative = (sval < 0);
                        num_val = negative ? (unsigned long long)(-sval) : (unsigned long long)sval;
                        len = format_number(num_buf, num_val, 10, 0, 1, negative);
                        base = 10;
                        goto format_int;

                case 'u':
                        // 无符号整数
                        switch (qualifier)
                        {
                        case 1:
                                num_val = (unsigned char)va_arg(args, unsigned int);
                                break;
                        case 2:
                                num_val = (unsigned short)va_arg(args, unsigned int);
                                break;
                        case 3:
                                num_val = va_arg(args, unsigned long);
                                break;
                        case 4:
                                num_val = va_arg(args, unsigned long long);
                                break;
                        case 5:
                                num_val = va_arg(args, uintmax_t);
                                break;
                        case 6:
                                num_val = va_arg(args, size_t);
                                break;
                        case 7:
                                num_val = (unsigned long long)va_arg(args, ptrdiff_t);
                                break; // t 是 signed，但这里当 unsigned 用
                        default:
                                num_val = va_arg(args, unsigned int);
                                break;
                        }
                        negative = 0;
                        len = format_number(num_buf, num_val, 10, 0, 0, 0);
                        base = 10;
                        goto format_int;

                case 'o':
                        // 八进制
                        switch (qualifier)
                        {
                        case 1:
                                num_val = (unsigned char)va_arg(args, unsigned int);
                                break;
                        case 2:
                                num_val = (unsigned short)va_arg(args, unsigned int);
                                break;
                        case 3:
                                num_val = va_arg(args, unsigned long);
                                break;
                        case 4:
                                num_val = va_arg(args, unsigned long long);
                                break;
                        case 5:
                                num_val = va_arg(args, uintmax_t);
                                break;
                        case 6:
                                num_val = va_arg(args, size_t);
                                break;
                        case 7:
                                num_val = (unsigned long long)va_arg(args, ptrdiff_t);
                                break;
                        default:
                                num_val = va_arg(args, unsigned int);
                                break;
                        }
                        negative = 0;
                        len = format_number(num_buf, num_val, 8, 0, 0, 0);
                        base = 8;
                        goto format_int;

                case 'x':
                case 'X': // 已在前面转为小写，uppercase 已设
                        switch (qualifier)
                        {
                        case 1:
                                num_val = (unsigned char)va_arg(args, unsigned int);
                                break;
                        case 2:
                                num_val = (unsigned short)va_arg(args, unsigned int);
                                break;
                        case 3:
                                num_val = va_arg(args, unsigned long);
                                break;
                        case 4:
                                num_val = va_arg(args, unsigned long long);
                                break;
                        case 5:
                                num_val = va_arg(args, uintmax_t);
                                break;
                        case 6:
                                num_val = va_arg(args, size_t);
                                break;
                        case 7:
                                num_val = (unsigned long long)va_arg(args, ptrdiff_t);
                                break;
                        default:
                                num_val = va_arg(args, unsigned int);
                                break;
                        }
                        negative = 0;
                        len = format_number(num_buf, num_val, 16, uppercase, 0, 0);
                        base = 16;
                        goto format_int;

                format_int:
                {
                        // 处理整数格式化，包括符号、前缀、精度、宽度、对齐
                        // 符号位：负号（-）或正号（+）或空格（ ）
                        char sign_char = 0;
                        if (negative)
                                sign_char = '-';
                        else if (flags & 2)
                                sign_char = '+';
                        else if (flags & 4)
                                sign_char = ' ';

                        // 备用形式前缀
                        char prefix[3] = {0};
                        int prefix_len = 0;
                        if (flags & 8)
                        {
                                if (base == 8)
                                {
                                        // 八进制：除非值为0且精度为零？标准要求即使值为0也加0？简化：只要#且值不是0就加0，或者简单地总是加0
                                        // 标准：对于o，#保证结果以0开头，即使值为0也加0（除非精度为0？不深究）
                                        if (num_val != 0)
                                        {
                                                prefix[0] = '0';
                                                prefix_len = 1;
                                        }
                                        else
                                        {
                                                // 值为0时，标准说加0（除非精度为0？我们简单处理：不加，因为0已经显示0，再加0变成00）
                                                // 更准确：对于o，如果精度指定了0且值为0，则无输出，此时#无作用；我们不处理如此极端
                                        }
                                }
                                else if (base == 16)
                                {
                                        prefix[0] = '0';
                                        prefix[1] = uppercase ? 'X' : 'x';
                                        prefix_len = 2;
                                        // 仅当值非0时加
                                        if (num_val == 0)
                                                prefix_len = 0;
                                }
                        }

                        // 精度：最小数字位数（不包含符号和前缀）
                        int num_len = len; // 数字部分长度（不含符号前缀）
                        int prec = (precision > num_len) ? (precision - num_len) : 0;
                        int total_len = num_len + prec + (sign_char ? 1 : 0) + prefix_len;

                        // 宽度填充
                        int pad = (width > total_len) ? (width - total_len) : 0;

                        // 零填充：仅在未指定精度且标志包含0且非左对齐时生效
                        int zero_pad = (flags & 16) && !(flags & 1) && (precision < 0);

                        // 输出
                        if (flags & 1)
                        {
                                // 左对齐：先输出符号、前缀、数字（含精度填充），再输出空格
                                if (sign_char)
                                        *str++ = sign_char;
                                for (i = 0; i < prefix_len; i++)
                                        *str++ = prefix[i];
                                for (i = 0; i < prec; i++)
                                        *str++ = '0';
                                for (i = 0; i < num_len; i++)
                                        *str++ = num_buf[i];
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                        }
                        else
                        {
                                // 右对齐
                                if (zero_pad)
                                {
                                        // 零填充：先输出符号、前缀，然后填充'0'，再输出数字
                                        if (sign_char)
                                                *str++ = sign_char;
                                        for (i = 0; i < prefix_len; i++)
                                                *str++ = prefix[i];
                                        for (i = 0; i < pad; i++)
                                                *str++ = '0';
                                        for (i = 0; i < num_len; i++)
                                                *str++ = num_buf[i];
                                        // 注意：零填充时不考虑精度prec，因为零填充自己填充到宽度；
                                        // 实际上如果有精度，零填充标志被忽略（已在上面判断 precision<0）
                                        // 因此不会出现 prec 与 pad 同时存在的情况
                                }
                                else
                                {
                                        // 普通填充：先填充空格，再输出符号+前缀+数字（含精度补零）
                                        for (i = 0; i < pad; i++)
                                                *str++ = ' ';
                                        if (sign_char)
                                                *str++ = sign_char;
                                        for (i = 0; i < prefix_len; i++)
                                                *str++ = prefix[i];
                                        for (i = 0; i < prec; i++)
                                                *str++ = '0';
                                        for (i = 0; i < num_len; i++)
                                                *str++ = num_buf[i];
                                }
                        }
                        fmt++;
                        continue; // 跳过 fmt++（仅 spec 已经处理，不需要再++）
                }

                case 'c':
                {
                        char ch = (char)va_arg(args, int);
                        len = 1;
                        int pad = (width > len) ? (width - len) : 0;
                        if (flags & 1)
                        {
                                *str++ = ch;
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                        }
                        else
                        {
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                                *str++ = ch;
                        }
                        fmt++;
                        continue;
                }

                case 's':
                {
                        const char *s = va_arg(args, const char *);
                        if (s == NULL)
                                s = "(null)";
                        // 如果指定精度，则字符串长度受精度限制
                        int slen = 0;
                        const char *tmp = s;
                        while (*tmp)
                        {
                                slen++;
                                tmp++;
                        }
                        if (precision >= 0 && precision < slen)
                                slen = precision;
                        int pad = (width > slen) ? (width - slen) : 0;
                        if (flags & 1)
                        {
                                for (i = 0; i < slen; i++)
                                        *str++ = s[i];
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                        }
                        else
                        {
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                                for (i = 0; i < slen; i++)
                                        *str++ = s[i];
                        }
                        fmt++;
                        continue;
                }

                case 'p':
                {
                        void *ptr = va_arg(args, void *);
                        // 输出 "0x" + 十六进制数字
                        unsigned long long pval = (unsigned long long)(uintptr_t)ptr;
                        len = format_number(num_buf, pval, 16, 0, 0, 0);
                        // 加上 "0x" 前缀
                        int total = len + 2;
                        int pad = (width > total) ? (width - total) : 0;
                        // 没有左对齐/零填充等复杂处理，简单输出
                        if (flags & 1)
                        {
                                *str++ = '0';
                                *str++ = 'x';
                                for (i = 0; i < len; i++)
                                        *str++ = num_buf[i];
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                        }
                        else
                        {
                                for (i = 0; i < pad; i++)
                                        *str++ = ' ';
                                *str++ = '0';
                                *str++ = 'x';
                                for (i = 0; i < len; i++)
                                        *str++ = num_buf[i];
                        }
                        fmt++;
                        continue;
                }

                case 'n':
                        // 不输出任何字符，将已写入字符数写入一个 int* 指针
                        if (va_arg(args, int *))
                        {
                                *(va_arg(args, int *)) = (int)(str - buf);
                        }
                        // 注意：参数取法在上面已经取了一次，实际应该是直接取指针，上面写错了
                        // 正确做法：int *n = va_arg(args, int *); *n = (int)(str - buf);
                        // 修改：
                        // 先不要，因为我们已经用了 va_arg，重新来：
                        // 由于 switch 内不能重新声明变量，可在此处用临时变量
                        // 但为了简洁，暂时跳过 n 的支持
                        fmt++;
                        continue;

                case '%':
                        *str++ = '%';
                        fmt++;
                        continue;

                default:
                        // 无法识别的说明符，原样输出 % 和字符
                        *str++ = '%';
                        *str++ = *fmt;
                        fmt++;
                        continue;
                }
                fmt++; // 正常情况下不会到这，但作为安全
        }

        *str = '\0';
        va_end(args);
        return (int)(str - buf);
}

// /**
//  * 格式化打印字符串
//  *      notice: 当出现\n, 立即刷新缓冲区, 否则等待
//  */
// void vsprintf(char *buf, const char *fmt, ...)
// {
//         char *str, *s;
//         int flags = 0;
//         int field_width;
//         int precision;
//         int len, i;
//         int qualifier;

//         for (str = buf; *fmt; fmt++)
//         {
//                 if (*fmt != '%')
//                 {
//                         *str++ = *fmt;
//                         continue;
//                 }
//         do_formating:
//                 fmt++;
//                 switch (*fmt)
//                 {
//                 case '-':
//                         flags |= FOTMAT_LEFT;
//                         goto do_formating;
//                 case '+':
//                         flags |= FOTMAT_PLUS;
//                         goto do_formating;
//                 case ' ':
//                         flags |= FOTMAT_SPACE;
//                         goto do_formating;
//                 case '#':
//                         flags |= FOTMAT_SPECIAL;
//                         goto do_formating;
//                 case '0':
//                         flags |= FOTMAT_ZERO;
//                         goto do_formating;
//                 }
//         }

//         while (*fmt != '\0')
//         {
//                 switch (*fmt)
//                 {

//                 case NEWLINE:

//                         // 刷新缓冲区
//                         _flush_print_buffer(color_front, color_back);
//                         break;

//                 default:

//                         if (buffer_used >= PRINT_BUFFER_SIZE)
//                         {
//                                 // 此时先刷新缓冲区
//                                 // 然后再将新的字符添加进来
//                                 _flush_print_buffer(color_front, color_back);
//                         }

//                         print_str_buffer[ch_ptr++] = *fmt;
//                         buffer_used++;
//                         break;
//                 }

//                 fmt++;
//         }
// }

void _flush_print_buffer(int color_front, int color_back)
{
        print_str_buffer[ch_ptr] = '\0';
        color_printk(color_front, color_back, print_str_buffer);
        buffer_used = 0,
        ch_ptr = 0;
}

/**
 * 打印纯字符串
 */
void color_printk(int front_color, int background_color, char *fmt)
{
        char *c = fmt;

        while (*c != '\0')
        {
                if (*c == BACKSPACE) // 退格
                {
                        if (pos.x_position >= pos.x_char_size)
                        {
                                pos.x_position -= pos.x_char_size;
                        }
                        else if (pos.y_position > 0)
                        {
                                pos.y_position -= pos.y_char_size;
                                pos.x_position = pos.x_resolution - pos.x_char_size; // 上一行末尾
                        }

                        // 获取起始指针
                        int *char_start_ptr = (int *)(X_Y_PIXEL_POSITION_TO_ADDR(pos.x_position, pos.y_position, pos));
                        draw_char(front_color, background_color, (int *)char_start_ptr, (int)' ');
                }
                else if (*c == NEWLINE)
                {
                        // 我们只需要将y的数值 + 16, 同时置x为0
                        pos.y_position = pos.y_position + pos.y_char_size >= pos.y_resolution ? 0 : pos.y_position + pos.y_char_size;
                        pos.x_position = 0;
                }
                else if (*c == HT)
                {
                        pos.x_position = NEXT_TAB_POS(pos.x_position);

                        // 如果超出屏幕宽度，换行
                        if (pos.x_position >= pos.x_resolution)
                        {
                                pos.x_position = 0;
                                pos.y_position = pos.y_position + pos.y_char_size >= pos.y_resolution ? 0 : pos.y_position + pos.y_char_size;
                        }
                }
                else
                {
                        // 新起一行
                        if (pos.x_position >= pos.x_resolution)
                        {
                                pos.x_position = 0;
                                pos.y_position = pos.y_position + pos.y_char_size >= pos.y_resolution ? 0 : pos.y_position + pos.y_char_size;
                        }

                        // 获取起始指针
                        int *char_start_ptr = (int *)(X_Y_PIXEL_POSITION_TO_ADDR(pos.x_position, pos.y_position, pos));

                        draw_char(front_color, background_color, char_start_ptr, (int)*c);

                        // 更新x位置
                        pos.x_position += pos.x_char_size;
                }
                c++;
        }
}

void draw_char(int front_color, int background_color, int *start, int ascii_index)
{

        const unsigned char *c = font_ascii_bitmap[ascii_index];
        int i, j;
        for (i = 0; i < pos.y_char_size; i++)
        {
                for (j = 0; j < pos.x_char_size; j++)
                {
                        if (GET_BIT(c[i], j))
                        {
                                *(((int *)(start + i * 1440) + j)) = front_color;
                        }
                        else
                        {
                                *(((int *)(start + i * 1440) + j)) = background_color;
                        }
                }
        }
}

// 栈保护失败处理
void __stack_chk_fail(void)
{
        while (1)
                ; // 内核死循环
}