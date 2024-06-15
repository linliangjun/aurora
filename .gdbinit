set history filename .gdb_history
set history size 100
set history save on
file build/boot/boot.debug
target remote localhost:1234
set tdesc filename target.xml

set $lastcs = -1

define hook-stop
  if $cs == 8 || $cs == 27
    if $lastcs != 8 && $lastcs != 27
      set architecture i386
    end
  else
    if $lastcs == -1 || $lastcs == 8 || $lastcs == 27
      set architecture i8086
    end
  end
  set $lastcs = $cs
end

b start
c
