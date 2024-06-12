SRC_DIR := ./src
BUILD_DIR := ./build

AS := as
LD := ld
OBJCOPY := objcopy
QEMU := qemu-system-i386

ASFLAGS := --32 -march=i386 --warn --fatal-warnings
QEMUFLAGS := -m 32M -boot a

$(BUILD_DIR)/aurora.img: $(BUILD_DIR)/boot/boot.bin
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ count=2880
	dd if=$< of=$@ conv=notrunc

include $(shell find src -name "*.mk")

.PHONY: run clean
run: $(BUILD_DIR)/aurora.img
	$(QEMU) $(QEMUFLAGS) -drive if=floppy,format=raw,file=$<

clean:
	rm -rf $(BUILD_DIR)
