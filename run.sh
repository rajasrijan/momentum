echo "Launching qemu"
qemu-system-x86_64 -cpu max -smp 8 -machine q35 -rtc base=localtime \
-bios /usr/share/qemu/OVMF.fd \
-drive id=disk1,file=build/momentum.raw,format=raw,if=none -device ide-hd,drive=disk1,bus=ide.0 \
-net none \
-m 2048 -s -monitor stdio \
-no-reboot -no-shutdown \
-d int,mmu -D qemu_log.txt