#ifndef RAMDRIVE_H
#define	RAMDRIVE_H

#include <stdint.h>

int ramdrive_init(void);
void ramdrive_cleanup(void);
void ramdrive_write(uint32_t sector_off, uint8_t *buffer, unsigned int sectors);
void ramdrive_read(uint32_t sector_off, uint8_t *buffer, unsigned int sectors);

#endif	/* RAMDRIVE_H */
