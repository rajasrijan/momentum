#ifndef BITMAP_H
#define BITMAP_H
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/logging.h>
class bitmap
{
  private:
    size_t bit_count;
    uint64_t *dat;

  public:
    bitmap(size_t count)
        : bit_count(count), dat(nullptr)
    {
        dat = new uint64_t[(bit_count + 63) / 64];
    }
    ~bitmap()
    {
        delete dat;
    }
    bool isBitSet(size_t index)
    {
        if (index >= bit_count) {
            log_error("index out of range\n");
            asm("cli;hlt");
        }
        return dat[index % 64] & (1 << (index % 64));
    }
    int alloc_bit(size_t &index)
    {
        bool found = false;
        for (size_t i = 0; i < bit_count / 64; i++) {
            if (dat[i] == 0xFFFFFFFFFFFFFFFF)
                continue;
            int pos = __builtin_ctz(~dat[i]);
            index = (64 * i) + pos;
            found = true;
            break;
        }
        if (found && index < bit_count) {
            dat[index / 64] |= (1 << (index % 64));
            return 0;
        }
        index = 0;
        return -ENOMEM;
    }
    uint8_t *get_ptr()
    {
        return (uint8_t *)dat;
    }
};

#endif //BITMAP_H