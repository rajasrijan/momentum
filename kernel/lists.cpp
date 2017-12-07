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

#include "lists.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "../arch/x86_64/timer.h"

int vector_push_fn(struct _vector_list* list, void* data);
void* vector_at_fn(const struct _vector_list* list, uint32_t index);
uint32_t vector_size_fn(const struct _vector_list* list);

void create_linked_list(linked_list_t** ll, uint32_t size)
{
	ll[0] = (linked_list_t*)calloc(1, sizeof(linked_list_t));
	ll[0]->size = size;
	ll[0]->first_node = 0;
	ll[0]->last_node = 0;
}

void addto_linked_list(linked_list_t* ll, void* ptr)
{
	llnode* node = (llnode*)calloc(1, sizeof(llnode));
	if (ll->last_node != 0)
		ll->last_node->next = node;
	else
		ll->first_node = node;
	ll->last_node = node;

	node->ptr = calloc(1, ll->size);
	memcpy((char*)node->ptr, (char*)ptr, ll->size);
}

void removefrm_linked_list(linked_list_t *ll, void *ptr)
{
	for (llnode *i = ll->first_node, *prev_node = 0; i != NULL; prev_node = i, i = (llnode*)i->next)
	{
		if (i->ptr == ptr)
		{
			//printf("\nFound *ptr.");

			if (i == ll->last_node)
				ll->last_node = prev_node;
			if (i == ll->first_node)
				ll->first_node = (llnode*)i->next;

			if (prev_node)
				prev_node->next = i->next;
			free(i->ptr);
			free(i);
		}
	}
	/*setColor(0x14);
	printf("\nUnimplimented function \"%s\".Halting...", __FUNCTION__);
	__asm__("cli;hlt;");*/
}

void* getelement_linked_list(linked_list_t *ll, uint32_t index)
{
	for (llnode *i = ll->first_node; i != ll->last_node; i = (llnode*)i->next)
	{
		if (index-- == 0)
			return i->ptr;
	}
	return ll->last_node;
}