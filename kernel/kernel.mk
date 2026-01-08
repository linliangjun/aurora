KERNEL_OBJS := $(patsubst kernel/%.S,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.S"))
KERNEL_OBJS += $(patsubst kernel/%.c,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.c"))

$(BUILD_DIR)/ramfs.tar: $(shell find rootfs -type f)
	@mkdir -p $(@D)
	tar -cvf $@ -C rootfs .

$(BUILD_DIR)/ramfs.o: $(BUILD_DIR)/ramfs.tar
	@mkdir -p $(@D)
	$(OBJCOPY) -I binary -O elf32-i386 -B i386 $< $@

$(BUILD_DIR)/kernel/kernel: kernel/kernel.lds $(KERNEL_OBJS) $(BUILD_DIR)/lib/libaurora.a $(BUILD_DIR)/ramfs.o
	$(LD) -T $< -o $@ $(KERNEL_OBJS) $(BUILD_DIR)/lib/libaurora.a $(BUILD_DIR)/ramfs.o
	$(OBJCOPY) --only-keep-debug $@ $@.dbg
	$(OBJCOPY) --strip-debug $@

$(BUILD_DIR)/kernel/kernel.lock: $(BUILD_DIR)/kernel/kernel
	dd if=$< of=$(SYSTEM_IMG) seek=32 conv=notrunc
	@touch $@

build-kernel: $(BUILD_DIR)/kernel/kernel.lock
