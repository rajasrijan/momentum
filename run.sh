qemu-system-x86_64 -rtc base=localtime -drive id=disk1,file=momentum.raw,format=raw,if=none -device ide-drive,drive=disk1,bus=ide.0 -m 1024 -s