$(BUILD_DIR)/boot/bootsect: boot/bootsect.ld $(BUILD_DIR)/boot/bootsect.o
	$(LD) -T $< -o $@ $(BUILD_DIR)/boot/bootsect.o

$(BUILD_DIR)/boot/bootsect.lock: $(BUILD_DIR)/boot/bootsect.bin
	@$(MAKE) -s $(BUILD_DIR)/aurora.img
	dd if=$< of=$(BUILD_DIR)/aurora.img conv=notrunc status=none
	@touch $@

build-boot: $(BUILD_DIR)/boot/bootsect.lock
