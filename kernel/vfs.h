/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include <stddef.h>
#include <functional>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

using namespace std;

#define BLKPBSZGET 1
#define BLKGETSIZE64 2

enum vtype
{
	VNON = 0,
	VREG = 1,
	VDIR = 2,
	VBLK = 3,
	VCHR = 4,
	VLNK = 5,
	VSOCK = 6,
	VBAD = 7
};
enum FS_TYPE_MAGIC
{
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

struct statfs_t
{
	uint64_t type;	/* Type of filesystem (see below) */
	uint64_t bsize;   /* Optimal transfer block size */
	uint64_t blocks;  /* Total data blocks in filesystem */
	uint64_t bfree;   /* Free blocks in filesystem */
	uint64_t bavail;  /* Free blocks available to unprivileged user */
	uint64_t files;   /* Total file nodes in filesystem */
	uint64_t ffree;   /* Free file nodes in filesystem */
	uint64_t fsid;	/* Filesystem ID */
	uint32_t namelen; /* Maximum length of filenames */
	uint64_t frsize;  /* Fragment size (since Linux 2.6) */
	uint64_t flags;   /* Mount flags of filesystem*/
};

class vnode
{
  public:
	uint16_t v_shlockc;			 /* # of shared locks */
	uint16_t v_exlockc;			 /* # of exclusive locks */
	uint32_t v_type;			 /* vnode type */
	class vfs *v_vfsmountedhere; /* covering vfs */
	class vfs *v_vfsp;			 /* vfs we are in */
								 //vnode *v_root;

  private:
	uint16_t v_flag; /* vnode flags */
	string v_name;
	uint64_t v_count; /* reference count */
	vector<shared_ptr<vnode>> special_nodes;
	vector<shared_ptr<vnode>> dnode_cache;
	bool runOnce = false;
	vector<shared_ptr<vnode>> ref_nodes;

  protected:
	void setName(const char *name);

  public:
	vnode(class vfs *vfsp);
	vnode(const vnode &) = delete;
	constexpr vnode &operator=(const vnode &) = delete;
	virtual ~vnode();
	virtual int access(void);
	virtual int bmap(void);
	virtual int bread(ssize_t position, size_t size, char *data, int *bytesRead) = 0;
	virtual int bwrite(void);
	virtual int brelse(void);
	virtual int close(void);
	virtual int create(void);
	int dolookup(const char *const path, shared_ptr<vnode> &foundNode);
	int doreaddir(vector<shared_ptr<vnode>> &vnodes);
	int doopen(shared_ptr<vnode> &node, uint32_t flags, class vfile **file);
	virtual int fsync(void);
	const string &getName() const;
	virtual int getattr(void);
	virtual int ioctl(uint32_t command, void *data, int fflag);
	virtual int inactive(void);
	virtual int link(void);
	virtual int mkdir(std::string name, shared_ptr<vnode> &pDir);
	int mknod(shared_ptr<vnode> &current_node, shared_ptr<vnode> pNode);
	virtual int rdwr(void);
	virtual int rename(string name);
	virtual int rmdir(void);
	virtual int readlink(void);
	virtual int remove(void);
	virtual int select(void);
	virtual int setattr(void);
	virtual int symlink(void);
	virtual int strategy(void);
	virtual bool namecmp(const string &name) const;
	void addRef(shared_ptr<vnode> node) { ref_nodes.push_back(node); }
	bool isDirectory() { return (v_type == VDIR); }
	bool isFile() { return (v_type == VREG); }
	bool isBlockDevice() { return (v_type == VBLK); }

  protected:
	virtual int lookup(const char *const path, shared_ptr<vnode> &foundNode);
	virtual int readdir(vector<shared_ptr<vnode>> &vnodes) = 0;
	virtual int open(uint32_t flags) = 0;
};

class vfile
{
	shared_ptr<vnode> _parent;
	ssize_t posP, posG;
	mtx_t fileIOLock;

  public:
	vfile(shared_ptr<vnode> parent);
	~vfile();
	int read(char *data, size_t sz);
	int seekg(long int offset, int origin);
};

class vfs
{
  public:
	shared_ptr<vnode> vfs_vnodecovered; /* vnode we cover */
	int flag;							/* flags */
										//void* vfs_data; /* private data */
  public:
	vfs();
	virtual ~vfs();
	virtual int mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory) = 0;
	virtual int unmount(void) = 0;
	virtual int root(vnode *&rootNode) = 0;
	virtual int statfs(shared_ptr<vnode> rootNode, statfs_t &statfs) = 0;
	virtual int sync(void) = 0;
	virtual int fid(void) = 0;
	virtual int vget(void) = 0;
};

struct fileSystem
{
	function<vfs *(void)> new_vfs;
	function<void(vfs *)> delete_vfs;
};

void init_vfs(void);
void register_filesystem(fileSystem fs, string fsName);
void unregister_filesystem(fileSystem fs);
int mknod(const char *pathname, shared_ptr<vnode> dev);
//extern std::vector<vfs*> vfs_list;
extern shared_ptr<vnode> rnode; //	Pointer to root vnode.
int mount_root(vnode *vn);
int add_blk_dev(shared_ptr<vnode> blk_dev);
vnode *open_bdev(string dev_path);

enum OpenAt_Flags
{
	FDCWD = -100,			  /* Indicates that openat should use the current working directory. */
	SYMLINK_NOFOLLOW = 0x100, /* Do not follow symbolic links.  */
	REMOVEDIR = 0x200,		  /* Remove directory instead of unlinking file.  */
	SYMLINK_FOLLOW = 0x400,   /* Follow symbolic links.  */
	NO_AUTOMOUNT = 0x800,	 /* Suppress terminal automount traversal */
	EMPTY_PATH = 0x1000		  /* Allow empty relative pathname */
};

enum Open_Flags
{
	O_RDONLY = 1 << 0,   //    Open for reading only.
	O_WRONLY = 1 << 1,   //    Open for writing only.
	O_RDWR = 1 << 2,	 //    Open for reading and writing. The result is undefined if this flag is applied to a FIFO.
	O_NONBLOCK = 1 << 3, //    When opening a FIFO with O_RDONLY or O_WRONLY set: If O_NONBLOCK is set:
						 //    An open() for reading only will return without delay. An open() for writing only will return an error if no process currently has the file open for reading.
	O_DIRECTORY = 1 << 4,
	O_CLOEXEC = 1 << 5,
};

typedef uint64_t mode_t;
int lookup(const char *path, shared_ptr<vnode> &node);
int open(const string &name);
void close(int fd);
int openat(int dirfd, const string &pathname, int flags, mode_t mode);
int chdir(const char *path);

int read(int fd, char *dst, size_t size);
int fseek(int fd, long int offset, int origin);
int mount(string mountPoint, string mountSource);
int getdents(int fd, vector<string> &dir);
string getCurrentPath();
#endif
