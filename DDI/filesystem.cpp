#include "filesystem.h"
#include <vector>

filesystem::filesystem()
{
}

filesystem::filesystem(const filesystem& orig)
{
}

filesystem::~filesystem()
{
}

std::string filesystem::getName()
{
    return name;
}

std::vector<filesystem*> fs_list;

void register_filesystem(filesystem* fs)
{
    fs_list.push_back(fs);
}

void unregister_filesystem(filesystem* fs)
{
    //fs_list.remove(fs);
}
