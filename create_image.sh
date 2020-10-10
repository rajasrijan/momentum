#!/bin/sh
# This file is part of momentum project Copyright 2009-2020 Srijan Kumar Sharma
# Check LICENSE file
echo 'Deleting old image..'
rm $1
echo 'Creating image disk..'
#dd if=/dev/zero of=$1 bs=512 count=4194304  status=progress
truncate -s 256M $1
echo 'Creating partition table..'
echo "start=2048,type=c,bootable" | sfdisk -q $1
echo 'Creating file system..'
LODRIVE=$(losetup -f)
LOPART="$LODRIVE""p1"
echo "$LODRIVE"
echo "$LOPART"
sudo losetup $LODRIVE momentum.raw
sudo partprobe $LODRIVE
echo 'Formatting to fat32..'
sudo mkfs.vfat -F32 $LOPART
PART_UUID=$(lsblk -f -n -o uuid "$LOPART")
echo "fat32 uuid:$PART_UUID"
mkdir tmpdir
echo 'mounting to ./tmpdir'
sudo mount $LOPART ./tmpdir -o umask=000
mkdir ./tmpdir/dev
mkdir ./tmpdir/mnt
echo 'Installing GRUB'
sudo grub-install --target=i386-pc --boot-directory=./tmpdir/boot --no-floppy --modules="normal part_msdos ext2 multiboot multiboot2" $LODRIVE
echo 'unmounting ./tmpdir'
sudo umount $LOPART
rm -rf tmpdir
sudo losetup -d $LODRIVE
echo -e "set timeout=2\nset default=0\nmenuentry \"momentum OS\" {\n\tmultiboot2 /kernel.elf uuid=$PART_UUID \n\tboot\n}" | MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M - ::/boot/grub/grub.cfg