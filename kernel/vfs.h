/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VFS_H
#define VFS_H
#include <errno.h>
#include <functional>
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <uuid.h>

using namespace std;

#define BLKPBSZGET 1
#define BLKGETSIZE64 2

enum FS_TYPE_MAGIC {
    ADFS_SUPER_MAGIC = 0xadf5,
    AFFS_SUPER_MAGIC = 0xadff,
    AFS_SUPER_MAGIC = 0x5346414f,
    ANON_INODE_FS_MAGIC = 0x09041934,
    AUTOFS_SUPER_MAGIC = 0x0187,
    BDEVFS_MAGIC = 0x62646576,
    BEFS_SUPER_MAGIC = 0x42465331,
    BFS_MAGIC = 0x1badface,
    BINFMTFS_MAGIC = 0x42494e4d,
    BPF_FS_MAGIC = 0xcafe4a11,
    BTRFS_SUPER_MAGIC = 0x9123683e,
    BTRFS_TEST_MAGIC = 0x73727279,
    CGROUP_SUPER_MAGIC = 0x27e0eb,
    CGROUP2_SUPER_MAGIC = 0x63677270,
    CIFS_MAGIC_NUMBER = 0xff534d42,
    CODA_SUPER_MAGIC = 0x73757245,
    COH_SUPER_MAGIC = 0x012ff7b7,
    CRAMFS_MAGIC = 0x28cd3d45,
    DEBUGFS_MAGIC = 0x64626720,
    DEVFS_SUPER_MAGIC = 0x1373,
    DEVPTS_SUPER_MAGIC = 0x1cd1,
    ECRYPTFS_SUPER_MAGIC = 0xf15f,
    EFIVARFS_MAGIC = 0xde5e81e4,
    EFS_SUPER_MAGIC = 0x00414a53,
    EXT_SUPER_MAGIC = 0x137d,
    EXT2_OLD_SUPER_MAGIC = 0xef51,
    EXT2_SUPER_MAGIC = 0xef53,
    EXT3_SUPER_MAGIC = 0xef53,
    EXT4_SUPER_MAGIC = 0xef53,
    F2FS_SUPER_MAGIC = 0xf2f52010,
    FUSE_SUPER_MAGIC = 0x65735546,
    FUTEXFS_SUPER_MAGIC = 0xbad1dea,
    HFS_SUPER_MAGIC = 0x4244,
    HOSTFS_SUPER_MAGIC = 0x00c0ffee,
    HPFS_SUPER_MAGIC = 0xf995e849,
    HUGETLBFS_MAGIC = 0x958458f6,
    ISOFS_SUPER_MAGIC = 0x9660,
    JFFS2_SUPER_MAGIC = 0x72b6,
    JFS_SUPER_MAGIC = 0x3153464a,
    MINIX_SUPER_MAGIC = 0x137f,
    MINIX_SUPER_MAGIC2 = 0x138f,
    MINIX2_SUPER_MAGIC = 0x2468,
    MINIX2_SUPER_MAGIC2 = 0x2478,
    MINIX3_SUPER_MAGIC = 0x4d5a,
    MQUEUE_MAGIC = 0x19800202,
    MSDOS_SUPER_MAGIC = 0x4d44,
    MTD_INODE_FS_MAGIC = 0x11307854,
    NCP_SUPER_MAGIC = 0x564c,
    NFS_SUPER_MAGIC = 0x6969,
    NILFS_SUPER_MAGIC = 0x3434,
    NSFS_MAGIC = 0x6e736673,
    NTFS_SB_MAGIC = 0x5346544e,
    OCFS2_SUPER_MAGIC = 0x7461636f,
    OPENPROM_SUPER_MAGIC = 0x9fa1,
    OVERLAYFS_SUPER_MAGIC = 0x794c7630,
    PIPEFS_MAGIC = 0x50495045,
    PROC_SUPER_MAGIC = 0x9fa0,
    PSTOREFS_MAGIC = 0x6165676c,
    QNX4_SUPER_MAGIC = 0x002f,
    QNX6_SUPER_MAGIC = 0x68191122,
    RAMFS_MAGIC = 0x858458f6,
    REISERFS_SUPER_MAGIC = 0x52654973,
    ROMFS_MAGIC = 0x7275,
    SECURITYFS_MAGIC = 0x73636673,
    SELINUX_MAGIC = 0xf97cff8c,
    SMACK_MAGIC = 0x43415d53,
    SMB_SUPER_MAGIC = 0x517b,
    SOCKFS_MAGIC = 0x534f434b,
    SQUASHFS_MAGIC = 0x73717368,
    SYSFS_MAGIC = 0x62656572,
    SYSV2_SUPER_MAGIC = 0x012ff7b6,
    SYSV4_SUPER_MAGIC = 0x012ff7b5,
    TMPFS_MAGIC = 0x01021994,
    TRACEFS_MAGIC = 0x74726163,
    UDF_SUPER_MAGIC = 0x15013346,
    UFS_MAGIC = 0x00011954,
    USBDEVICE_SUPER_MAGIC = 0x9fa2,
    V9FS_MAGIC = 0x01021997,
    VXFS_SUPER_MAGIC = 0xa501fcf5,
    XENFS_SUPER_MAGIC = 0xabba1974,
    XENIX_SUPER_MAGIC = 0x012ff7b4,
    XFS_SUPER_MAGIC = 0x58465342,
    _XIAFS_SUPER_MAGIC = 0x012fd16d
};

struct statfs_t {
    uint64_t type;    /* Type of filesystem (see below) */
    uint64_t bsize;   /* Optimal transfer block size */
    uint64_t blocks;  /* Total data blocks in filesystem */
    uint64_t bfree;   /* Free blocks in filesystem */
    uint64_t bavail;  /* Free blocks available to unprivileged user */
    uint64_t files;   /* Total file nodes in filesystem */
    uint64_t ffree;   /* Free file nodes in filesystem */
    uuid_t fsid;      /* Filesystem ID */
    uint32_t namelen; /* Maximum length of filenames */
    uint64_t frsize;  /* Fragment size (since Linux 2.6) */
    uint64_t flags;   /* Mount flags of filesystem*/
};

class vfile
{
    shared_ptr<class vnode> _parent;
    [[maybe_unused]] ssize_t posP, posG;
    [[maybe_unused]] mtx_t fileIOLock;

  public:
    vfile(shared_ptr<class vnode> parent);
    ~vfile();
    int read(char *data, size_t sz);
    int write(const char *data, size_t sz);
    int seekg(long int offset, int origin);
};

class vfs
{
  public:
    shared_ptr<class vnode> vfs_vnodecovered; /* vnode we cover */
    int flag;                                 /* flags */
                                              // void* vfs_data; /* private data */
  public:
    vfs();
    virtual ~vfs();
    virtual int mount(uint64_t flags, shared_ptr<class vnode> blk_dev, shared_ptr<class vnode> &fs_root_directory) = 0;
    virtual int unmount(void) = 0;
    virtual int root(class vnode *&rootNode) = 0;
    virtual int statfs(shared_ptr<class vnode> rootNode, statfs_t &statfs) = 0;
    virtual int sync(void) = 0;
    virtual int fid(void) = 0;
    virtual int vget(void) = 0;
};

struct fileSystem {
    function<vfs *(void)> new_vfs;
    function<void(vfs *)> delete_vfs;
    // fileSystem(const fileSystem &fs) : new_vfs(fs.new_vfs), delete_vfs(fs.delete_vfs) {}
    // fileSystem(const fileSystem &fs) = default;
};

void init_vfs(void);
void register_filesystem(fileSystem fs, string fsName);
void unregister_filesystem(fileSystem fs);
int mknod(const char *pathname, shared_ptr<class vnode> dev);
// extern std::vector<vfs*> vfs_list;
extern shared_ptr<class vnode> rnode; //	Pointer to root vnode.
int mount_root(class vnode *vn);
int add_blk_dev(shared_ptr<class vnode> blk_dev);
class vnode *open_bdev(string dev_path);

enum OpenAt_Flags {
    FDCWD = -100,             /* Indicates that openat should use the current working
                                 directory. */
    SYMLINK_NOFOLLOW = 0x100, /* Do not follow symbolic links.  */
    REMOVEDIR = 0x200,        /* Remove directory instead of unlinking file.  */
    SYMLINK_FOLLOW = 0x400,   /* Follow symbolic links.  */
    NO_AUTOMOUNT = 0x800,     /* Suppress terminal automount traversal */
    EMPTY_PATH = 0x1000       /* Allow empty relative pathname */
};

enum Open_Flags {
    O_RDONLY = 1 << 0,   //    Open for reading only.
    O_WRONLY = 1 << 1,   //    Open for writing only.
    O_RDWR = 1 << 2,     //    Open for reading and writing. The result is undefined
                         //    if this flag is applied to a FIFO.
    O_NONBLOCK = 1 << 3, //    When opening a FIFO with O_RDONLY or O_WRONLY set: If
                         //    O_NONBLOCK is set: An open() for reading only will return
                         //    without delay. An open() for writing only will return an
                         //    error if no process currently has the file open for reading.
    O_DIRECTORY = 1 << 4,
    O_CLOEXEC = 1 << 5,
};

int lookup(const char *path, shared_ptr<class vnode> &node);
int create(const string &path, shared_ptr<class vnode> &node);

int open(const string &name, int oflag);
int open(const string &name, int oflag, vfile **file);

int openat(int dirfd, const char *pathname, int flags, mode_t mode);

int read(int fd, char *dst, size_t size);
int write(int fd, const char *src, size_t size);

int fseek(int fd, long int offset, int origin);

int mount(string mountPoint, string mountSource);
int getdents(int fd, vector<string> &dir);
string getCurrentPath();
#endif
