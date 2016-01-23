#include "driver.h"
#include "ata.h"
#include "fatgen.h"

void init_drivers()
{
    ata_init();
    fat_init();
}
