BOOT_OBJS := $(patsubst $(SRC_DIR)/boot/%.S,$(BUILD_DIR)/boot/%.o, $(shell find $(SRC_DIR)/boot -name "*.S"))

$(BUILD_DIR)/boot/%.o: $(SRC_DIR)/boot/%.S
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/boot/boot: $(SRC_DIR)/boot/boot.lds $(BOOT_OBJS)
	$(LD) -T $< $(BOOT_OBJS) -o $@
	$(OBJCOPY) --only-keep-debug $@ $@.debug
	$(OBJCOPY) --strip-debug $@

$(BUILD_DIR)/boot/boot.bin: $(BUILD_DIR)/boot/boot
	$(OBJCOPY) -O binary $< $@
