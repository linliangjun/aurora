BOOT_OBJS := $(patsubst $(SRC_DIR)/boot/%.S,$(BUILD_BIN_DIR)/boot/%.o, $(shell find $(SRC_DIR)/boot -name "*.S"))
BOOT_OBJS += $(patsubst $(SRC_DIR)/boot/%.c,$(BUILD_BIN_DIR)/boot/%.o, $(shell find $(SRC_DIR)/boot -name "*.c"))

$(BUILD_BIN_DIR)/boot/boot: $(SRC_DIR)/boot/boot.ld $(BOOT_OBJS) $(BUILD_DIR)/libc.a
	$(LD) -T $< $(BOOT_OBJS) $(BUILD_DIR)/libc.a -o $@
	$(OBJCOPY) --only-keep-debug $@ $@.debug
	$(OBJCOPY) --strip-debug $@
