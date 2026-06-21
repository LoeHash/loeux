// 内核初始化函数
// 我们只进行一些颜色显示
#define COLOR_RIBBON_WEITH 25
void kernel_start(void)
{
        int *addr = (int *)0xffff800000a00000;
        int i;

        for (int j = 0; j < 900 / COLOR_RIBBON_WEITH; i++)
        {
                // 第一个色带
                for (i = 0; i < 1440 * 20; i++)
                {
                        *(((char *)(addr)) + 0) = (char)0xff;
                        *(((char *)(addr)) + 1) = (char)0x00;
                        *(((char *)(addr)) + 2) = (char)0x00;
                        *(((char *)(addr)) + 3) = (char)0x00;
                        addr++;
                }

                // 第二个色带
                for (i = 0; i < 1440 * 20; i++)
                {
                        *(((char *)(addr)) + 0) = (char)0x00;
                        *(((char *)(addr)) + 1) = (char)0xff;
                        *(((char *)(addr)) + 2) = (char)0x00;
                        *(((char *)(addr)) + 3) = (char)0x00;
                        addr++;
                }

                // 第三个色带
                for (i = 0; i < 1440 * 20; i++)
                {
                        *(((char *)(addr)) + 0) = (char)0x00;
                        *(((char *)(addr)) + 1) = (char)0x00;
                        *(((char *)(addr)) + 2) = (char)0xff;
                        *(((char *)(addr)) + 3) = (char)0x00;
                        addr++;
                }
        }

        while (1)
        {
                /* code */
        }
}