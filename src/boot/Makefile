$(BUILD)/boot/%.o: $(SRC)/boot/%.S
	@mkdir -p $(@D)
	$(AS) $(AS_FLAGS) -o $@ $<

$(BUILD)/boot/boot: $(BUILD)/boot/mbr.o
	$(LD) $(LD_FLAGS) -Ttext 0x7c00 $^ -o $@

$(BUILD)/boot/boot.bin: $(BUILD)/boot/boot
	$(OBJCOPY) -O binary --remove-section .note.gnu.property $< $@
