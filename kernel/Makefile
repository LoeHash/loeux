C_SOURCES = $(wildcard *.c)
# 排除 head.S，不让 OBJECTS 包含 head.o
ASM_SOURCES = $(filter-out head.S, $(wildcard *.S))
OBJECTS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

system: head.o $(OBJECTS)
	ld -b elf64-x86-64 -o system $^ -T Kernel.lds

head.o:	head.S
	gcc -E  head.S > head.s
	as --64 -o head.o head.s

entry.o: entry.S
	gcc -E  entry.S > entry.s
	as --64 -o entry.o entry.s
	
main.o:	main.c
	gcc  -mcmodel=large -fno-builtin -m64 -c main.c
	
printk.o: printk.c
	gcc  -mcmodel=large -fno-builtin -m64 -c printk.c

trap.o: trap.c
	gcc  -mcmodel=large -fno-builtin -m64 -c trap.c


clean:
	rm -rf *.o *.s system kernel.bin