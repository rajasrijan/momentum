/*
 * Copyright 2009-2012 Srijan Kumar Sharma
 * 
 * This file is part of Momentum.
 * 
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LISTS_H
#define	LISTS_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct linked_list
{
    void* pointer;
    struct linked_list *next, *prev;
} linked_list_t;

struct _vector_list;

void vector_init_fn(struct _vector_list* list, uint32_t data_size);
typedef int (vector_push) (struct _vector_list* list, void* data);
typedef void* (vector_at) (const struct _vector_list* list, uint32_t index);
typedef uint32_t(vector_size)(const struct _vector_list* list);

typedef struct _vector_list
{
    void* data;
    uint32_t data_size;
    uint32_t buffer_size;
    uint32_t current_buffer_ptr;
    vector_push *push;
    vector_at *at;
    vector_size *size;
} vector_list_t;
#ifdef	__cplusplus
}
#endif

#endif	/* LISTS_H */

