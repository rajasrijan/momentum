#!/bin/sh
# This file is part of momentum project Copyright 2009-2017 Srijan Kumar Sharma
# Check LICENSE file
echo 'Deleting old image..'
rm momentum.raw
echo 'Creating image disk..'
#dd if=/dev/zero of=momentum.raw bs=512 count=4194304  status=progress
truncate -s 2G momentum.raw
echo 'Creating partition table..'
echo "start=2048,type=c,bootable" | sfdisk -q momentum.raw
echo 'Creating file system..'
sudo losetup /dev/loop1 momentum.raw
sudo partprobe /dev/loop1
echo 'Formatting to fat32..'
sudo mkfs.vfat -F32 /dev/loop1p1
mkdir tmpdir
echo 'mounting to ./tmpdir'
sudo mount /dev/loop1p1 ./tmpdir -o umask=000
echo 'Installing GRUB'
sudo grub-install --target=i386-pc --boot-directory=./tmpdir/boot --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop1
echo 'unmounting ./tmpdir'
sudo umount /dev/loop1p1
rm -rf tmpdir
sudo losetup -d /dev/loop1
echo -e "set timeout=3\nset default=0\nmenuentry \"momentum OS\" {\n\tmultiboot /kernel.elf\n\tboot\n}" | MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M - ::/boot/grub/grub.cfg