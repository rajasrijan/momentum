mkdir drive_mnt
tools/fusepart momentum.raw drive_mnt
fsck.ext2 -n -f drive_mnt/disk_part0
sync drive_mnt
umount drive_mnt
sync drive_mnt
rm -r drive_mnt