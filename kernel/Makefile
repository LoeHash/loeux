# 自动查找当前目录下所有.c文件
C_SOURCES = $(wildcard *.c)
C_OBJECTS = $(C_SOURCES:.c=.o)

all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

system: head.o $(C_OBJECTS)
	ld -b elf64-x86-64 -o system head.o $(C_OBJECTS) -T Kernel.lds 

# 通用规则：将所有.c编译为.o
%.o: %.c
	gcc -mcmodel=large -fno-builtin -m64 -c $< -o $@

head.o: head.S
	gcc -E head.S > head.s
	as --64 -o head.o head.s

clean:
	rm -rf *.o *.s~ *.s *.S~ *.c~ *.h~ system Makefile~ Kernel.lds~ kernel.binmake