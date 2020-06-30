#pragma once

extern "C"
{
    //  atomically increment the value and return the old value.
    extern int64_t atomic_increment(int64_t *val);
}