qemu-system-x86_64 -drive id=disk,file=momentum.raw,format=raw,if=none \
-device ide-drive,drive=disk,bus=ide.0 \
-device ich9-usb-uhci1 -device usb-kbd \
-m 1024 -s