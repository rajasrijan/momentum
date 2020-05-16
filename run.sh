qemu-system-x86_64 -machine q35 -rtc base=localtime \
-drive id=disk1,file=momentum.raw,format=raw,if=none -device ide-hd,drive=disk1,bus=ide.0 \
-drive id=disk2,file=s1.raw,format=raw,if=none -device ide-hd,drive=disk2,bus=ide.1 \
-drive id=disk3,file=s2.raw,format=raw,if=none -device ide-hd,drive=disk3,bus=ide.2 \
-m 2048 -s