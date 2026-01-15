LIBC_OBJS := $(patsubst libc/%.c,$(BUILD_DIR)/libc/%.o, $(shell find libc -name "*.c"))

$(BUILD_DIR)/libc/libc.a: $(LIBC_OBJS)
	$(AR) rcs $@ $^
