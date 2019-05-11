/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <stdint.h>
#include "fatgen.h"
#include "stdio.h"
#include "string.h"
#include <string>
#include <errno.h>
#include <kernel/vfs.h>
#include <DDI/driver.h>

#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]", x);
#define LOGSTR(x) printf("\n\"" #x "\" :[%s]", x);
#define DBG_OUTPUT printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);

using namespace std;

class fat32dir
{
  public:
	char DIR_Name[11];
	uint8_t DIR_Attr;
#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
	uint8_t DIR_NTRes;
	uint8_t DIR_CrtTimeTenth;
	uint16_t DIR_CrtTime;
	uint16_t DIR_CrtDate;
	uint16_t DIR_LstAccDate;
	uint16_t DIR_FstClusHI;
	uint16_t DIR_WrtTime;
	uint16_t DIR_WrtDate;
	uint16_t DIR_FstClusLO;
	uint32_t DIR_FileSize;

	string getName()
	{
		char filename[15] = {0};
		for (int i = 0; i < 8 && DIR_Name[i] != ' '; i++)
		{
			filename[i] = DIR_Name[i];
		}
		if (DIR_Name[8] != ' ')
		{
			auto tmp = strcat(filename, ".");
			for (int i = 8; i < 11 && DIR_Name[i] != ' '; i++)
			{
				*tmp++ = DIR_Name[i];
			}
		}
		return string(filename);
	}
	uint32_t getFirstCluster()
	{
		return (DIR_FstClusHI << 16) | (DIR_FstClusLO);
	}

} __attribute__((packed));

std::string convertFat32String(char name[11], bool isDir = false);

std::string convertFat32String(char name[11], bool isDir)
{
	char filename[15] = {0};
	copy(name, name + 8, filename);
	if (!isDir)
	{
		for (int i = 7; i >= 0; i--)
			if (filename[i] != ' ')
			{
				filename[i + 1] = '.';
				copy(name + 8, name + 11, filename + i + 2);
				break;
			}
			else
				filename[i] = 0;
	}
	return string(filename);
}

class fat_vnode : public vnode
{
	shared_ptr<vnode> dev_node;
	bpb bfat;
	fat32dir _dir;
	uint32_t start_cluster;
	uint32_t sectorSize;
	uint32_t sectorsPerCluster;

  public:
	fat_vnode(vfs *vfsp, shared_ptr<vnode> dev, const bpb &fat, uint32_t sclus, uint32_t _sectorSize, uint32_t _sectorsPerCluster, fat32dir *dir) : vnode(vfsp), dev_node(dev), bfat(fat), start_cluster(sclus), sectorSize(_sectorSize), sectorsPerCluster(_sectorsPerCluster), _dir{0}
	{
		if (dir)
		{
			setName(dir->getName().c_str());
			_dir = *dir;
			if (_dir.DIR_Attr & ATTR_DIRECTORY)
				v_type = VDIR;
			else
				v_type = VREG;
		}
		else
		{
			v_type = VDIR;
		}
	}
	~fat_vnode()
	{
		printf("[%s] fat_vnode destroyed!\n", getName().c_str());
	}

	int lookup(const char *const path, shared_ptr<vnode> &foundNode);

	int open(uint32_t flags);

	int bread(ssize_t position, size_t size, char *data, int *bytesRead);

	int ioctl(uint32_t command, void *data, int fflag);

	int readdir(vector<shared_ptr<vnode>> &vnodes);

	int mkdir(std::string name, shared_ptr<vnode> &pDir);

	int getMountedDevice(shared_ptr<vnode> &devNode);

	bool namecmp(const string &name) const
	{
		return !stricmp(name.c_str(), getName().c_str());
	}
};

class fat : public vfs
{
	shared_ptr<uint32_t> fatlist;

  public:
	fat() : fatlist() {}

	int unmount(void) { return ENOSYS; }
	int root(vnode *&rootNode) { return ENOSYS; }
	int statfs(shared_ptr<vnode> rootNode, statfs_t &statfs)
	{
		shared_ptr<vnode> MountedDevice;
		((fat_vnode *)rootNode.get())->getMountedDevice(MountedDevice);
		statfs.type = MSDOS_SUPER_MAGIC;
		statfs.bsize = 512;
		shared_ptr<char> data(new char[statfs.bsize]);
		MountedDevice->bread(0, 1, data.get(), nullptr);
		uint64_t fat_uuid = *(uint32_t *)(&(data.get()[0x43]));
		uint64_t uuid = fat_uuid << 48;
		statfs.fsid = uuid;
		return 0;
	}
	int sync(void) { return ENOSYS; }
	int fid(void) { return ENOSYS; }
	int vget(void) { return ENOSYS; }

	int mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory)
	{
		fs_root_directory = NULL;
		if (blk_dev == NULL)
			return true;
		//  FAT32 stuff
		shared_ptr<char> data;
		data = new char[512];
		blk_dev->bread(0, 1, data.get(), nullptr);
		bpb bfat = *((bpb *)data.get());
		if (memcmp(bfat.BS_FilSysType, "FAT32   ", 8))
			return EINVAL;
		uint64_t fat_uuid = *(uint32_t *)(&((char *)&bfat)[0x43]);
		uint64_t uuid = ((fat_uuid & 0xFFFF0000) << 16) | (fat_uuid & 0x0000FFFF);
		printf("Device UUID [%x-%x]\n", uuid >> 32, (uuid & 0xFFFFFFFF));
		uint32_t TotSec = 0;
		if (bfat.BPB_TotSec16 != 0)
			TotSec = bfat.BPB_TotSec16;
		else
			TotSec = bfat.BPB_TotSec32;
		uint32_t DataSec = TotSec - (bfat.BPB_RsvdSecCnt + (bfat.BPB_NumFATs * bfat.getFatSize()) + bfat.getRootDirSectorCount());
		uint32_t CountofClusters = DataSec / bfat.BPB_SecPerClus;
		//  get the fat list
		uint16_t totalSectorCount = ((((CountofClusters * (uint32_t)sizeof(uint32_t)) + bfat.BPB_BytsPerSec - 1) / bfat.BPB_BytsPerSec) * bfat.BPB_BytsPerSec) / (uint32_t)sizeof(uint32_t);
		fatlist = new uint32_t[CountofClusters];
		blk_dev->bread(bfat.BPB_RsvdSecCnt, ((CountofClusters * sizeof(uint32_t)) + bfat.BPB_BytsPerSec - 1) / bfat.BPB_BytsPerSec, (char *)fatlist.get(), nullptr);
		uint32_t root = bfat.getFirstSectorOfCluster(2);
		shared_ptr<char> root_sec(new char[512]);
		blk_dev->bread(root, 1, root_sec.get(), nullptr);
		auto BPB_BytsPerSec = bfat.BPB_BytsPerSec;
		fs_root_directory = make_shared<fat_vnode>(this, blk_dev, bfat, 2, BPB_BytsPerSec, bfat.BPB_SecPerClus, nullptr);
		fs_root_directory->v_type = VDIR;
		return false;
	}
	uint32_t nextCluster(uint32_t N)
	{
		return fatlist.get()[N];
	}
	uint32_t nextFreeCluster(uint32_t N)
	{
		auto fatlist_ptr = fatlist.get();
		uint32_t freeCluster = N;
		for (; fatlist_ptr[freeCluster] != 0; freeCluster++)
		{
		}
		if (fatlist_ptr[freeCluster] == 0)
		{
			fatlist_ptr[freeCluster] = 0x0FFFFFFF;
			return freeCluster;
		}
		else
			return -1;
	}
};
vfs *vfat_new_vfs() { return new fat; }
void vfat_delete_vfs(vfs *fs) { delete fs; }

fileSystem vfat = {vfat_new_vfs, vfat_delete_vfs};
void fat_init()
{
	printf("Initializing fat\n");
	register_filesystem(vfat, "vfat");
}

int fat_vnode::lookup(const char *const path, shared_ptr<vnode> &foundNode)
{
	if (v_type != VDIR)
		return ENOTDIR;
	shared_ptr<char> clusterData(new char[sectorSize * sectorsPerCluster]);
	uint32_t current_cluster = start_cluster;
	uint32_t current_sector = bfat.getFirstSectorOfCluster(current_cluster);
	dev_node->bread(current_sector, sectorsPerCluster, clusterData.get(), nullptr);
	fat32dir *dirArr = (fat32dir *)clusterData.get();
	for (uint32_t i = 0; i < sectorsPerCluster * 16; i++)
	{
		if (dirArr[i].DIR_Name[0] == 0)
		{
			break;
		}
		else if (dirArr[i].DIR_Name[0] == 0x5E)
		{
			continue;
		}
		if ((dirArr[i].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
		{
			continue;
		}
		if (!stricmp(dirArr[i].getName().c_str(), path))
		{
			foundNode = make_shared<fat_vnode>(v_vfsp, dev_node, bfat, dirArr[i].getFirstCluster(), sectorSize, sectorsPerCluster, &dirArr[i]);
			return 0;
		}
	}
	return ENOFILE;
}

int fat_vnode::open(uint32_t flags)
{
	return 0;
}

int fat_vnode::bread(ssize_t position, size_t size, char *data, int *bytesRead)
{
	size_t cluster_count = position / (sectorSize * sectorsPerCluster);
	size_t cluster_off = position % (sectorSize * sectorsPerCluster);
	uint32_t cluster = start_cluster;
	shared_ptr<char> tmpData(new char[sectorSize * sectorsPerCluster]);
	for (size_t i = 0; i < cluster_count; i++)
	{
		cluster = ((fat *)v_vfsp)->nextCluster(cluster);
	}
	while (cluster != 0x0FFFFFFF && size > 0)
	{
		//	which sector to get
		uint32_t sector = bfat.getFirstSectorOfCluster(cluster) + (cluster_off / sectorSize);
		//	offset inside sector where data begins
		uint32_t sector_off = (cluster_off % sectorSize);
		// how many sectors to get
		uint32_t sector_count = min((sector_off + size + sectorSize - 1) / sectorSize, sectorsPerCluster - (cluster_off / sectorSize));
		// read the sectors
		dev_node->bread(sector, sector_count, tmpData.get(), nullptr);
		auto size_to_get = min((uint32_t)size, sector_count * sectorSize, (sector_count * sectorSize) - sector_off);
		//	copy sectors to output
		auto dataPtr = tmpData.get();
		memcpy(data, &(dataPtr[sector_off]), size_to_get);
		//	change size to get
		size -= size_to_get;
		//	update data ptr
		data += size_to_get;
		//	next cluster
		cluster = ((fat *)v_vfsp)->nextCluster(cluster);
		if (bytesRead)
		{
			*bytesRead += size_to_get;
		}
		cluster_off = 0;
	}
	return 0;
}

int fat_vnode::ioctl(uint32_t command, void *data, int fflag)
{
	return 1;
}

int fat_vnode::getMountedDevice(shared_ptr<vnode> &devNode)
{
	devNode = dev_node;
	return 0;
}

int fat_vnode::readdir(vector<shared_ptr<vnode>> &vnodes)
{
	if (v_type != VDIR)
		return ENOTDIR;
	shared_ptr<char> clusterData(new char[sectorSize * sectorsPerCluster]);
	uint32_t current_cluster = start_cluster;
	uint32_t current_sector = bfat.getFirstSectorOfCluster(current_cluster);
	dev_node->bread(current_sector, sectorsPerCluster, clusterData.get(), nullptr);
	fat32dir *dirArr = (fat32dir *)clusterData.get();
	for (uint32_t i = 0; i < sectorsPerCluster * 16; i++)
	{
		if (dirArr[i].DIR_Name[0] == 0)
		{
			break;
		}
		else if (dirArr[i].DIR_Name[0] == 0x5E)
		{
			continue;
		}
		if ((dirArr[i].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
		{
			continue;
		}
		auto tmp = new fat_vnode(v_vfsp, dev_node, bfat, dirArr[i].getFirstCluster(), sectorSize, sectorsPerCluster, &dirArr[i]);
		vnodes.push_back(shared_ptr<vnode>(tmp));
	}
	return 0;
}

int fat_vnode::mkdir(std::string name, shared_ptr<vnode> &pDir)
{
	//	1.find next free cluster
	auto freeCluster = ((fat *)v_vfsp)->nextFreeCluster(start_cluster);
	//	2.find free diretory entry
	fat32dir dentry = {};
	//	3.associate directory entry to cluster
	return ENOSYS;
}
MODULT_INIT(fat_init)