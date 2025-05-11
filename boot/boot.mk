BOOT_OBJS := $(patsubst boot/%.S,$(BUILD_DIR)/boot/%.o, $(shell find boot -name "*.S"))
BOOT_OBJS += $(patsubst boot/%.c,$(BUILD_DIR)/boot/%.o, $(shell find boot -name "*.c"))

$(BUILD_DIR)/boot/boot: boot/boot.lds $(BOOT_OBJS) $(BUILD_DIR)/lib/libaurora.a
	$(LD) -T $< -o $@ $(BOOT_OBJS) $(BUILD_DIR)/lib/libaurora.a

$(BUILD_DIR)/boot/boot.lock: $(BUILD_DIR)/boot/boot.bin
	dd if=$< of=$(SYSTEM_IMG) conv=notrunc
	@touch $@

build-boot: $(BUILD_DIR)/boot/boot.lock
