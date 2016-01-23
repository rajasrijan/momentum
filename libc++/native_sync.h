/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   native_sync.h
 * Author: Srijan
 *
 * Created on 23 October, 2015, 12:15 PM
 */

#ifndef NATIVE_SYNC_H
#define NATIVE_SYNC_H

#include <stdint.h>

enum
{
    thrd_success,
    thrd_nomem,
    thrd_timedout,
    thrd_busy,
    thrd_error
};

typedef uint32_t mtx_t;
extern int mtx_init(mtx_t* mutex, int type);
extern int mtx_lock(mtx_t* mutex);
extern int mtx_trylock(mtx_t *mutex);
extern int mtx_unlock(mtx_t *mutex);
extern void mtx_destroy(mtx_t *mutex);

/*Syncrosization helper class. Acquires while constructed, releases while destroyed.*/
class sync
{
    mtx_t* lock;
public:

    sync(mtx_t* lock_ptr) : lock(lock_ptr)
    {
        mtx_lock(lock);
    }

    ~sync()
    {
        mtx_unlock(lock);
    }
};

#endif /* NATIVE_SYNC_H */

