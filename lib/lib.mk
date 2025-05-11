LIB_OBJS := $(patsubst lib/%.c,$(BUILD_DIR)/lib/%.o, $(shell find lib -name "*.c"))

$(BUILD_DIR)/lib/libaurora.a: $(LIB_OBJS)
	$(AR) rcs $@ $^
