/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
#include <ddi/driver.h>

#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]", x);
#define LOGSTR(x) printf("\n\"" #x "\" :[%s]", x);
#define DBG_OUTPUT printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);

using namespace std;

struct bpb
{
	uint8_t BS_jmpBoot[3];
	char BS_OEMName[8];
	uint16_t BPB_BytsPerSec;
	uint8_t BPB_SecPerClus;
	uint16_t BPB_RsvdSecCnt;
	uint8_t BPB_NumFATs;
	uint16_t BPB_RootEntCnt;
	uint16_t BPB_TotSec16;
	uint8_t BPB_Media;
	uint16_t BPB_FATSz16;
	uint16_t BPB_SecPerTrk;
	uint16_t BPB_NumHeads;
	uint32_t BPB_HiddSec;
	uint32_t BPB_TotSec32;
	//  FAT32
	uint32_t BPB_FATSz32;
	uint16_t BPB_ExtFlags;
	uint16_t BPB_FSVer;
	uint32_t BPB_RootClus;
	uint16_t BPB_FSInfo;
	uint16_t BPB_BkBootSec;
	uint8_t BPB_Reserved[12];
	uint8_t BS_DrvNum;
	uint8_t BS_Reserved1;
	uint8_t BS_BootSig;
	uint32_t BS_VolID;
	uint8_t BS_VolLab[11];
	uint8_t BS_FilSysType[8];
} __attribute__((packed));

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
		copy(DIR_Name, DIR_Name + 8, filename);
		for (int i = 7; i >= 0; i--)
			if (filename[i] != ' ')
			{
				filename[i + 1] = '.';
				copy(DIR_Name + 8, DIR_Name + 11, filename + i + 2);
				break;
			}
			else
				filename[i] = 0;
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
	class fat *parent;
	vnode *dev_node;
	uint32_t start_cluster;
	uint32_t sectorSize;
	uint32_t sectorsPerCluster;

  public:
	fat_vnode(class fat *_parent, vnode *dev, uint32_t sclus, uint32_t _sectorSize, uint32_t _sectorsPerCluster) : parent(_parent), dev_node(dev), start_cluster(sclus), vnode(), sectorSize(_sectorSize), sectorsPerCluster(_sectorsPerCluster)
	{
		v_type = VDIR;
	}

	int bread(ssize_t position, size_t size, char *data);

	int lookup(const char *const path, vnode *&foundNode);

	int open(uint32_t flags, vfile *&file);

	int ioctl(uint32_t command, void *data, int fflag);

	int getMountedDevice(vnode *&devNode);
};

class fat : public vfs
{
	vnode *dev_node;
	bpb *bfat;
	uint32_t FirstDataSector;
	shared_ptr<uint32_t> fatlist;
	uint32_t totalSectorCount;
	
  public:
	fat() : dev_node(0)
	{
	}

	int unmount(void) {}
	int root(vnode *&rootNode) {}
	int statfs(vnode* rootNode, statfs_t& statfs)
	{
		vnode *MountedDevice = nullptr;
		((fat_vnode *)rootNode)->getMountedDevice(MountedDevice);
		statfs.type = MSDOS_SUPER_MAGIC;
		statfs.bsize = 512;
		shared_ptr<char> data(new char[statfs.bsize]);
		MountedDevice->bread(0, 1, data.get());
		uint64_t fat_uuid = *(uint32_t *)(&(data.get()[0x43]));
		uint64_t uuid = ((fat_uuid & 0xFFFF0000) << 16) | (fat_uuid & 0x0000FFFF);		
		statfs.fsid = uuid;
		return 0;
	}
	int sync(void) {}
	int fid(void) {}
	int vget(void) {}

	int mount(uint64_t flags, vnode *blk_dev, vnode *&fs_root_directory)
	{

		//printf("\nmount issued for [%s]", dev_path.c_str());
		fs_root_directory = NULL;
		//mount_bdev(this, flags, dev_path);
		dev_node = blk_dev;
		if (dev_node == NULL)
			return true;

		//  FAT32 stuff
		shared_ptr<char> data;
		data = new char[512];
		dev_node->bread(0, 1, data.get());
		bfat = (bpb *)data.get();
		uint64_t fat_uuid = *(uint32_t *)(&((char *)bfat)[0x43]);
		uint64_t uuid = ((fat_uuid & 0xFFFF0000) << 16) | (fat_uuid & 0x0000FFFF);
		printf("Device UUID [%x-%x]\n", uuid >> 32, (uuid & 0xFFFFFFFF));
		uint32_t RootDirSectors = ((bfat->BPB_RootEntCnt * 32) + (bfat->BPB_BytsPerSec - 1)) / bfat->BPB_BytsPerSec;
		//LOGHEX(bfat->BPB_RootEntCnt)
		uint32_t FATSz = 0;
		uint32_t TotSec = 0;
		if (bfat->BPB_FATSz16 != 0)
			FATSz = bfat->BPB_FATSz16;
		else
			FATSz = bfat->BPB_FATSz32;
		//LOGHEX(RootDirSectors);
		FirstDataSector = bfat->BPB_RsvdSecCnt + (bfat->BPB_NumFATs * FATSz) + RootDirSectors;

		/*char type[10] = {0};
		for (int i = 0; i < 8; i++)
			type[i] = bfat->BS_FilSysType[i];*/

		//FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;

		//uint32_t RootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec - 1)) // BPB_BytsPerSec;

		if (bfat->BPB_TotSec16 != 0)
			TotSec = bfat->BPB_TotSec16;
		else
			TotSec = bfat->BPB_TotSec32;

		uint32_t DataSec = TotSec - (bfat->BPB_RsvdSecCnt + (bfat->BPB_NumFATs * FATSz) + RootDirSectors);
		uint32_t CountofClusters = DataSec / bfat->BPB_SecPerClus;

		//printf("\nCluster Count:[%d]", CountofClusters);

		//  get the fat list
		totalSectorCount = ((((CountofClusters * (uint32_t)sizeof(uint32_t)) + bfat->BPB_BytsPerSec - 1) / bfat->BPB_BytsPerSec) * bfat->BPB_BytsPerSec) / (uint32_t)sizeof(uint32_t);
		fatlist = new uint32_t[CountofClusters];

		dev_node->bread(bfat->BPB_RsvdSecCnt, (CountofClusters * sizeof(uint32_t)) / bfat->BPB_BytsPerSec, (char *)fatlist.get());
		LOGHEX(fatlist.get()[8]);
		uint32_t root = getFirstSectorOfCluster(2);
		shared_ptr<char> root_sec(new char[512]);
		//        LOGHEX(root)
		dev_node->bread(root, 1, root_sec.get());

		fat32dir *f32d = (fat32dir *)root_sec.get();

		for (int i = 0; i < 10; i++)
			if ((f32d[i].DIR_Attr & ATTR_LONG_NAME) != ATTR_LONG_NAME)
				printf("\nName:[%s]", f32d[i].DIR_Name);

		fs_root_directory = new fat_vnode(this, dev_node, 2, bfat->BPB_BytsPerSec, bfat->BPB_SecPerClus);
		return false;
	}

	int read(ssize_t position, size_t size, char *_data)
	{
		LOGHEX(position);
		////size *= bfat->BPB_SecPerClus*bfat->BPB_BytsPerSec;
		//int sectorIndex = getFirstSectorOfCluster(position);
		//printf("\nreading from block drive [%s]", dev_node->v_name.c_str());
		//size_t blockSize = 0;
		//if (dev_node->ioctl(BLKPBSZGET, &blockSize, 0) == 0 && blockSize > 0)
		//{
		//	shared_ptr<char> sectorData(new char[blockSize]);
		//	for (size_t dataRead = 0; dataRead < size; dataRead += blockSize)
		//	{
		//		dev_node->bread(sectorIndex, 1, sectorData.get());
		//		memcpy(&_data[dataRead], sectorData.get(), min(blockSize, size - dataRead));
		//		sectorIndex++;
		//	}

		//}
	}

	uint32_t getFirstSectorOfCluster(uint32_t N)
	{
		LOGHEX(FirstDataSector);
		return ((N - 2) * bfat->BPB_SecPerClus) + FirstDataSector;
	}

	uint32_t getClusterByIndex(uint32_t index, uint32_t currentCluster)
	{
		for (size_t i = 0; i < index; i++)
		{
			currentCluster = fatlist.get()[currentCluster];
		}
		return currentCluster;
	}
};

void fat_init()
{
	printf("Initializing fat\n");
	fat *fs = new fat();
	register_filesystem(fs, "vfat");
}

int fat_vnode::bread(ssize_t position, size_t size, char *data)
{
	parent->read(position, size, data);
}

int fat_vnode::lookup(const char *const path, vnode *&foundNode)
{
	size_t name_len = 0;
	for (name_len = 0; (path[name_len] != 0) && (path[name_len] != '/'); name_len++)
		;
	string file(path, name_len);
	LOGSTR(file.c_str());
	shared_ptr<char> clusterData(new char[sectorSize * sectorsPerCluster]);

	uint32_t current_cluster = start_cluster;
	uint32_t current_sector = parent->getFirstSectorOfCluster(current_cluster);

	fat32dir fatDir;
	bool fileFound = false;

	DBG_OUTPUT;
	bool allDirsParsed = false;
	for (int sec = 0; sec < 1 && !allDirsParsed; sec++)
	{
		parent->read(current_sector + sec, sectorSize, clusterData.get());
		//bread(start_cluster + sec, sectorSize*sectorsPerCluster, clusterData.get());
		fat32dir *dirArr = (fat32dir *)clusterData.get();

		for (int i = 0; i < 16 * sectorsPerCluster; i++)
		{
			string filename;
			if (dirArr[i].DIR_Name[0] == 0)
			{
				allDirsParsed = true;
				break;
			}
			else if (dirArr[i].DIR_Name[0] == 0x5E)
			{
				printf("\nEmpty entry skipping...");
				continue;
			}
			if ((dirArr[i].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
			{
				printf("\nCannot print filename. Not implemented");
				continue;
			}
			else
			{
				filename = convertFat32String(dirArr[i].DIR_Name, dirArr[i].DIR_Attr == ATTR_DIRECTORY);
				LOGSTR(filename.c_str());
			}
			if (filename == file)
			{
				fatDir = dirArr[i];
				fileFound = true;
				break;
			}
		}
		if (fileFound)
			break;
	}

	if (!fileFound)
	{
		return 1;
	}

	LOGSTR(fatDir.getName().c_str());
	LOGHEX(fatDir.getFirstCluster());
	LOGHEX(parent->getFirstSectorOfCluster(fatDir.getFirstCluster()));

	foundNode = new fat_vnode(parent, NULL, fatDir.getFirstCluster(), sectorSize, sectorsPerCluster);

	return 0;
}

int fat_vnode::open(uint32_t flags, vfile *&file)
{
	file = new vfile(this);
	if (file == 0)
		return ENOMEM;
	return 0;
}

int fat_vnode::ioctl(uint32_t command, void *data, int fflag)
{
	return 1;
}

int fat_vnode::getMountedDevice(vnode *&devNode)
{
	devNode = dev_node;
	return 0;
}

MODULT_INIT(fat_init)