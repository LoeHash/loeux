# 引导加载程序（Bootloader）编译与刷入
.PHONY: boot
boot:
	@nasm boot/boot.asm -o boot/boot.bin
	@dd if=boot/boot.bin of=/work/tools/loeux/boot.img bs=512 count=1 conv=notrunc 2>/dev/null || \
		{ echo "\033[1;31m[ERROR]\033[0m dd failed!"; false; }
	@echo ""
	@echo "\033[1;36m┌─────────────────────────────────────────────────────┐\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;33m LOEUX Bootloader successfully deployed!           \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m  Target: \033[0m /work/tools/loeux/boot.img              \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m   Sector: \033[0m Cylinder 0, Head 0, Sector 1           \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m    Bytes: \033[0m 512 bytes (Master Boot Record)         \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;35m         LOEUX - The OS from scratch               \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m└─────────────────────────────────────────────────────┘\033[0m"
	@echo ""

# 清理
clean:
	@find . -type d \( -name out -o -name obj \) -exec rm -rf {} + 2>/dev/null || true
