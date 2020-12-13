#ifndef DEFLATE_H
#define DEFLATE_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define DEFLATE_LAST_BLOCK 0x01

#define DEFLATE_ENCODING_STORED 0x00
#define DEFLATE_ENCODING_STATICHUFF 0x02
#define DEFLATE_ENCODING_COMPRESSED 0x04
#define DEFLATE_ENCODING_RESERVED 0x06

/**
 * @brief Compress the input data using deflate algorithm.
 * 
 * @param inp_data pointer to input data
 * @param in_sz size of input data
 * @param out_data pointer to location where pointer to output data will be returned.
 * @param out_sz size of output data
 * @return int negative error codes
 */
inline int deflate(uint8_t *inp_data, size_t in_sz, uint8_t **out_data, size_t *out_sz)
{
    if (inp_data == NULL || in_sz == 0 || out_data == NULL || out_sz == NULL)
        return -EINVAL;
    uint16_t huffman_tree[288] = {};
    uint8_t *cdata = (uint8_t *)malloc(in_sz);

    free(cdata);
}

inline int inflate(uint8_t *inp_data, size_t in_sz, uint8_t *out_data, size_t out_sz)
{
    if (inp_data == NULL || in_sz == 0 || out_data == NULL || out_sz == 0)
        return -EINVAL;
    uint8_t huffman_tree[288];
    uint8_t udata[32 * 1024];
    for (size_t idx = 0; idx < in_sz; idx += (32 * 1024)) {
        if ((inp_data[idx] & 0x06) != DEFLATE_ENCODING_COMPRESSED) {
            return -ENOSYS;
        }
        memcpy(huffman_tree, &inp_data[idx + 1], sizeof(huffman_tree));
        if (inp_data[idx] & DEFLATE_LAST_BLOCK)
            break;
    }
}
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //DEFLATE_H