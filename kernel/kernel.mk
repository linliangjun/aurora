KERNEL_OBJS := $(patsubst kernel/%.S,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.S"))

$(BUILD_DIR)/kernel/kernel: kernel/kernel.ld $(KERNEL_OBJS)
	$(LD) -T $< -o $@ $(KERNEL_OBJS)

$(BUILD_DIR)/kernel/kernel.lock: $(BUILD_DIR)/kernel/kernel
	@$(MAKE) -s $(SYSTEM_IMG)
	dd if=$< of=$(SYSTEM_IMG) seek=32 conv=notrunc
	@touch $@

build-kernel: $(BUILD_DIR)/kernel/kernel.lock
