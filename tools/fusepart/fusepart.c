#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

int fd = 0;
struct partition_files
{
    char name[256];
    size_t offset;
    size_t size;
} partition_file_list[4];
int partition_file_count = 0;

int atime = 0;
int mtime = 0;

static int
do_getattr(const char *path, struct stat *st)
{
    st->st_uid = getuid();
    st->st_gid = getgid();

    st->st_atime = atime;
    st->st_mtime = mtime;

    if (strcmp(path, "/") == 0)
    {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    }
    else
    {
        if (path[0] == '/')
        {
            for (size_t i = 0; i < partition_file_count; i++)
                if (!strcmp(partition_file_list[i].name, &path[1]))
                {
                    st->st_mode = S_IFREG | 0666;
                    st->st_nlink = 1;
                    st->st_size = partition_file_list[i].size;
                    return 0;
                }
        }
    }
    return 0;
}

static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if (strcmp(path, "/") == 0)
    {
        for (size_t i = 0; i < partition_file_count; i++)
            filler(buffer, partition_file_list[i].name, NULL, 0);
    }

    return 0;
}

static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    if (path[0] == '/')
    {
        for (size_t i = 0; i < partition_file_count; i++)
            if (!strcmp(partition_file_list[i].name, &path[1]))
            {
                lseek(fd, partition_file_list[i].offset + offset, SEEK_SET);
                atime = time(NULL);
                return read(fd, buffer, size);
            }
    }
    return 0;
}

static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    if (path[0] == '/')
    {
        for (size_t i = 0; i < partition_file_count; i++)
            if (!strcmp(partition_file_list[i].name, &path[1]))
            {
                lseek(fd, partition_file_list[i].offset + offset, SEEK_SET);
                mtime = time(NULL);
                return write(fd, buffer, size);
            }
    }
    return 0;
}

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .write = do_write,
};

struct Partition
{
    uint8_t status;
    uint8_t legacy1[3];
    uint8_t partitiotype;
    uint8_t legacy2[3];
    uint32_t startLBA;
    uint32_t count;
} __attribute__((packed));

struct PartitionTable
{
    char boot_code[446];
    struct Partition partition[4];
    uint16_t boot_signature; // 0x55AA
} __attribute__((packed));

int main(int argc, char *argv[])
{
    int ret = 0;
    atime = time(NULL);
    mtime = time(NULL);
    char *disk_img_path = NULL;
    if (argc < 2)
    {
        fprintf(stderr, "pass disk image and mount point as first two arguments\n");
        return -EINVAL;
    }

    disk_img_path = argv[1];
    fd = open(disk_img_path, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to open disk image [%s].\n", disk_img_path);
        return -EINVAL;
    }
    struct stat st = {};
    fstat(fd, &st);
    size_t disk_size = st.st_size;
    //  remove the disk image from the argument
    for (size_t i = 2; i < argc; i++)
    {
        argv[i - 1] = argv[i];
    }
    argc--;

    struct PartitionTable part_table = {};
    ret = read(fd, (void *)&part_table, sizeof(struct PartitionTable));
    printf("boot signature [%#X]\n", part_table.boot_signature);

    for (size_t i = 0; i < 4; i++)
    {
        if (part_table.partition[i].count > 0 && part_table.partition[i].startLBA > 1 && part_table.partition[i].startLBA + part_table.partition[i].count <= disk_size)
        {
            snprintf(partition_file_list[partition_file_count].name, 256, "disk_part%lu", i);
            printf("Found partition %s\n", partition_file_list[partition_file_count].name);
            partition_file_list[partition_file_count].offset = part_table.partition[i].startLBA * 512;
            partition_file_list[partition_file_count].size = part_table.partition[i].count * 512;
            partition_file_count++;
        }
    }

    ret = fuse_main(argc, argv, &operations, NULL);

    close(fd);

    return ret;
}