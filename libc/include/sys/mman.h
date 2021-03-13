/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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

//	Page can be read.
#define PROT_READ (1 << 0)

//	Page can be written.
#define PROT_WRITE (1 << 0)

//	Page can be executed.
#define PROT_EXEC (1 << 0)

//	Page can not be accessed.
#define PROT_NONE (1 << 0)

//	Share changes.
#define MAP_SHARED (1 << 0)

//	Changes are private.
#define MAP_PRIVATE (1 << 0)

//	Interpret addr exactly.
#define MAP_FIXED (1 << 0)

//	Perform asynchronous writes.
#define MS_ASYNC (1 << 0)

//	Perform synchronous writes.
#define MS_SYNC (1 << 0)

//	Invalidate mappings.
#define MS_INVALIDATE (1 << 0)

//	Lock currently mapped pages.
#define MCL_CURRENT (1 << 0)

//	Lock pages that become mapped.
#define MCL_FUTURE (1 << 0)

#define MAP_ANON (1 << 0)
#define MAP_ANONYMOUS MAP_ANON
#define MAP_FAILED ((void *)-1)

int mlock(const void *, size_t);
int mlockall(int);
void *mmap(void *, size_t, int, int, int, off_t);
int mprotect(void *, size_t, int);
int msync(void *, size_t, int);
int munlock(const void *, size_t);
int munlockall(void);
int munmap(void *, size_t);
int shm_open(const char *, int, mode_t);
int shm_unlink(const char *);
