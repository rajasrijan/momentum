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

#include <stdint.h>
#include "fatgen.h"
#include "stdio.h"
#include "string.h"
#include <DDI/driver.h>
#include <errno.h>
#include <kernel/vfs.h>
#include <kernel/vnode.h>
#include <string>
#include <ctype.h>
#include <algorithm>

using namespace std;

#pragma pack(push, 1)
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
        for (int idx = 0; idx < 8 && DIR_Name[idx] != ' '; idx++)
        {
            filename[idx] = DIR_Name[idx];
        }
        if (DIR_Name[8] != ' ' && DIR_Name[8] != 0x00)
        {
            auto tmp = strcat(filename, ".");
            for (int idx = 8; idx < 11 && DIR_Name[idx] != ' '; idx++)
            {
                *tmp++ = DIR_Name[idx];
            }
        }
        return string(filename);
    }
    uint32_t getFirstCluster()
    {
        return (DIR_FstClusHI << 16) | (DIR_FstClusLO);
    }
};
#pragma pack(pop)

std::string convertFat32String(char name[11], bool isDir = false);

std::string convertFat32String(char name[11], bool isDir)
{
    char filename[15] = {0};
    copy(name, name + 8, filename);
    if (!isDir)
    {
        for (int idx = 7; idx >= 0; idx--)
            if (filename[idx] != ' ')
            {
                filename[idx + 1] = '.';
                copy(name + 8, name + 11, filename + idx + 2);
                break;
            }
            else
                filename[idx] = 0;
    }
    return string(filename);
}

class fat_vnode : public vnode
{
    const bpb bfat;
    fat32dir _dir;
    uint32_t start_cluster;

  public:
    fat_vnode(vfs *vfsp, const bpb &fat, uint32_t sclus, fat32dir *dir) : vnode(vfsp), bfat(fat), _dir{}, start_cluster(sclus)
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

    int open(uint64_t flags);

    int bread(ssize_t position, size_t size, char *sector_data, int *bytesRead);

    int bwrite(ssize_t position, size_t size, const char *sector_data, int *bytesWritten);

    int ioctl(uint32_t command, void *sector_data, int fflag);

    int readdir(vector<shared_ptr<vnode>> &vnodes);

    int mkdir(std::string name, shared_ptr<vnode> &pDir);

    int getMountedDevice(shared_ptr<vnode> &devNode);

    int create(const string &path, shared_ptr<vnode> &created_node);
};

class fat : public vfs
{
    uint32_t CountofClusters;
    shared_ptr<uint32_t> fatlist;
    shared_ptr<vnode> parent_dev;
    bpb bfat;

  public:
    fat() : fatlist()
    {
    }

    int unmount(void)
    {
        return ENOSYS;
    }
    int root(vnode *&rootNode)
    {
        return ENOSYS;
    }
    int statfs(shared_ptr<vnode> rootNode, statfs_t &statfs)
    {
        statfs.type = MSDOS_SUPER_MAGIC;
        statfs.bsize = bfat.BPB_BytsPerSec;
        statfs.fsid = calculate_uuid((char *)&bfat);
        return 0;
    }
    int sync(void)
    {
        return ENOSYS;
    }
    int fid(void)
    {
        return ENOSYS;
    }
    int vget(void)
    {
        return ENOSYS;
    }

    int mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory)
    {
        int ret = 0;
        uint64_t sector_size = 512;
        fs_root_directory = nullptr;
        shared_ptr<char> sector_data;

        if (blk_dev == NULL)
            return true;

        ret = blk_dev->ioctl(BLKPBSZGET, &sector_size, 0);
        if (ret < 0)
        {
            printf("Failed to get device block size");
            return ret;
        }

        sector_data = new char[sector_size];

        blk_dev->bread(0, 1, sector_data.get(), nullptr);
        bfat = *((bpb *)sector_data.get());
        if (memcmp(bfat.BS_FilSysType, "FAT32   ", 8))
            return -EINVAL;

        uuid_t uuid = calculate_uuid((char *)&bfat);
        printf("Device UUID [%s]\n", std::to_string(uuid).c_str());

        uint32_t TotSec = 0;
        if (bfat.BPB_TotSec16 != 0)
            TotSec = bfat.BPB_TotSec16;
        else
            TotSec = bfat.BPB_TotSec32;

        uint32_t DataSec = TotSec - (bfat.BPB_RsvdSecCnt + (bfat.BPB_NumFATs * bfat.getFatSize()) + bfat.getRootDirSectorCount());
        CountofClusters = DataSec / bfat.BPB_SecPerClus;
        CountofClusters = ((CountofClusters + 127) / 128) * 128;
        //  get the fat list
        //  uint16_t totalSectorCount = ((((CountofClusters * (uint32_t)sizeof(uint32_t)) + bfat.BPB_BytsPerSec - 1) /
        //  bfat.BPB_BytsPerSec) * bfat.BPB_BytsPerSec) / (uint32_t)sizeof(uint32_t);
        fatlist = new uint32_t[CountofClusters];
        blk_dev->bread(bfat.BPB_RsvdSecCnt, ((CountofClusters * sizeof(uint32_t)) + bfat.BPB_BytsPerSec - 1) / bfat.BPB_BytsPerSec, (char *)fatlist.get(),
                       nullptr);
        uint32_t root = bfat.getFirstSectorOfCluster(2);
        shared_ptr<char> root_sec(new char[512]);
        blk_dev->bread(root, 1, root_sec.get(), nullptr);

        fs_root_directory = make_shared<fat_vnode>(this, bfat, 2, nullptr);
        fs_root_directory->v_type = VDIR;
        parent_dev = blk_dev;
        return false;
    }
    uint32_t nextCluster(uint32_t N)
    {
        return fatlist.get()[N];
    }
    uint32_t allocCluster(uint32_t N)
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

    void flush_cluster_chain()
    {
        parent_dev->bwrite(bfat.BPB_RsvdSecCnt, ((CountofClusters * sizeof(uint32_t)) + bfat.BPB_BytsPerSec - 1) / bfat.BPB_BytsPerSec, (char *)fatlist.get(),
                           nullptr);
    }

    uuid_t calculate_uuid(char *sector)
    {
        uuid_t fat_uuid = {};
        fat_uuid.ui32[2] = *(uint32_t *)(&sector[0x43]);
        return fat_uuid;
    }

    int readCluster(uint64_t offset, size_t count, char *pdata)
    {
        uint32_t current_sector = bfat.getFirstSectorOfCluster(offset);
        return parent_dev->bread(current_sector, count * bfat.BPB_SecPerClus, pdata, nullptr);
    }

    int writeCluster(uint64_t offset, size_t count, char *pdata)
    {
        uint32_t current_sector = bfat.getFirstSectorOfCluster(offset);
        return parent_dev->bwrite(current_sector, count * bfat.BPB_SecPerClus, pdata, nullptr);
    }

    int readSector(uint64_t offset, size_t count, char *pdata)
    {
        return parent_dev->bread(offset, count, pdata, nullptr);
    }
};
vfs *vfat_new_vfs()
{
    return new fat;
}
void vfat_delete_vfs(vfs *fs)
{
    delete fs;
}

fileSystem vfat = {vfat_new_vfs, vfat_delete_vfs};
void fat_init()
{
    printf("Initializing fat\n");
    register_filesystem(vfat, "vfat");
}

int fat_vnode::lookup(const char *const path, shared_ptr<vnode> &foundNode)
{
    if (v_type != VDIR)
        return -ENOTDIR;
    auto vfat_vfs = reinterpret_cast<fat *>(v_vfsp);
    shared_ptr<char> clusterData(new char[bfat.BPB_BytsPerSec * bfat.BPB_SecPerClus]);

    uint32_t current_cluster = start_cluster;

    vfat_vfs->readCluster(current_cluster, 1, clusterData.get());
    fat32dir *dirArr = (fat32dir *)clusterData.get();
    for (uint32_t idx = 0; idx < bfat.BPB_SecPerClus * 16; idx++)
    {
        if (dirArr[idx].DIR_Name[0] == 0)
        {
            break;
        }
        else if (dirArr[idx].DIR_Name[0] == 0x5E)
        {
            continue;
        }
        if ((dirArr[idx].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
        {
            continue;
        }
        if (!strcmp(dirArr[idx].getName().c_str(), path))
        {
            foundNode = make_shared<fat_vnode>(v_vfsp, bfat, dirArr[idx].getFirstCluster(), &dirArr[idx]);
            return 0;
        }
    }
    return -ENOFILE;
}

int fat_vnode::open(uint64_t flags)
{
    asm("cli;hlt");
    return -ENOSYS;
}

int fat_vnode::bread(ssize_t position, size_t size, char *sector_data, int *bytesRead)
{
    auto vfat_vfs = reinterpret_cast<fat *>(v_vfsp);
    const size_t cluster_size = bfat.BPB_BytsPerSec * bfat.BPB_SecPerClus;
    size_t cluster_count = position / cluster_size;
    size_t cluster_off = position % cluster_size;
    uint32_t cluster = start_cluster;
    shared_ptr<char> tmpData(new char[cluster_size]);
    for (size_t idx = 0; idx < cluster_count; idx++)
    {
        cluster = ((fat *)v_vfsp)->nextCluster(cluster);
    }
    while (cluster != 0x0FFFFFFF && size > 0)
    {
        //	which sector to get
        uint32_t sector = bfat.getFirstSectorOfCluster(cluster) + (cluster_off / bfat.BPB_BytsPerSec);
        //	offset inside sector where sector_data begins
        uint32_t sector_off = (cluster_off % bfat.BPB_BytsPerSec);
        // how many sectors to get
        uint32_t sector_count = min((sector_off + size + bfat.BPB_BytsPerSec - 1) / bfat.BPB_BytsPerSec,
                                    bfat.BPB_SecPerClus - (cluster_off / bfat.BPB_BytsPerSec));
        // read the sectors
        vfat_vfs->readSector(sector, sector_count, tmpData.get());
        auto size_to_get = min(min((uint32_t)size, sector_count * bfat.BPB_BytsPerSec), (sector_count * bfat.BPB_BytsPerSec) - sector_off);
        //	copy sectors to output
        auto dataPtr = tmpData.get();
        memcpy(sector_data, &(dataPtr[sector_off]), size_to_get);
        //	change size to get
        size -= size_to_get;
        //	update sector_data ptr
        sector_data += size_to_get;
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

int fat_vnode::bwrite(ssize_t position, size_t size, const char *sector_data, int *bytesWritten)
{
    return -ENOSYS;
}

int fat_vnode::ioctl(uint32_t command, void *sector_data, int fflag)
{
    return 1;
}

int fat_vnode::getMountedDevice(shared_ptr<vnode> &devNode)
{
    // devNode = dev_node;
    return 0;
}

int fat_vnode::readdir(vector<shared_ptr<vnode>> &vnodes)
{
    if (v_type != VDIR)
        return ENOTDIR;
    auto vfat_vfs = reinterpret_cast<fat *>(v_vfsp);
    const size_t cluster_size = bfat.BPB_BytsPerSec * bfat.BPB_SecPerClus;

    shared_ptr<char> clusterData(new char[cluster_size]);
    uint32_t current_cluster = start_cluster;

    vfat_vfs->readCluster(current_cluster, 1, clusterData.get());
    fat32dir *dirArr = (fat32dir *)clusterData.get();
    for (uint32_t idx = 0; idx < bfat.BPB_SecPerClus * 16; idx++)
    {
        if (dirArr[idx].DIR_Name[0] == 0)
        {
            break;
        }
        else if (dirArr[idx].DIR_Name[0] == 0x5E)
        {
            continue;
        }
        if ((dirArr[idx].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
        {
            continue;
        }
        auto tmp = new fat_vnode(v_vfsp, bfat, dirArr[idx].getFirstCluster(), &dirArr[idx]);
        vnodes.push_back(shared_ptr<vnode>(tmp));
    }
    return 0;
}

int fat_vnode::mkdir(std::string name, shared_ptr<vnode> &pDir)
{
    int ret = 0;
    if (!isDirectory())
        return -ENOTDIR;

    const size_t cluster_size = bfat.BPB_BytsPerSec * bfat.BPB_SecPerClus;
    const size_t dir_entries_per_cluster = bfat.BPB_SecPerClus * 16;

    char new_name[11] = {};

    auto ext = strrchar(name.c_str(), '.');
    if (ext != nullptr)
    {
        strcpy(&(new_name[8]), ext + 1);
        memcpy(new_name, name.c_str(), min<uint64_t>(8ul, (uint64_t)(ext - name.c_str())));
    }
    else
    {
        memcpy(new_name, name.c_str(), min(8ul, name.size()));
    }

    for (size_t i = 0; i < 11; i++)
    {
        new_name[i] = toupper(new_name[i]);
    }

    //	1.find next free cluster
    auto vfat_vfs = reinterpret_cast<fat *>(v_vfsp);

    auto freeCluster = vfat_vfs->allocCluster(start_cluster);
    //	2.find free diretory entry
    shared_ptr<char> clusterData(new char[cluster_size]);
    uint32_t current_cluster = start_cluster;

    vfat_vfs->readCluster(current_cluster, 1, clusterData.get());
    fat32dir *dirArr = (fat32dir *)clusterData.get();

    size_t idx = 0;
    for (idx = 0; idx < dir_entries_per_cluster; idx++)
    {
        if (dirArr[idx].DIR_Name[0] == 0)
        {
            break;
        }
        else if (dirArr[idx].DIR_Name[0] == 0x5E)
        {
            continue;
        }
        if ((dirArr[idx].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
        {
            continue;
        }
        if (!memcmp(new_name, dirArr[idx].DIR_Name, 11))
        {
            return -EEXIST;
        }
    }
    if (idx >= dir_entries_per_cluster)
        return -ENOMEM;
    for (int i = 0; i < 11; i++)
        dirArr[idx].DIR_Name[i] = toupper(new_name[i]);
    //	3.associate directory entry to cluster
    dirArr[idx].DIR_FstClusHI = freeCluster >> 16;
    dirArr[idx].DIR_FstClusLO = freeCluster & 0xFFFF;
    dirArr[idx].DIR_Attr = ATTR_DIRECTORY;

    //  write to disk
    ret = vfat_vfs->writeCluster(current_cluster, 1, clusterData.get());
    if (ret < 0)
        return ret;
    pDir = make_shared<fat_vnode>(v_vfsp, bfat, freeCluster, &dirArr[idx]);
    vfat_vfs->flush_cluster_chain();
    return 0;
}
int fat_vnode::create(const string &path, shared_ptr<vnode> &created_node)
{
    return -EPERM;
}
MODULT_INIT(fat_init)