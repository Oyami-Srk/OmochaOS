#ifndef __MOD_VFS_VFS_INC__
#define __MOD_VFS_VFS_INC__

#include <generic/typedefs.h>

struct vfs_inode {
    unsigned long long file_size;
    uint               attribute;
    uint               blocks;

    struct vfs_superblock *sb;

    struct vfs_inode_operator *inode_ops;
    struct vfs_file_operator * file_ops;

    void *priv_data;
};

struct vfs_dir {
    char * name;
    size_t name_length;

    struct vfs_inode *dir_inode;
    struct vfs_dir *  parent;

    struct vfs_dir *subdirs;
    struct vfs_dir *sub_next;
    struct vfs_dir *sub_prev;

    struct vfs_dir_operator *ops;
};

struct file {
    long long         position;
    uint              mode;
    struct vfs_dir *  dir;
    struct vfs_inode *inode;

    struct vfs_file_operator *ops;

    void *priv_data;
};

struct vfs_superblock {
    struct vfs_dir *        root;
    struct vfs_sb_operator *ops;
    pid_t                   fs_task;
    void *                  priv_data;
};

struct vfs_sb_operator {
    void (*write_sb)(struct vfs_superblock *sb);
    void (*drop_sb)(struct vfs_superblock *sb);
};

struct vfs_inode_operator {
    int (*create)(struct vfs_inode *inode, struct vfs_dir *dir, int mode);
    int (*rename)(struct vfs_inode *old_inode, struct vfs_dir *old_dir,
                  struct vfs_inode *new_inode, struct vfs_dir *new_dir);
    int (*mkdir)(struct vfs_inode *inode, struct vfs_dir *dir, int mode);
    int (*rmdir)(struct vfs_inode *inode, struct vfs_dir *dir);
    int (*getattr)(struct vfs_dir *dir, unsigned int *attr);
    int (*setattr)(struct vfs_dir *dir, unsigned int *attr);
};

struct vfs_dir_operator {
    int (*compare)(struct vfs_dir *parent, char *source, char *dest);
    int (*drop_inode)(struct vfs_dir *dir, struct vfs_inode *inode);
    int (*drop)(struct vfs_dir *dir);
};

struct vfs_file_operator {
    int (*open)(struct vfs_inode *inode, struct file *pfile);
    int (*close)(struct vfs_inode *inode, struct file *pfile);
    int (*read)(struct file *pfile, char *buf, unsigned long long count,
                long long *position);
    int (*write)(struct file *pfile, char *buf, unsigned long long count,
                 long long *position);
    int (*lseek)(struct file *pfile, long long offset, long long origin);
    int (*ioctl)(struct vfs_inode *inode, struct file *pfile, int cmd, int arg);
};

struct vfs_fs_type {
    char *name;
    int   flags;
    struct vfs_superblock *(*read_sb)(struct vfs_fs_type *fs_type,
                                      pid_t fs_task, int data, void *buf);
    struct vfs_fs_type *next;
};

#endif // __MOD_VFS_VFS_INC__