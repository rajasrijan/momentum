#include <stddef.h>
#include <unistd.h>
#include <kernel/syscall.h>

#if __STDC_HOSTED__ == 1
int chdir(const char *path)
{
    int ret = 0;
    __asm__ volatile("syscall" : "=a"(ret) : "D"(SYSCALL_CHDIR), "S"(path), "d"(0) : "rcx", "r11");
    return ret;
}
char *getcwd(char *buf, size_t size)
{
    char *ret = 0;
    __asm__ volatile("syscall" : "=a"(ret) : "D"(SYSCALL_GETCWD), "S"(buf), "d"(size) : "rcx", "r11");
    return ret;
}
#endif