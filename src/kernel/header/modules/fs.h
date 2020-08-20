#ifndef __MODULE_FS__
#define __MODULE_FS__

#include <modules/systask.h>

#define FS_READ_FILE     1
#define FS_GET_FILE_INFO 2

struct fs_file_info {
    char   filename[8 + 3 + 1];
    size_t size; // in byte
    uint   clus;
};

// offset in bytes
static inline uint FS_read_file(struct fs_file_info *fileinfo, uint offset,
                                void *buf, size_t buf_size) {
    uint    task_fs = query_proc("TaskFS");
    message msg;
    msg.type             = FS_READ_FILE;
    msg.major            = offset;
    msg.data.uint_arr.d1 = (uint)fileinfo;
    msg.data.uint_arr.d2 = (uint)buf;
    msg.data.uint_arr.d3 = (uint)buf_size;
    msg.receiver         = task_fs;
    send_msg(&msg);
    recv_msg(&msg, task_fs);
    return msg.major;
}

static inline uint FS_get_file_info(const char *         filename,
                                    struct fs_file_info *fileinfo) {
    uint    task_fs = query_proc("TaskFS");
    message msg;
    msg.type             = FS_GET_FILE_INFO;
    msg.major            = (uint)filename;
    msg.data.uint_arr.d1 = (uint)fileinfo;
    msg.receiver         = task_fs;
    send_msg(&msg);
    recv_msg(&msg, task_fs);
    return msg.major;
}

#endif // __MODULE_FS__
