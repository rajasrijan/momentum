#ifndef __ACMOMENTUM_H__
#define __ACMOMENTUM_H__

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <native_sync.h>

#define ACPI_MACHINE_WIDTH 64

#define ACPI_DEBUG_OUTPUT

#define ACPI_MUTEX_TYPE ACPI_OSL_MUTEX

#define ACPI_USE_SYSTEM_CLIBRARY

#define ACPI_MUTEX mtx_t *
#define ACPI_SPINLOCK mtx_t *
#define ACPI_SEMAPHORE sem_t * 

#define ACPI_USE_LOCAL_CACHE

#endif /* __ACMOMENTUM_H__ */
