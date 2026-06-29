#include "mminfo.h"
#include "../printk.h"
#include "../fonts/color.h"
#include "../panic.h"

meminfo mem_info = {0};

void init_memory()
{
        detect_mem_info();

        printk(GREEN, BLACK, "memory info detected: \n");
        printk(KHAKI, BLACK, "\t\tmemory base address: %p", (void *)mem_info.mem_base_addr);
        printk(KHAKI, BLACK, "\t\tmemory length: %lu", mem_info.mem_length);
        printk(KHAKI, BLACK, "\t\tmemory type: %u", mem_info.mem_type);
}

void detect_mem_info()
{
        mem_info.mem_base_addr = *((unsigned long *)(MEM_INFO_START_ADDR));

        mem_info.mem_length = *((unsigned long *)(MEM_INFO_START_ADDR + 8));
        if (mem_info.mem_length == 0)
        {
                panic(PANIC_ERROR, "can't gather memory length!\n");
        }

        mem_info.mem_type = *((unsigned int *)(MEM_INFO_START_ADDR + 16));
        if (mem_info.mem_type == 0)
        {
                panic(PANIC_ERROR, "can't gather memory type!\n");
        }
}