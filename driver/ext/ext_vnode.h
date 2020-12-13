#ifndef EXT_VNODE_H
#define EXT_VNODE_H

#include "ext_vfs.h"
#include <kernel/vfs.h>
#include <kernel/vnode.h>

class ext_vnode : public vnode
{
  private:
    uint32_t inode_id;
    inode *_inode;

  public:
    ext_vnode(ext_vfs *vfsp, size_t _inode_id);
    ~ext_vnode();
    int mkdir(std::string name, std::shared_ptr<vnode> &pDir);
    int bread(ssize_t position, size_t size, char *data, int *bytesRead);
    int bwrite(ssize_t position, size_t size, char *data, int *bytesWritten);
    int readdir(vector<shared_ptr<vnode>> &vnodes);
    int open(uint64_t flags);
    int lookup(char const *, shared_ptr<vnode> &);
    int read_block(uint32_t block_index, shared_ptr<char> &buffer);
    int write_block(uint32_t block_index, shared_ptr<char> &buffer);
    int flush_inode(uint32_t inode_id);
};

#endif //   EXT_VNODE_H