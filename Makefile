BUILD_DIR := build
INCLUDE_DIR := include

CC = $(CC_PREFIX)gcc
LD = $(BINUTILS_PREFIX)ld
OBJCOPY = $(BINUTILS_PREFIX)objcopy
QEMU_SYSTEM_I386 = $(QEMU_SYSTEM_I386_PREFIX)qemu-system-i386
BOCHS = $(BOCHS_PREFIX)bochs

ASMFLAGS := -m32 -ffreestanding -Wall -Werror -MD -g -I$(INCLUDE_DIR)
CFLAGS := -m32 -ffreestanding -Wall -Werror -MD -g -I$(INCLUDE_DIR)\
        -march=pentium -fno-pie -fno-asynchronous-unwind-tables \
		-fno-stack-protector -DDEBUG

QEMUFLAGS := -cpu pentium -m 32M \
	-drive if=floppy,format=raw,file=$(BUILD_DIR)/aurora.img \
	-boot a -monitor stdio
BOCHSFLAGS := 'cpu: model=pentium' 'megs: 32' \
	'floppya: 1_44=aurora.img, status=inserted' \
	'boot: floppy' \
	'magic_break: enabled=1'

include $(shell find ./ -regex ".*\.mk\|.*\.d")

.DEFAULT_GOAL := all
.PHONY: all clean qemu bochs

$(BUILD_DIR)/aurora.img:
	mkdir -p $(@D)
	dd if=/dev/zero of=$@ count=2880

$(BUILD_DIR)/.bochsrc:
	mkdir -p $(@D)
	touch $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(@D)
	$(CC) -c $(ASMFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%
	$(OBJCOPY) -O binary $< $@

all: build-boot build-kernel

clean:
	rm -rf $(BUILD_DIR)

qemu: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS)

qemu-gdb: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS) -s -S

bochs: $(BUILD_DIR)/.bochsrc all
	cd $(dir $<); $(BOCHS) -q $(BOCHSFLAGS)
