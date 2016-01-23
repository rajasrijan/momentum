#include "ramdrive.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

const uint32_t g_DeviceSizeSectors = 1024; //	Device size in Sectors.
const uint32_t g_SectorSize = 512; //	Sector size in bytes.
static uint8_t* g_pRamDriveData = 0; //	Memory where everything is stored.

int ramdrive_init(void)
{
    g_pRamDriveData = (uint8_t*) calloc(g_DeviceSizeSectors, g_SectorSize);
    if (g_pRamDriveData == 0)
        return ENOMEM;
    return 0;
}

void ramdrive_cleanup(void)
{
    free(g_pRamDriveData);
}

void ramdrive_write(uint32_t sector_off, uint8_t *buffer, unsigned int sectors)
{
    memcpy((char*) &g_pRamDriveData[g_SectorSize * sector_off], (char*) buffer, sectors * g_SectorSize);
}

void ramdrive_read(uint32_t sector_off, uint8_t *buffer, unsigned int sectors)
{
    memcpy( buffer, &g_pRamDriveData[g_SectorSize * sector_off], sectors * g_SectorSize);
}
