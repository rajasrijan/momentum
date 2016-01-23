#include <stdint.h>
#include "fatgen.h"
#include "stdio.h"
#include "../DDI/filesystem.h"
#include "string.h"

#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]",x);
#define LOGSTR(x) printf("\n\"" #x "\" :[%s]",x);
#define DBG_OUTPUT printf("\nLine %d, function %s(), file %s",__LINE__,__FUNCTION__,__FILE__);

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

struct fat32dir
{
    char DIR_Name[11];
    uint8_t DIR_Attr;
#define ATTR_READ_ONLY    0x01
#define ATTR_HIDDEN  0x02
#define ATTR_SYSTEM  0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE   0x20
#define ATTR_LONG_NAME  (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
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
} __attribute__((packed));
std::string convertFat32String(char name[11]);

std::string convertFat32String(char name[11])
{
    char filename[15] = {0};
    copy(name, name + 8, filename);
    for (int i = 7; i >= 0; i--)
        if (filename[i] != ' ')
        {
            filename[i + 1] = '.';
            copy(name + 8, name + 11, filename + i + 2);
            break;
        }
        else
            filename[i] = 0;
    return string(filename);
}

class fat_vnode : public vnode
{
    class fat* parent;
    vnode* dev_node;
    size_t start_cluster;
    size_t sectorSize;
    size_t sectorsPerCluster;
public:

    fat_vnode(class fat* _parent, vnode* dev, size_t sclus, size_t _sectorSize, size_t _sectorsPerCluster) : parent(_parent), dev_node(dev), start_cluster(sclus), vnode(0), sectorSize(_sectorSize), sectorsPerCluster(_sectorsPerCluster)
    {
        v_type = VDIR;
    }

    int bread(int position, int size, shared_ptr<char> data);

    int lookup(const char* const path);
};

class fat : public filesystem
{
    vnode* dev_node;
    shared_ptr<char> data;
    bpb* bfat;
    uint32_t FirstDataSector;

public:

    fat() : dev_node(0)
    {
        name = "fat";
    }

    bool mount(uint32_t flags, std::string dev_path, vnode*& fs_root_directory)
    {

        //printf("\nmount issued for [%s]", dev_path.c_str());
        fs_root_directory = NULL;
        //mount_bdev(this, flags, dev_path);
        dev_node = open_bdev(dev_path);
        if (dev_node == NULL)
            return true;

        //  FAT32 stuff
        data = new char[512];
        dev_node->bread(0, 1, data);
        bfat = (bpb*) data.get();

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
        uint32_t words_to_allocate = ((((CountofClusters * sizeof (uint32_t)) + bfat->BPB_BytsPerSec - 1) / bfat->BPB_BytsPerSec) * bfat->BPB_BytsPerSec) / sizeof (uint32_t);
        //printf("\nwords_to_allocate Count:[%d]", words_to_allocate);
        shared_ptr<uint32_t> fatlist = new uint32_t[words_to_allocate];

        dev_node->bread(bfat->BPB_RsvdSecCnt, words_to_allocate / bfat->BPB_BytsPerSec, fatlist);
        uint32_t root = getFirstSectorOfCluster(2);
        shared_ptr<char> root_sec = new char[512];
        //        LOGHEX(root)
        dev_node->bread(root, 1, root_sec);

        fat32dir* f32d = (fat32dir*) root_sec.get();

        for (int i = 0; i < 10; i++)
            if ((f32d[i].DIR_Attr & ATTR_LONG_NAME) != ATTR_LONG_NAME)
                printf("\nName:[%s]", f32d[i].DIR_Name);

        fs_root_directory = new fat_vnode(this, dev_node, 2, bfat->BPB_BytsPerSec, bfat->BPB_SecPerClus);
        return false;
    }

    int read(int position, int size, shared_ptr<char> _data)
    {
        //printf("\ndev_node is:%x\n", dev_node);
        printf("\nreading from block drive [%s]", dev_node->v_name.c_str());
        dev_node->bread(position, size, _data);
    }

    uint32_t getFirstSectorOfCluster(uint32_t N)
    {
        return ((N - 2) * bfat->BPB_SecPerClus) +FirstDataSector;
    }
};

void fat_init()
{
    printf("\nInitializing fat");
    fat *fs = new fat();
    register_filesystem(fs);
}

int fat_vnode::bread(int position, int size, shared_ptr<char> data)
{
    DBG_OUTPUT
    parent->read(position, size, data);
}

int fat_vnode::lookup(const char* const path)
{
    //LOGHEX(path)
    //LOGSTR(path)
    size_t name_len = 0;
    for (name_len = 0; (path[name_len] != 0) && (path[name_len] != '/'); name_len++);
    //LOGHEX(name_len)
    string file(path, name_len);
    LOGSTR(file.c_str())
    shared_ptr<char> sectorData = new char[sectorSize];

    size_t current_cluster = start_cluster;
    size_t current_sector = parent->getFirstSectorOfCluster(current_cluster);
    parent->read(current_sector, 1, sectorData);
    fat32dir *dirArr = (fat32dir*) sectorData.get();

    DBG_OUTPUT

    for (int i = 0; i < 16; i++)
    {
        string filename;
        if ((dirArr[i].DIR_Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
        {
            printf("\nCannot print filename. Not implemented");
            continue;
        }
        else
        {
            filename=convertFat32String(dirArr[i].DIR_Name);
            LOGSTR(filename.c_str());
        }
        if (filename == file)
        {
            
        }

    }
    return 1;
}