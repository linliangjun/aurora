# Aurora

![license](https://img.shields.io/github/license/linliangjun/aurora)
![last commit](https://img.shields.io/github/last-commit/linliangjun/aurora)

Aurora 是一个基于 IA32 指令集架构的操作系统。

## 开发环境

- 工具链
    - GCC（x86_64-elf）
    - GNU bash
    - GNU bintuils（x86_64-elf）
    - GNU coreutils
    - GNU gdb
    - GNU Make
    - GNU mtools
- 模拟器：QEMU、Bochs

对于非 GNU/Linux-x86_64 的宿主操作系统，需要在项目根目录下的 env.mk 文件中正确配置交叉编译环境。

以 MacOS 为例，env.mk 如下：

```makefile
BOCHSFLAGS += 'display_library: sdl2'
```

## 构建

```shell
$ make all
```

## 运行

- 通过 QEMU 运行

    ```shell
    $ make qemu
    ```

- 通过 Bochs 运行

    ```shell
    $ touch .bochsrc    # 创建一个空的 bochs 配置文件
    $ make bochs
    ```
