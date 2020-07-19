#ifndef _UNISTD_H_
#define _UNISTD_H_
int chdir(const char *path);
char *getcwd(char *buf, size_t size);
int close(int fd);
#endif