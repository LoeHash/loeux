# 全部构建（默认目标）
.PHONY: all
all: boot loader               # 此处可追加其他目标，如 kernel
	@echo "\033[1;32m✓ All targets built successfully.\033[0m"

# 在这里添加更多扩展规则
# TODO: e.g. kernel: ...
# TODO: e.g. image: boot loader ... && ... 

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

# 第二阶段引导加载程序（Loader）编译
.PHONY: loader
loader:
	@nasm boot/loader.asm -o boot/loader.bin
	@sudo mount /work/tools/loeux/boot.img /media/loe -t vfat -o loop
	@sudo cp boot/loader.bin /media/loe/LOADER.BIN 
	@sudo sync
	@sudo umount /media/loe
	@echo ""
	@echo "\033[1;36m┌─────────────────────────────────────────────────────┐\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;33m    LOEUX Loader successfully compiled!            \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m  Output: \033[0m boot/loader.bin                         \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;35m         LOEUX - The OS from scratch               \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m└─────────────────────────────────────────────────────┘\033[0m"
	@echo ""

CLEAN_PATTERNS := *.o *.bin

.PHONY: clean
clean:
	@for pattern in $(CLEAN_PATTERNS); do \
		find . -type f -name "$$pattern" -exec rm -f {} + 2>/dev/null || true; \
	done
	@find . -type d \( -name out -o -name obj -o -name bin \) -exec rm -rf {} + 2>/dev/null || true
	@echo "\033[1;32m✓ Cleanup done.\033[0m"