#ifndef EXT_VNODE_H
#define EXT_VNODE_H

#include "ext_vfs.h"
#include <kernel/vfs.h>
#include <kernel/vnode.h>

class ext_vnode : public vnode
{
  private:
    class ext_dir_iterator
    {
      private:
        ext_vnode *parent_vnode;
        size_t offset;
        shared_ptr<char> buffer;

      public:
        ext_dir_iterator(ext_vnode *_parent_vnode, size_t _offset);
        ~ext_dir_iterator();
        void operator++();
        bool operator!=(const ext_dir_iterator &rhs);
        void operator=(const ext_dir_iterator &rhs);
        directory &operator*();
    };
    friend ext_dir_iterator;

  private:
    uint32_t inode_id;
    inode *_inode;
    int get_block_id_from_position(size_t pos, size_t &blk_id);

  public:
    ext_vnode(ext_vfs *vfsp, size_t _inode_id);
    ~ext_vnode();
    int mkdir(std::string name, std::shared_ptr<vnode> &pDir);
    int bread(ssize_t position, size_t size, char *data, int *bytesRead);
    int bwrite(ssize_t position, size_t size, char *data, int *bytesWritten);
    int readdir(vector<shared_ptr<vnode>> &vnodes);
    int open(uint64_t flags);
    int lookup(char const *, shared_ptr<vnode> &);
    int create(const string &path, shared_ptr<vnode> &created_node);
    int read_block(uint32_t block_index, shared_ptr<char> &buffer);
    int write_block(uint32_t block_index, shared_ptr<char> &buffer);
    int flush_inode(uint32_t inode_id);
    ext_dir_iterator directory_it_begin();
    ext_dir_iterator directory_it_end();
};

#endif //   EXT_VNODE_H