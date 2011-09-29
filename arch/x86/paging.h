/* 
 * File:   paging.h
 * Author: srijan
 *
 * Created on 27 April, 2011, 12:06 PM
 */

#ifndef PAGING_H
#define	PAGING_H
#include <stdint.h>

struct paging_structure
{
    uint32_t page_directory[1024];
    uint32_t page_table[1024 * 1024];
};

void map_4mb(uint32_t virtual_address);
typedef struct paging_structure paging_structure_t;
void new_paging_structure(paging_structure_t* ps);
void init_paging(void);
void identity_map_4mb(uint32_t address);
void force_map(uint32_t physical, uint32_t virtual, uint32_t pages);

#endif	/* PAGING_H */

