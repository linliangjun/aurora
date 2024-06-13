set history filename .gdb_history
set history size 100
set history save on
file build/boot/boot.debug
target remote localhost:1234
set architecture i8086
set tdesc filename target.xml
b start
c
