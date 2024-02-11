LIB_OBJS := $(patsubst $(SRC_DIR)/lib/%.c,$(BUILD_BIN_DIR)/lib/%.o, $(shell find $(SRC_DIR)/lib -name "*.c"))

$(BUILD_DIR)/libc.a: $(LIB_OBJS)
	$(AR) -rcs $@ $(LIB_OBJS)
