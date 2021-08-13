#ifndef HEAP_H
#define HEAP_H

#define HEAP_EMPTY (1 << 0)
#define HEAP_FULL (1 << 1)
#define HEAP_DEFAULT_SIZE (256 * 1024 * 1024)

typedef struct _heap_
{
    uint64_t size;
    struct _heap_ *next;
    uint64_t flags:56;
    uint8_t checksum;
} heap_t;

#ifdef __cplusplus
extern "C"
{
#endif

    extern heap_t *pHeap;

#ifdef __cplusplus
};
#endif

#endif // HEAP_H