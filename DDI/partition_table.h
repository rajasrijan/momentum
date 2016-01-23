#ifndef PARTITION_TABLE_H
#define PARTITION_TABLE_H

#include <stdint.h>

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
    Partition partition[4];
    uint16_t boot_signature; //0x55AA
} __attribute__((packed));

#endif /* PARTITION_TABLE_H */

