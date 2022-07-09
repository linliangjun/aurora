# 项目目录结构
SRC := src
BUILD := build

# 编译器和链接器
AS := as
CC := gcc
LD := ld

# 构建参数
AS_FLAGS := --32
C_FLAGS := -m32 -fno-stack-protector -no-pie -fno-pic -mno-sse -Wall
LD_FLAGS := -m elf_i386
DEBUG_FLAGS := -g

# 工具
DD := dd
OBJCOPY := objcopy
QEMU_IMG := qemu-img

# 模拟器
QEMU := qemu-system-i386

# 系统参数
IMG_SIZE := 16M
MEM_SIZE := 16M

# 创建磁盘镜像文件
$(BUILD)/aurora.img: $(BUILD)/boot/boot.bin $(BUILD)/kernel/kernel.bin
	@mkdir -p $(@D)
	$(QEMU_IMG) create -f raw $@ $(IMG_SIZE)
	$(DD) if=$< of=$@ conv=notrunc
	$(DD) if=$(BUILD)/kernel/kernel.bin of=$@ seek=5 conv=notrunc

# 虚假的目标
.PHONY: qemu clean

# 运行 QEMU
qemu: $(BUILD)/aurora.img
	$(QEMU) -m $(MEM_SIZE) -drive file=$<,format=raw,if=ide

# 以调试模式运行 QEMU
qemu-debug: $(BUILD)/aurora.img
	$(QEMU) -m $(MEM_SIZE) -drive file=$<,format=raw,if=ide -s -S

# 清除所有构建内容
clean:
	rm -rf $(BUILD)

# 引入外部 Makefile 文件
include $(shell find src -name Makefile)
