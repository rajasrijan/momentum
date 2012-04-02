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

#include "lists.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"

void vector_push_fn(struct _vector_list* list, void* data);
void* vector_at_fn(struct _vector_list* list, uint32_t index);
uint32_t vector_size_fn(struct _vector_list* list);

void vector_init_fn(struct _vector_list* list, uint32_t data_size)
{
    list->data_size = data_size;
    list->buffer_size = 4;
    list->current_buffer_ptr = 0;
    list->data = calloc(4, data_size);
    list->push = &vector_push_fn;
    list->at = &vector_at_fn;
    list->size = vector_size_fn;
}

void vector_push_fn(struct _vector_list* list, void* data)
{
    if (list->current_buffer_ptr >= list->buffer_size)
    {
        list->buffer_size += 4;
        void* tmp = calloc(list->buffer_size, list->data_size);
        memcpy(tmp, list->data, list->current_buffer_ptr * list->data_size);
        free(list->data);
        list->data = tmp;
    }
    void* dest = (void*) ((char*) list->data + (list->data_size * list->current_buffer_ptr));
    memcpy(dest, data, list->data_size);
    list->current_buffer_ptr++;
}

void* vector_at_fn(struct _vector_list* list, uint32_t index)
{
    if (index >= list->current_buffer_ptr)
        return (void*) 0;
    return (void*) ((uint32_t) list->data + (list->data_size * index));
}

uint32_t vector_size_fn(struct _vector_list* list)
{
    return list->current_buffer_ptr;
}
