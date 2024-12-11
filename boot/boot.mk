STAGE2_OBJS := $(BUILD_DIR)/boot/stage2.o \
	$(BUILD_DIR)/boot/mode_switch.o \
	$(BUILD_DIR)/boot/main.o \
	$(BUILD_DIR)/boot/bioscall.o

$(BUILD_DIR)/boot/%: boot/%.ld $(BUILD_DIR)/boot/%.o
	$(LD) -o $@ -T $^

$(BUILD_DIR)/boot/stage2: boot/stage2.ld $(STAGE2_OBJS)
	$(LD) -T $< -o $@ $(STAGE2_OBJS)

$(BUILD_DIR)/boot/bootsect.lock: $(BUILD_DIR)/boot/bootsect.bin
	@$(MAKE) -s $(BUILD_DIR)/aurora.img
	dd if=$< of=$(BUILD_DIR)/aurora.img conv=notrunc status=none
	@touch $@

$(BUILD_DIR)/boot/stage2.lock: $(BUILD_DIR)/boot/stage2.bin
	@$(MAKE) -s $(BUILD_DIR)/aurora.img
	dd if=$< of=$(BUILD_DIR)/aurora.img seek=1 conv=notrunc status=none
	touch $@

build-boot: $(BUILD_DIR)/boot/bootsect.lock $(BUILD_DIR)/boot/stage2.lock
