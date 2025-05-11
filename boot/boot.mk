BOOT_OBJS := $(patsubst boot/%.S,$(BUILD_DIR)/boot/%.o, $(shell find boot -name "*.S"))

$(BUILD_DIR)/boot/boot: boot/boot.lds $(BOOT_OBJS)
	$(LD) -T $< -o $@ $(BOOT_OBJS)

$(BUILD_DIR)/boot/boot.lock: $(BUILD_DIR)/boot/boot.bin
	dd if=$< of=$(SYSTEM_IMG) conv=notrunc
	@touch $@

build-boot: $(BUILD_DIR)/boot/boot.lock
