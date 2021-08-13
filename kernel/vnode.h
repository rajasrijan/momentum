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

#ifndef NODE_H
#define NODE_H
#include <stdint.h>
#include <string>
#include <memory>

enum vtype
{
    VNON = 0,
    VREG = 1,
    VDIR = 2,
    VBLK = 3,
    VCHR = 4,
    VLNK = 5,
    VSOCK = 6,
    VBAD = 7
};

class vnode
{
  public:
    uint16_t v_shlockc;          /* # of shared locks */
    uint16_t v_exlockc;          /* # of exclusive locks */
    uint32_t v_type;             /* vnode type */
    class vfs *v_vfsmountedhere; /* covering vfs */
    class vfs *v_vfsp;           /* vfs we are in */
                                 // vnode *v_root;

  private:
    [[maybe_unused]] uint16_t v_flag; /* vnode flags */
    std::string v_name;
    [[maybe_unused]] uint64_t v_count; /* reference count */
    std::vector<std::shared_ptr<vnode>> special_nodes;
    std::vector<std::shared_ptr<vnode>> dnode_cache;
    bool runOnce = false;
    std::vector<std::shared_ptr<vnode>> ref_nodes;

  protected:
    void setName(const std::string &name)
    {
        setName(name.c_str());
    }
    void setName(const char *name);

  public:
    vnode(class vfs *vfsp);
    vnode(const vnode &) = delete;
    constexpr vnode &operator=(const vnode &) = delete;
    virtual ~vnode();
    virtual int access(void);
    virtual int bmap(void);
    virtual int bread(ssize_t position, size_t size, char *data, int *bytesRead);
    virtual int bwrite(ssize_t position, size_t size, const char *data, int *bytesWritten);
    virtual int brelse(void);
    virtual int close(void);
    int docreate(const std::string &path, std::shared_ptr<vnode> &created_node);
    int dolookup(const char *const path, std::shared_ptr<vnode> &foundNode);
    int doreaddir(std::vector<std::shared_ptr<vnode>> &vnodes);
    int doopen(std::shared_ptr<vnode> &node, uint32_t flags, class vfile **file);
    virtual int fsync(void);
    const std::string &getName() const;
    virtual int getattr(void);
    virtual int ioctl(uint32_t command, void *data, int fflag);
    virtual int inactive(void);
    virtual int link(void);
    virtual int mkdir(std::string name, std::shared_ptr<vnode> &pDir);
    int mknod(std::shared_ptr<vnode> &current_node, std::shared_ptr<vnode> &pNode);
    int rmnod(std::shared_ptr<vnode> &current_node, std::shared_ptr<vnode> &pNode);
    virtual int rdwr(void);
    virtual int rename(std::string name);
    virtual int rmdir(void);
    virtual int readlink(void);
    virtual int remove(void);
    virtual int select(void);
    virtual int setattr(void);
    virtual int symlink(void);
    virtual int strategy(void);
    virtual int getSize(size_t &sz);
    virtual bool isPartitionable()
    {
        return true;
    }
    void addRef(std::shared_ptr<vnode> &node)
    {
        ref_nodes.push_back(node);
    }
    int removeRef(std::shared_ptr<vnode> &node)
    {
        auto result = find(ref_nodes.begin(), ref_nodes.end(), node);
        if (result == ref_nodes.end())
        {
            return ENOENT;
        }
        ref_nodes.erase(result);
        return 0;
    }
    bool isDirectory()
    {
        return (v_type == VDIR);
    }
    bool isCharacterDevice()
    {
        return (v_type == VCHR);
    }
    bool isFile()
    {
        return (v_type == VREG);
    }
    bool isBlockDevice()
    {
        return (v_type == VBLK);
    }
    bool isMountPoint()
    {
        return v_vfsmountedhere != nullptr;
    }

  protected:
    virtual int lookup(const char *const path, std::shared_ptr<vnode> &foundNode);
    virtual int readdir(std::vector<std::shared_ptr<vnode>> &vnodes);
    virtual int open(uint64_t flags);
    virtual int create(const std::string &path, std::shared_ptr<vnode> &created_node);
};

#endif // NODE_H