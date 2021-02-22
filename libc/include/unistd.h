#ifndef _UNISTD_H_
#define _UNISTD_H_

#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#ifndef _POSIX_VERSION
#define _POSIX_VERSION 200809L
#endif //_POSIX_VERSION

#ifndef FILE
#define FILE void
#endif //FILE

int chdir(const char *path);
char *getcwd(char *buf, size_t size);
int close(int fd);
#endif