KERNEL_OBJS := $(patsubst kernel/%.S,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.S"))
KERNEL_OBJS += $(patsubst kernel/%.c,$(BUILD_DIR)/kernel/%.o, $(shell find kernel -name "*.c"))

KERNEL_ASMFLAGS := -Iinclude -DDEBUG
KERNEL_CFLAGS := -Iinclude -DDEBUG

$(BUILD_DIR)/kernel/%.o: kernel/%.S
	@mkdir -p $(@D)
	$(CC) -c $(ASMFLAGS) $(KERNEL_ASMFLAGS) -o $@ $<

$(BUILD_DIR)/kernel/%.o: kernel/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) -o $@ $<

$(BUILD_DIR)/kernel/kernel: kernel/kernel.ld $(KERNEL_OBJS) $(BUILD_DIR)/lib/libc.a
	$(LD) -T $< -o $@ $(KERNEL_OBJS) -L $(BUILD_DIR)/lib -lc

$(BUILD_DIR)/kernel/kernel.lock: $(BUILD_DIR)/kernel/kernel
	@$(MAKE) -s $(SYSTEM_IMG)
	dd if=$< of=$(SYSTEM_IMG) seek=32 conv=notrunc
	@touch $@

build-kernel: $(BUILD_DIR)/kernel/kernel.lock
