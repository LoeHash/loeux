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

/**
 * memcpy - 拷贝内存区域（不允许重叠）
 * @dest:  目标地址
 * @src:   源地址
 * @num:   字节数
 *
 * 返回 dest 指针（标准 memcpy 语义）
 */
static inline void *memcpy(void *dest, const void *src, size_t num)
{
        unsigned long *d = (unsigned long *)dest;
        const unsigned long *s = (const unsigned long *)src;
        size_t qwords = num / 8; /* 完整 8 字节块数 */
        size_t tail = num % 8;   /* 剩余字节数 */
        /* 核心拷贝：使用 rep movsq 一次拷贝 8 字节，重复 qwords 次 */
        __asm__ volatile(
            "cld\n\t"
            "rep movsq"
            : "+D"(d), "+c"(qwords), "+S"(s)
            :
            : "memory");
        /* 处理剩余字节（小于 8） */
        unsigned char *cd = (unsigned char *)d;
        const unsigned char *cs = (const unsigned char *)s;
        while (tail--)
        {
                *cd++ = *cs++;
        }
        return dest;
}

#endif