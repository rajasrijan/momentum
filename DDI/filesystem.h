#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>
#include "../kernel/vfs.h"
#include "../kernel/vfsops.h"

class filesystem
{
public:
    filesystem();
    filesystem(const filesystem& orig);
    virtual ~filesystem();
    std::string getName();
    virtual bool mount(uint32_t flags, std::string dev_path, vnode*& fs_root_directory) = 0;
    //virtual bool fill_vfs(vfs* v);
protected:
    std::string name;
};

extern std::vector<filesystem*> fs_list;

void register_filesystem(filesystem* fs);
void unregister_filesystem(filesystem* fs);
#endif /* FILESYSTEM_H */

