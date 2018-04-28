/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

/*Linked list*/
typedef struct _llnode
{
	void *ptr;
	void *next;
} llnode;

typedef struct linked_list
{
    uint32_t size;
    llnode* first_node;
    llnode* last_node;
} linked_list_t;

void create_linked_list(linked_list_t** ll,uint32_t size);
void addto_linked_list(linked_list_t* ll, void* ptr);
void removefrm_linked_list(linked_list_t *ll,void *ptr);
void* getelement_linked_list(linked_list_t *ll,uint32_t index);
/*	Vector*/

#ifdef	__cplusplus
}
#endif

#endif	/* LISTS_H */

