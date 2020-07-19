#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <kernel/syscall.h>

#if __STDC_HOSTED__ == 1
int getdents64(int fd, struct dirent *dirp, size_t count)
{
    struct getdents_args args = {fd, dirp, count};
    int ret = _syscall(SYSCALL_GETDENTS, &args, 0);
    return ret;
}

int openat(int dirfd, const char *pathname, int flags)
{
    struct openat_args args = {dirfd, pathname, flags};
    int ret = _syscall(SYSCALL_OPENAT, &args, 0);
    return ret;
}
#endif