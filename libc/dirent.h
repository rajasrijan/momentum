#ifndef DIRENT_H
#define DIRENT_H
#include <limits.h>
struct dirent
{
    char d_name[NAME_MAX];
    unsigned char d_namlen;
};

typedef struct _dir
{
    char d_name[NAME_MAX];
} DIR;
#endif //   DIRENT_H