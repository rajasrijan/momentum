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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#define LOGVAR(x) printf("\""#x"\": %X\n",x);
#define LOGSTR(x) printf("\""#x"\": %s\n",x);

uint32_t FirstDataSector = 0;
uint32_t* fatlist;

struct bpb
{
    uint8_t BS_jmpBoot[3];
    uint8_t BS_OEMName[8];
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
} __attribute__((packed)) boot;

struct fat32
{
    uint32_t FATSz32;
    uint16_t ExtFlags;
    uint16_t FSVer;
    uint32_t RootClus;
    uint16_t FSInfo;
    uint16_t BkBootSec;
    uint8_t Reserved[12];
    uint8_t DrvNum;
    uint8_t Reserved1;
    uint8_t BootSig;
    uint32_t VolID;
    uint8_t VolLab[11];
    uint8_t FilSysType[8];
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
#define ATTR_LONG_NAME  ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID
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

uint32_t copyFile(FILE* disk, uint32_t cluster, FILE* src);

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("Not enough arguments");
        exit(-1);
    }
    int ret = 0;
    char *file_name = argv[1];
    LOGSTR(argv[0]);
    LOGSTR(argv[1]);
    LOGSTR(argv[2]);
    LOGSTR(argv[3]);
    uint32_t file_offset = atoi(argv[2]);
    FILE *fat32_file = fopen(file_name, "rb+");
    ret = fseek(fat32_file, -1, SEEK_END);
    uint32_t file_size = ftell(fat32_file);
    LOGVAR(fat32_file);
    LOGVAR(file_offset);
    LOGVAR(file_size);
    ret = fseek(fat32_file, 0, SEEK_SET);
    LOGVAR(ret);
    //struct bpb boot;
    uint32_t objects_read = fread(&boot, sizeof (struct bpb), 1, fat32_file);
    LOGSTR(boot.BS_OEMName);
    LOGVAR(objects_read);
    struct fat32 fat;
    objects_read = fread(&fat, sizeof (struct fat32), 1, fat32_file);
    LOGVAR(objects_read)
    LOGVAR(fat.RootClus)
    uint32_t RootDirSectors = ((boot.BPB_RootEntCnt * 32)
            + (boot.BPB_BytsPerSec - 1)) / boot.BPB_BytsPerSec;
    LOGVAR(RootDirSectors)
    LOGVAR(boot.BPB_RsvdSecCnt)
    LOGVAR(boot.BPB_NumFATs)
    LOGVAR(fat.FATSz32)
    FirstDataSector = boot.BPB_RsvdSecCnt + (boot.BPB_NumFATs * fat.FATSz32)
            + RootDirSectors;
    LOGVAR(FirstDataSector)
    LOGVAR(boot.BPB_TotSec32)
    uint32_t DataSec = boot.BPB_TotSec32 - FirstDataSector;
    LOGVAR(DataSec);
    LOGVAR(boot.BPB_SecPerClus)
    uint32_t CountofClusters = DataSec / boot.BPB_SecPerClus;
    LOGVAR(CountofClusters)

    fatlist = malloc(CountofClusters * sizeof (uint32_t));
    ret = fseek(fat32_file, boot.BPB_RsvdSecCnt * boot.BPB_BytsPerSec,
                SEEK_SET);
    LOGVAR(ret)
    LOGVAR(ftell(fat32_file));
    ret = fread(fatlist, sizeof (uint32_t), CountofClusters, fat32_file);
    LOGVAR(ret)
    LOGVAR(fatlist[0])
    LOGVAR(fatlist[1])
    LOGVAR(fatlist[2])
    fseek(fat32_file, FirstDataSector * boot.BPB_BytsPerSec, SEEK_SET);
    char *sector = malloc(boot.BPB_BytsPerSec * boot.BPB_SecPerClus);
    ret = fread(sector, boot.BPB_BytsPerSec, boot.BPB_SecPerClus, fat32_file);
    LOGVAR(ret)
            struct fat32dir *f32d = (void*) sector;
    for (int i = 0; i < strlen(argv[3]); i++)
        argv[3][i] = toupper(argv[3][i]);
    LOGSTR(argv[3])
    for (int i = 0; i < 15; i++)
    {
        char filename[11] = {
            0
        };
        for (int j = 0, index = 0; j < 11; j++, index++)
            if (f32d[i].DIR_Name[j] == ' ')
            {
                filename[index] = '.';
                j = 7;
            }
            else
                filename[index] = f32d[i].DIR_Name[j];

        LOGSTR(f32d[i].DIR_Name)
        LOGVAR(f32d[i].DIR_FstClusHI)
        LOGVAR(f32d[i].DIR_FstClusLO)
        LOGSTR(filename)
        if (strcmp(filename, argv[3]) == 0)
        {
            printf("File \"%s\" found.\n", filename);
            FILE *src = fopen(argv[3], "rb");
            uint32_t sz_new = copyFile(fat32_file,
                                       (f32d[i].DIR_FstClusHI << 16) | (f32d[i].DIR_FstClusLO),
                                       src);
            fclose(src);
            f32d[i].DIR_FileSize = sz_new;
            fseek(fat32_file, FirstDataSector * boot.BPB_BytsPerSec, SEEK_SET);
            ret = fwrite(sector, boot.BPB_BytsPerSec, boot.BPB_SecPerClus, fat32_file);
            break;
        }
    }

    free(fatlist);
    fclose(fat32_file);
    return 0;
}

uint32_t getSectorFromCluster(uint32_t N)
{
    uint32_t FirstSectorofCluster = ((N - 2) * boot.BPB_SecPerClus)
            + FirstDataSector;
    return FirstSectorofCluster;
}

uint32_t copyFile(FILE* disk, uint32_t cluster, FILE* src)
{
    char *buffer = malloc(boot.BPB_BytsPerSec);
    uint32_t location = boot.BPB_BytsPerSec * getSectorFromCluster(cluster);
    LOGVAR(location);
    fseek(disk, location, SEEK_SET);
    uint32_t next_cluster = cluster;
    uint32_t size = 0;
    do
    {
        LOGVAR(next_cluster);
        fflush(stdout);
        for (int i = 0; i < boot.BPB_SecPerClus; i++)
        {
            int offset = fread(buffer, 1, boot.BPB_BytsPerSec, src);
            size += offset;
            LOGVAR(offset);
            if (offset == 0)
                break;
            offset = fwrite(buffer, 1, offset, disk);
            LOGVAR(offset);
        }
        next_cluster = fatlist[next_cluster];
    }
    while (next_cluster < 0x0FFFFFF8);
    free(buffer);
    return size;
}
