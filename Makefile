CLEAN_PATTERNS := *.o *.bin system an



.PHONY: all
all: boot loader kernel               
	@echo "\033[1;32m✓ All targets built successfully.\033[0m"

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

MOUNT_POINT := /media/tmp

.PHONY: loader
loader:
	@nasm boot/loader.asm -o boot/loader.bin
	@if mountpoint -q $(MOUNT_POINT); then \
		echo "\033[1;33mMount point $(MOUNT_POINT) already mounted, skipping mount.\033[0m"; \
	else \
		sudo mount /work/tools/loeux/boot.img $(MOUNT_POINT) -t vfat -o loop || \
		{ echo "\033[1;31m[ERROR] Mount failed!\033[0m"; false; }; \
	fi
	@sudo cp boot/loader.bin $(MOUNT_POINT)/LOADER.BIN
	@sudo sync
	@if fuser -m $(MOUNT_POINT) >/dev/null 2>&1; then \
		echo "\033[1;33mMount point $(MOUNT_POINT) is in use, skipping unmount.\033[0m"; \
	else \
		sudo umount $(MOUNT_POINT); \
	fi
	@echo ""
	@echo "\033[1;36m┌─────────────────────────────────────────────────────┐\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;33m    LOEUX Loader successfully compiled!            \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m  Output: \033[0m boot/loader.bin                         \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;35m         LOEUX - The OS from scratch               \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m└─────────────────────────────────────────────────────┘\033[0m"
	@echo ""

.PHONY: kernel
kernel:
	@$(MAKE) -C kernel all
	@if mountpoint -q $(MOUNT_POINT); then \
		echo "\033[1;33mMount point $(MOUNT_POINT) already mounted, skipping mount.\033[0m"; \
	else \
		sudo mount /work/tools/loeux/boot.img $(MOUNT_POINT) -t vfat -o loop || \
		{ echo "\033[1;31m[ERROR] Mount failed!\033[0m"; false; }; \
	fi
	@sudo cp kernel/kernel.bin $(MOUNT_POINT)/KERNEL.BIN
	@sudo sync
	@if fuser -m $(MOUNT_POINT) >/dev/null 2>&1; then \
		echo "\033[1;33mMount point $(MOUNT_POINT) is in use, skipping unmount.\033[0m"; \
	else \
		sudo umount $(MOUNT_POINT); \
	fi
	@echo ""
	@echo "\033[1;36m┌─────────────────────────────────────────────────────┐\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;33m    LOEUX Kernel successfully compiled!            \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;32m  Output: \033[0m kernel/kernel.bin                       \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m                                                     \033[1;36m│\033[0m"
	@echo "\033[1;36m│\033[0m  \033[1;35m         LOEUX - The OS from scratch               \033[0m\033[1;36m│\033[0m"
	@echo "\033[1;36m└─────────────────────────────────────────────────────┘\033[0m"
	@echo ""

.PHONY: clean
clean:
	@for pattern in $(CLEAN_PATTERNS); do \
		find . -type f -name "$$pattern" -exec rm -f {} + 2>/dev/null || true; \
	done
	@find . -type d \( -name out -o -name obj -o -name bin \) -exec rm -rf {} + 2>/dev/null || true
	@echo "\033[1;32m✓ Cleanup done.\033[0m"