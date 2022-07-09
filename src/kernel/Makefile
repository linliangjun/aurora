$(BUILD)/kernel/%.o: $(SRC)/kernel/%.S
	@mkdir -p $(@D)
	$(AS) $(AS_FLAGS) $(DEBUG_FLAGS) -o $@ $<

$(BUILD)/kernel/kernel: $(BUILD)/kernel/start.o
	$(LD) $(LD_FLAGS) -Ttext 0x10000 $^ -o $@

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/kernel
	$(OBJCOPY) -O binary --remove-section .note.gnu.property $< $@
