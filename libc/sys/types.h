#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stddef.h>

typedef uint64_t off_t;
typedef uint64_t mode_t;
typedef uint64_t pid_t;
;
typedef uint64_t dev_t;
#define __pid_t_defined
typedef uint64_t uid_t;
#define __uid_t_defined
typedef uint64_t gid_t;
#define __gid_t_defined
typedef unsigned int u_int;
typedef unsigned long u_long;

#endif //TYPES_H
