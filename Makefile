BUILD_DIR := build

SYSTEM_IMG := $(BUILD_DIR)/aurora.img

QEMU_SYSTEM_I386 = $(QEMU_SYSTEM_I386_PREFIX)qemu-system-i386
BOCHS = $(BOCHS_PREFIX)bochs

QEMUFLAGS := -cpu pentium -m 32M \
	-drive if=floppy,format=raw,file=$(SYSTEM_IMG) \
	-boot a -monitor stdio
BOCHSFLAGS := 'cpu: model=pentium' 'megs: 32' \
	'floppya: 1_44=$(SYSTEM_IMG), status=inserted' \
	'boot: floppy' \
	'magic_break: enabled=1'

include $(shell find ./ -name "*.mk")

.DEFAULT_GOAL := all
.PHONY: all clean qemu bochs

$(SYSTEM_IMG):
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ count=2880

all: $(SYSTEM_IMG)

clean:
	rm -rf $(BUILD_DIR)

qemu: all
	$(QEMU_SYSTEM_I386) $(QEMUFLAGS)

bochs: all
	$(BOCHS) -q $(BOCHSFLAGS)
