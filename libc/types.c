#include <sys/types.h>

dev_t makedev(uint32_t maj, uint32_t min)
{
    return (uint64_t)min | ((uint64_t)maj << 32);
}
uint32_t major(dev_t dev)
{
    return (uint32_t)((dev >> 32) & 0xFFFFFFFF);
}
uint32_t minor(dev_t dev)
{
    return (uint32_t)(dev & 0xFFFFFFFF);
}