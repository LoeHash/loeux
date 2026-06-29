#ifndef __MEMINFO_H_
#define __MEMINFO_H_

#define MEM_INFO_START_ADDR 0x7e00

// 由loader调用bios的中断服务
// 获得的内存结构信息
// 字节偏移    大小        字段              说明
// ─────────────────────────────────────────────────
//   0        8 字节      BaseAddr         内存区域的起始物理地址（64 位）
//   8        8 字节      Length           内存区域的长度（64 位）
//  16        4 字节      Type             内存区域类型（见下表）
//  20        4 字节      Extended Attributes  扩展属性（ACPI 3.0+，可选）
//                                           bit 0: 是否可被非易失性存储忽略
//                                           bit 1: 是否非易失性
//                                 bit 0+1: 00 = 不可用无视
//                                           这些位在传统 E820 中不填充
typedef struct meminfo
{
        unsigned long mem_base_addr;
        unsigned long mem_length;
        unsigned int mem_type;
} meminfo;

extern meminfo mem_info;

void detect_mem_info();
void init_memory(void);
#endif