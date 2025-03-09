LIB_OBJS += $(patsubst lib/%.c,$(BUILD_DIR)/lib/%.o, $(shell find lib -name "*.c"))

LIB_CFLAGS := -Iinclude

$(BUILD_DIR)/lib/%.o: lib/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

$(BUILD_DIR)/lib/libc.a: $(LIB_OBJS)
	$(AR) -rcs $@ $^
