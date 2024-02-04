set history save on
set history size 1000
file build/bin/boot/boot.debug
target remote localhost:1234
set tdesc filename target.xml
set architecture i386
b protect_entry
c
