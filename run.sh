qemu-system-x86_64 -drive id=disk1,file=momentum.raw,format=raw,if=none -drive id=disk2,file=disk.raw,format=raw,if=none \
-device ide-drive,drive=disk1,bus=ide.0 -device ide-drive,drive=disk2,bus=ide.1 \
-m 1024 -s