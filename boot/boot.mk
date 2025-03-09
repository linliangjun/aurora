BOOT_ASMFLAGS := -Iboot/include
BOOT_CFLAGS := -Iboot/include

BOOT_STAGE := 1 2
BOOT_STAGE_LBA := 0 1

$(BUILD_DIR)/boot/%.o: boot/%.S
	@mkdir -p $(@D)
	$(CC) -c $(ASMFLAGS) $(BOOT_ASMFLAGS) -o $@ $<

$(BUILD_DIR)/boot/%.o: boot/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(BOOT_CFLAGS) -o $@ $<

define boot_stage
BOOT_LOCK += $(BUILD_DIR)/boot/stage$(1)/stage$(1).lock

BOOT_STAGE$(1)_OBJS := \
	$(patsubst boot/stage$(1)/%.S,$(BUILD_DIR)/boot/stage$(1)/%.o, $(shell find boot/stage$(1) -name "*.S"))
BOOT_STAGE$(1)_OBJS += \
	$(patsubst boot/stage$(1)/%.c,$(BUILD_DIR)/boot/stage$(1)/%.o, $(shell find boot/stage$(1) -name "*.c"))

$(BUILD_DIR)/boot/stage$(1)/stage$(1): boot/stage$(1)/stage$(1).ld $$(BOOT_STAGE$(1)_OBJS)
	$$(LD) -T $$< -o $$@ $$(BOOT_STAGE$(1)_OBJS)

$(BUILD_DIR)/boot/stage$(1)/stage$(1).lock: $(BUILD_DIR)/boot/stage$(1)/stage$(1).bin
	@$(MAKE) -s $(SYSTEM_IMG)
	dd if=$$< of=$(SYSTEM_IMG) seek=$(word $(1), $(BOOT_STAGE_LBA)) conv=notrunc
	@touch $$@
endef

$(foreach stage, $(BOOT_STAGE), $(eval $(call boot_stage,$(stage))))

build-boot: $(BOOT_LOCK)
