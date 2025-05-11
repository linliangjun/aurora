$(BUILD_DIR)/boot/bootsect: boot/bootsect.lds $(BUILD_DIR)/boot/bootsect.o
	$(LD) -T $< -o $@ $(BUILD_DIR)/boot/bootsect.o

$(BUILD_DIR)/boot/bootsect.lock: $(BUILD_DIR)/boot/bootsect.bin
	dd if=$< of=$(SYSTEM_IMG) conv=notrunc
	@touch $@

build-boot: $(BUILD_DIR)/boot/bootsect.lock
