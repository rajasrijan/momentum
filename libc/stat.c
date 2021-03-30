#include <sys/stat.h>
#include <errno.h>
#include <kernel/syscall.h>

int stat(const char *path, struct stat *buf)
{
    int ret = 0;
    struct stat_args args = {path, buf};
    ret = _syscall(SYSCALL_STAT, (uint64_t)&args, 0);
    if (ret) {
        errno = -ret;
        return -1;
    }
    return 0;
}