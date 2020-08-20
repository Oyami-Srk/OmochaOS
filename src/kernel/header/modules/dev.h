//
// Created by Shiroko on 2020/7/23.
//

#ifndef __MOD_DEV__
#define __MOD_DEV__

#include <generic/typedefs.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <modules/systask.h>

struct DeviceImpl_Register_Struct {
    char DeviceName[32];
    char TaskName[16];
    uint ID_Read_func;
    uint ID_Write_func;
    uint ID_Open_func;
    uint ID_Close_func;
    uint ID_Info_func;
    BOOL Has_Permission_To_Write_Directly;
};

#define DEV_READ  1
#define DEV_WRITE 2
#define DEV_OPEN  3
#define DEV_CLOSE 4
#define DEV_INFO  5
#define DEV_LIST  6
#define DEV_REG   10
#define DEV_UNREG 11
#define DEV_UPD   12

#define DEV_STATUS_OK        0
#define DEV_STATUS_WAIT      1
#define DEV_STATUS_ERROR     2
#define DEV_STATUS_BUF_SMALL 3

static int DEV_Register(const char *DeviceName, const char *TaskName,
                        uint Read_func, uint Write_func, uint Open_func,
                        uint Close_func, uint Info_func,
                        BOOL Has_permission_to_write_directly) {
    size_t lenDevicename = strlen(DeviceName);
    size_t lenTaskname   = strlen(TaskName);
    assert(lenDevicename <= 32);
    assert(lenTaskname <= 16);
    struct DeviceImpl_Register_Struct pdrs = {
        .ID_Read_func                     = Read_func,
        .ID_Write_func                    = Write_func,
        .ID_Open_func                     = Open_func,
        .ID_Close_func                    = Close_func,
        .ID_Info_func                     = Info_func,
        .Has_Permission_To_Write_Directly = Has_permission_to_write_directly};
    memset(pdrs.DeviceName, 0, 32 + 16);
    memcpy(pdrs.DeviceName, DeviceName, lenDevicename);
    memcpy(pdrs.TaskName, TaskName, lenTaskname);

    uint    task_dev = query_proc("TaskDEV");
    message msg      = {.receiver = task_dev, .type = DEV_REG, .major = &pdrs};
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major; // device id
}

static int DEV_Unregister(int device_id) {
    uint    task_dev = query_proc("TaskDEV");
    message msg = {.receiver = task_dev, .type = DEV_REG, .major = device_id};
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

// return status
static int DEV_Read(int device_id, char *buf, uint address, size_t size) {
    uint    task_dev = query_proc("TaskDEV");
    message msg      = {.receiver         = task_dev,
                   .type             = DEV_READ,
                   .major            = device_id,
                   .data.uint_arr.d1 = (uint)buf,
                   .data.uint_arr.d2 = address,
                   .data.uint_arr.d3 = size,
                   .data.uint_arr.d4 = 0

    };
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

static int DEV_Write(int device_id, char *buf, uint address, size_t size) {
    uint    task_dev = query_proc("TaskDEV");
    message msg      = {.receiver         = task_dev,
                   .type             = DEV_WRITE,
                   .major            = device_id,
                   .data.uint_arr.d1 = (uint)buf,
                   .data.uint_arr.d2 = address,
                   .data.uint_arr.d3 = size,
                   .data.uint_arr.d4 = 0

    };
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

static int DEV_Open(int device_id) {
    uint    task_dev = query_proc("TaskDEV");
    message msg      = {
        .receiver = task_dev,
        .type     = DEV_OPEN,
        .major    = device_id,
    };
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

static int DEV_Close(int device_id) {
    uint    task_dev = query_proc("TaskDEV");
    message msg      = {
        .receiver = task_dev,
        .type     = DEV_CLOSE,
        .major    = device_id,
    };
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

static int DEV_Info(int device_id, char *buf, size_t size) {
    uint    task_dev = query_proc("TaskDEV");
    message msg      = {.receiver         = task_dev,
                   .type             = DEV_INFO,
                   .major            = device_id,
                   .data.uint_arr.d1 = (uint)buf,
                   .data.uint_arr.d3 = size};
    send_msg(&msg);
    recv_msg(&msg, task_dev);
    return msg.major;
}

#endif // __MOD_DEV__
