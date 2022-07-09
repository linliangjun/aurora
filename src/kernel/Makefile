$(BUILD)/kernel/%.o: $(SRC)/kernel/%.S
	@mkdir -p $(@D)
	$(AS) $(AS_FLAGS) $(DEBUG_FLAGS) -o $@ $<

$(BUILD)/kernel/%.o: $(SRC)/kernel/%.c
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) $(DEBUG_FLAGS) -c -o $@ $<

$(BUILD)/kernel/kernel: $(BUILD)/kernel/start.o $(BUILD)/kernel/main.o $(BUILD)/kernel/io.o
	$(LD) $(LD_FLAGS) -Ttext 0x10000 $^ -o $@

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/kernel
	$(OBJCOPY) -O binary --remove-section .note.gnu.property $< $@
