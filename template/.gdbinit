symbol-file build/boot/boot
# symbol-file build/kernel/kernel
target remote localhost:1234
b main
c
tui enable
