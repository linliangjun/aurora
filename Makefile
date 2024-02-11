SRC_DIR := ./src
BUILD_DIR := ./build
BUILD_BIN_DIR := $(BUILD_DIR)/bin
INCLUDE_DIR := $(SRC_DIR)/include

HOST_SYSTEM := $(shell uname -o)-$(shell uname -m)

ifeq ($(HOST_SYSTEM),GNU/Linux-aarch64)
	TOOLCHAIN_PREFIX ?= x86_64-linux-gnu-
endif

AS := $(TOOLCHAIN_PREFIX)as
CC := $(TOOLCHAIN_PREFIX)gcc
LD := $(TOOLCHAIN_PREFIX)ld
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
AR := $(TOOLCHAIN_PREFIX)ar

$(BUILD_BIN_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(AS) --32 -g --fatal-warnings -o $@ $<

$(BUILD_BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c -m32 -ffreestanding -nostdinc -fno-pie -fno-asynchronous-unwind-tables -I$(INCLUDE_DIR) -g -MD -Wall -Werror -o $@ $<

$(BUILD_BIN_DIR)/%.bin: $(BUILD_BIN_DIR)/%
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/aurora.img: $(BUILD_BIN_DIR)/boot/boot.bin
	dd if=/dev/zero of=$@ count=2880
	dd if=$< of=$@ conv=notrunc
	sleep 1
	touch $@

include $(shell find $(SRC_DIR) -name "*.mk")

ifneq ($(wildcard $(BUILD_BIN_DIR)),)
    include $(shell find $(BUILD_BIN_DIR) -name "*.d")
endif

.PHONY: clean qemu qemu-gdb
clean:
	rm -rf $(BUILD_DIR)

qemu: $(BUILD_DIR)/aurora.img
	qemu-system-i386 -m 32M -drive file=$<,format=raw,if=floppy -boot a

qemu-gdb: $(BUILD_DIR)/aurora.img
	qemu-system-i386 -m 32M -drive file=$<,format=raw,if=floppy -boot a -s -S
