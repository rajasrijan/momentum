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

#ifndef VFS_H
#define VFS_H

#include <stdint.h>

struct information_node;
struct open_file;
struct file_metadata;

/*
 * file_metadata members.
 */

#define FILE_TYPE_FILE          (1<<0)
#define FILE_TYPE_OPENFILE      (1<<1)
#define FILE_TYPE_DIRECTORY     (1<<2)
#define FILE_TYPE_SYMLINK       (1<<3)


/*
 * Function types for inode structure.
 */

typedef struct open_file* (*fileCreateFn)(char*);
typedef struct file_metadata* (*fileStatusFn)(char*);
typedef uint32_t(*fileAccessFn)(char*, uint32_t);
typedef uint32_t(*fileChangeModeFn)(char*, uint32_t);
typedef uint32_t(*fileChangeOwnerFn)(char*, uint32_t);

/*
 * Function types for ofile structure.
 */

/*
 * returns number  of bytes read.
 */
typedef uint32_t(*fileReadFn)(struct open_file*, /*Pointer to open file.*/
        uint32_t, /* numger of bytes to read */
        uint32_t, /* size of buffer in bytes*/
        void* /* pointer to buffer */
        );
/*
 * returns number  of bytes written.
 */
typedef uint32_t(*fileWriteFn)(struct open_file*, /*Pointer to open file.*/
        uint32_t, /* numger of bytes to write */
        uint32_t, /* size of buffer in bytes*/
        void* /* pointer to buffer */
        );

/*
 * returns new position.
 */
typedef uint32_t(*fileSeekFn)(struct open_file*, /*Pointer to open file.*/
        uint32_t, /* numger of bytes to seek */
        uint32_t /* seek flags */
        );

/*
 * returns new success/failure.
 */
typedef uint32_t(*fileCloseFn)(struct open_file* /*Pointer to open file.*/
        );

/*
 * returns new status.
 */
typedef uint32_t(*fileLockFn)(struct open_file*, /*Pointer to open file.*/
        uint32_t/* lock flags. */
        );

/*
 * returns new Controles.
 */
typedef uint32_t(*fileControleFn)(struct open_file*, /*Pointer to open file.*/
        uint32_t/* Controle flags. */
        );

/*
 * returns Status.
 */
typedef uint32_t(*fileOpenStatusFn)(struct open_file* /*Pointer to open file.*/
        );

/*
 * returns Status.
 */
typedef uint32_t(*filePipeFn)(struct open_file*, /*Pointer to open file.*/
        char* name /* Name of the pipe. */
        );

typedef struct super_node
{
} snode;

typedef struct file_metadata
{
    char Name[40];
    uint32_t Owner;
    uint32_t Protection;
    uint32_t type;
} fmetadata;

typedef struct information_node
{
    fmetadata metaData;

    fileCreateFn createFile;
    fileStatusFn getStatus;
    fileAccessFn getAccess;
    fileChangeModeFn changeMode;
    fileChangeOwnerFn changeOwner;
} inode;

typedef struct open_file
{
    fmetadata metaData;
    inode* fileNode;

    fileReadFn read;
    fileWriteFn write;
    fileSeekFn seek;
    fileCloseFn close;
    fileLockFn fileLock;
    fileControleFn fileControle;
    fileOpenStatusFn fileStatus;
    filePipeFn pipe;
} ofile;

typedef struct directory_entry
{
} dentry;

/*
 * vfs root.
 */

inode* vfsRoot;

void init_vfs(void);

#endif
