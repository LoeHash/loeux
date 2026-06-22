#include "include/stdarg.h"
#include "printk.h"

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
                        pos.y_position += pos.y_char_size;
                        pos.x_position = 0;
                }
                else if (*c == HT)
                {
                        pos.x_position = NEXT_TAB_POS(pos.x_position);

                        // 如果超出屏幕宽度，换行
                        if (pos.x_position >= pos.x_resolution)
                        {
                                pos.x_position = 0;
                                pos.y_position += pos.y_char_size;
                        }
                }
                else
                {
                        // 新起一行
                        if (pos.x_position >= pos.x_resolution)
                        {
                                pos.x_position = 0;
                                pos.y_position += pos.y_char_size;
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