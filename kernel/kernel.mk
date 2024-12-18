KERNEL_OBJS := $(patsubst kernel/%.S,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.S"))
KERNEL_OBJS += $(patsubst kernel/%.c,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.c"))

$(BUILD_DIR)/kernel/kernel: kernel/kernel.ld $(KERNEL_OBJS)
	$(LD) -T $< -o $@ $(KERNEL_OBJS)

$(BUILD_DIR)/kernel/kernel.lock: $(BUILD_DIR)/kernel/kernel
	@$(MAKE) -s $(BUILD_DIR)/aurora.img
	dd if=$< of=$(BUILD_DIR)/aurora.img seek=32 conv=notrunc status=none
	touch $@

build-kernel: $(BUILD_DIR)/kernel/kernel.lock
