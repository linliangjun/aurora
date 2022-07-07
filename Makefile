# 项目目录结构
SRC := src
BUILD := build

# 编译器和链接器
AS := as
LD := ld

# 构建参数
AS_FLAGS := --32
LD_FLAGS := -m elf_i386

# 工具
OBJCOPY := objcopy

# 虚假的目标
.PHONY: clean

# 清除所有构建内容
clean:
	rm -rf $(BUILD)

# 引入外部 Makefile 文件
include $(shell find src -name Makefile)
