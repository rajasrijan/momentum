qemu-system-x86_64 -drive id=disk,file=momentum.raw,format=raw,if=none \
-device ide-drive,drive=disk,bus=ide.0 \
-m 1024 -s