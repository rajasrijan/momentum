#include <termio.h>
#include <errno.h>
int tcgetattr(int fd, struct termio *termios_p){
    errno = ENOSYS;
    return -1;
}