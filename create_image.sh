#!/bin/sh
# This file is part of momentum project Copyright 2009-2021 Srijan Kumar Sharma
# Check LICENSE file
echo 'Deleting old image..'
rm $1
echo 'Creating image disk..'
#dd if=/dev/zero of=$1 bs=512 count=4194304  status=progress
truncate -s 256M $1
echo 'Creating partition table..'
echo "start=2048,size=202753,type=ef,bootable\nstart=206848,type=83" | sfdisk -q $1
echo 'Creating file system..'
LODRIVE=$(losetup -f)
LOPART_EFI="$LODRIVE""p1"
LOPART_OS="$LODRIVE""p2"
echo "Loop Drive: $LODRIVE"
echo "EFI Partition: $LOPART_EFI"
echo "OS Partition: $LOPART_OS"
sudo losetup $LODRIVE momentum.raw
sudo partprobe $LODRIVE
echo 'Formatting to EFI..'
sudo mkfs.vfat -F 32 $LOPART_EFI
lsblk -f -n -o uuid "$LOPART_EFI"
echo 'Formatting to ext2..'
sudo mkfs.ext2 $LOPART_OS
lsblk -f -n -o uuid "$LOPART_OS"
mkdir -p EFI
mkdir -p OS
echo 'mounting to ./EFI'
sudo mount -o rw $LOPART_EFI ./EFI
echo 'mounting to ./OS'
sudo mount -o rw $LOPART_OS ./OS
echo 'Installing GRUB'
sudo grub-install --target=x86_64-efi --boot-directory=./OS/boot --efi-directory=./EFI/ --no-floppy --modules="normal part_msdos ext2 multiboot multiboot2" $LODRIVE
sudo chown -R $USER ./OS
mkdir ./OS/dev
mkdir ./OS/mnt
PART_UUID=$(lsblk -f -n -o uuid "$LOPART_OS")
echo "ext2 uuid:$PART_UUID"
echo -e "set timeout=2\nset default=0\nmenuentry \"momentum OS\" {\n\tmultiboot2 /momentum.elf uuid=$PART_UUID \n\tboot\n}" > ./OS/boot/grub/grub.cfg
echo 'unmounting'
sudo umount $LOPART_EFI
sudo umount $LOPART_OS
rm -rf ./OS ./EFI
sudo losetup -d $LODRIVE