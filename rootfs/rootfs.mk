$(BUILD_DIR)/rootfs/banner.txt: rootfs/banner.txt
	@mkdir -p $(@D)
	cp $< $@

$(BUILD_DIR)/rootfs/user_task: rootfs/user_task.lds $(BUILD_DIR)/rootfs/user_task.o $(BUILD_DIR)/libc/libc.a
	$(LD) -T $< -o $@ $(BUILD_DIR)/rootfs/user_task.o $(BUILD_DIR)/libc/libc.a

$(BUILD_DIR)/ramfs.tar: $(BUILD_DIR)/rootfs/user_task $(BUILD_DIR)/rootfs/banner.txt
	@mkdir -p $(@D)
	tar -cvf $@ -C $(BUILD_DIR)/rootfs --exclude *.o --exclude *.d .
