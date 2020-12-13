#include <DDI/driver.h>
#include <DDI/filesystem.h>
#include "ext_vfs.h"

vfs *ext_new_vfs() { return new ext_vfs; }
void ext_delete_vfs(vfs *ptr) { delete ptr; }
fileSystem ext = {ext_new_vfs, ext_delete_vfs};
void ext_init()
{
    printf("Initializing ext\n");
    register_filesystem(ext, "ext");
}
MODULT_INIT(ext_init)