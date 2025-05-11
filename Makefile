BUILD_DIR := build

SYSTEM_IMG := $(BUILD_DIR)/aurora.img

QEMU_SYSTEM_I386 = $(QEMU_SYSTEM_I386_PREFIX)qemu-system-i386
BOCHS = $(BOCHS_PREFIX)bochs
CC = $(CC_PREFIX)gcc
LD = $(BINUTILS_PREFIX)ld
AR = $(BINUTILS_PREFIX)ar
OBJCOPY = $(BINUTILS_PREFIX)objcopy

QEMUFLAGS := -cpu pentium -m 32M \
	-drive if=floppy,format=raw,file=$(SYSTEM_IMG) \
	-boot a -monitor stdio
BOCHSFLAGS := 'cpu: model=pentium' 'megs: 32' \
	'floppya: 1_44=$(SYSTEM_IMG), status=inserted' \
	'boot: floppy' \
	'magic_break: enabled=1'
ASMFLAGS := -m32 -ffreestanding -Wall -Werror -MD -Iinclude -g -D__ASSEMBLY__
CFLAGS := -m32 -ffreestanding -Wall -Werror -MD -Iinclude -g \
	-march=pentium -fno-pie -fno-asynchronous-unwind-tables -fno-stack-protector -std=c99

include $(shell find ./ \( -name "*.mk" -o -name "*.d" \))

.DEFAULT_GOAL := all
.PHONY: all clean qemu bochs

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(@D)
	$(CC) -c $(ASMFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%
	$(OBJCOPY) -O binary $< $@

$(SYSTEM_IMG):
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ count=2880

all: $(SYSTEM_IMG) build-boot build-kernel

clean:
	rm -rf $(BUILD_DIR)

qemu: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS)

qemu-gdb: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS) -s -S

bochs: all
	$(BOCHS) -q $(BOCHSFLAGS)
