BUILD_DIR := build

QEMU_SYSTEM_I386 = $(QEMU_SYSTEM_I386_PREFIX)qemu-system-i386
BOCHS = $(BOCHS_PREFIX)bochs

QEMUFLAGS := -cpu pentium -m 32M \
	-drive if=floppy,format=raw,file=$(BUILD_DIR)/aurora.img \
	-boot a -monitor stdio
BOCHSFLAGS := 'cpu: model=pentium' 'megs: 32' \
	'floppya: 1_44=aurora.img, status=inserted' \
	'boot: floppy' \
	'magic_break: enabled=1'

include $(shell find ./ -name "*.mk")

.DEFAULT_GOAL := all
.PHONY: all clean qemu bochs

$(BUILD_DIR)/aurora.img:
	mkdir -p $(@D)
	dd if=/dev/zero of=$@ count=2880

$(BUILD_DIR)/.bochsrc:
	mkdir -p $(@D)
	touch $@

all: $(BUILD_DIR)/aurora.img

clean:
	rm -rf $(BUILD_DIR)

qemu: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS)

bochs: $(BUILD_DIR)/.bochsrc all
	cd $(dir $<); $(BOCHS) -q $(BOCHSFLAGS)
