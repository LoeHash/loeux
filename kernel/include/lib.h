#ifndef __LIB_H__
#define __LIB_H__

#include <stddef.h>

/**
 * memset - 将内存区域填满指定字符
 * @ptr:  起始地址
 * @value: 填充字符（按 unsigned char 处理）
 * @num:  字节数
 */
static inline void *memset(void *ptr, int value, size_t num)
{
        unsigned long val8 = (unsigned char)value; /* 低8位扩展 */
        unsigned long val64 = (val8 << 56) | (val8 << 48) |
                              (val8 << 40) | (val8 << 32) |
                              (val8 << 24) | (val8 << 16) |
                              (val8 << 8) | val8; /* 8字节都填满 value */

        unsigned long *p = (unsigned long *)ptr;
        size_t qwords = num / 8;
        size_t tail = num % 8;

        __asm__ volatile(
            "cld\n\t"
            "rep stosq"
            : "+D"(p), "+c"(qwords)
            : "a"(val64)
            : "memory", "cc");

        if (tail)
        {
                unsigned char *cp = (unsigned char *)p;
                do
                {
                        *cp++ = (unsigned char)value;
                } while (--tail);
        }

        return ptr;
}

#endif