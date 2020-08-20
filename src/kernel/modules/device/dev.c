//
// Created by Shiroko on 2020/7/23.
//
#include <core/interrupt.h>
#include <core/process.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/syscall.h>
#include <modules/dev.h>
#include <modules/memory.h>
#include <modules/systask.h>
#include <modules/tty.h>

/*
 * TaskDEV receive message from both application proc and device implements proc
 * All requests must follow convention below：
 *      - type: request type (function MsgID)
 *      - major: Device ID ( ret for app is Status )
 *      - d1-d4 is not promised for return to application
 *      - d1: data buf in caller addr space
 *      - d2: address
 *      - d3: block count or stream byte size
 *      - d4: pid ( application proc do not need to fill this )
 * For application proc (Such as Filesystem proc):
 *      - Block device:
 *          Read/Write to certain position.
 *          Close/Open device.
 *          Get Information about device. (Contains block size)
 *      - Stream device:
 *          Read/Write to device.
 *          Close/Open device.
 *          Get Information about device.
 * For Device Implements proc:
 *      Provide MsgID for `Read,Write,Close,Open,Info
 *      Only need to call dev proc DEV_REG to register
 *          with major set to address to registering struct
 *      Return a ID that TaskDEV assigned.
 * If Implement proc has no permission to write/read:
 *      For reading, Dev proc send the request to impl proc, impl proc
 *          returns the address in its addr space which contains data.
 *          Dev proc copy data from impl proc to caller proc.
 *      For writing, Dev proc send the request to impl proc, impl proc
 *          returns the address in the size of request.
 *          Dev proc write to this buffer and send request to ask impl
 *          proc to write to device.
 */

#define MAX_DEVICE_COUNT 256

struct DeviceRequest {
    pid_t                 app_pid;
    pid_t                 impl_pid;
    uint                  type;
    struct DeviceRequest *next;
};

struct DeviceImpl {
    char                  DeviceName[32];
    char                  TaskName[16];
    uint                  ID_Read_func;
    uint                  ID_Write_func;
    uint                  ID_Open_func;
    uint                  ID_Close_func;
    uint                  ID_Info_func;
    BOOL                  Has_Permission_To_Write_Directly;
    pid_t                 PID;
    uint                  DeviceID;
    BOOL                  InUse;
    struct DeviceRequest *wait_list;

    struct DeviceImpl *next;
};

struct DeviceImpl *impl_list      = NULL;
struct DeviceImpl *impl_free_list = NULL;
struct DeviceImpl *impl_use_list  = NULL;

struct DeviceRequest *dev_req_wait_list = NULL;

void init_impl_list(const int max_device_count) {
    impl_list = (struct DeviceImpl *)kmalloc(sizeof(struct DeviceImpl) *
                                             max_device_count);
    for (int i = max_device_count; i >= 0; i--) {
        memset(&impl_list[i], 0, sizeof(struct DeviceImpl));
        if (impl_free_list)
            impl_list[i].next = impl_free_list;
        impl_free_list        = &impl_list[i];
        impl_list[i].DeviceID = i;
    }
}

int register_device_impl(
    struct DeviceImpl_Register_Struct *pDeviceImplRegisterStruct, pid_t pid) {
    struct DeviceImpl *impl = impl_free_list;
    impl_free_list          = impl_free_list->next;

    memcpy(impl, pDeviceImplRegisterStruct,
           sizeof(struct DeviceImpl_Register_Struct));

    if (impl_use_list)
        impl->next = impl_use_list;
    impl_use_list = impl;

    impl->next  = NULL;
    impl->InUse = TRUE;
    impl->PID   = pid;
    return impl->DeviceID;
}

int unregister_device_impl(uint DeviceID) {
    struct DeviceImpl *impl = &impl_list[DeviceID];

    struct DeviceImpl *p  = impl_use_list;
    struct DeviceImpl *pp = impl_use_list;
    while (p) {
        if (p == impl) {
            if (p != impl_use_list)
                pp->next = p->next;
            else
                impl_use_list = p->next;
            break;
        }
        pp = p;
        p  = p->next;
    }

    memset(impl, 0, sizeof(struct DeviceImpl));
    impl->DeviceID = DeviceID;
    impl->next     = impl_free_list;
    impl_free_list = impl;
    return 0;
}

_Noreturn void Task_DEV() {
    message msg;
    if (reg_proc("TaskDEV") != 0)
        printf("[HD] Cannot register as TaskDEV\n");
    init_impl_list(MAX_DEVICE_COUNT);
    printf("\n[DEV] Initialized\n");
    while (1) {
        recv_msg(&msg, PROC_ANY);
        int src = msg.sender;
        switch (msg.type) {
        case DEV_REG: {
            struct DeviceImpl_Register_Struct *pDeviceImplRegisterStruct =
                (struct DeviceImpl_Register_Struct *)proc_vir2phy(
                    msg.sender, (char *)msg.major);
            msg.major = register_device_impl(pDeviceImplRegisterStruct, src);
            SEND_BACK(msg);
            break;
        }
        case DEV_UNREG: {
            msg.major = unregister_device_impl(msg.major);
            SEND_BACK(msg);
            break;
        }
        case DEV_WRITE:
        case DEV_OPEN:
        case DEV_CLOSE:
        case DEV_READ: {
            int type      = msg.type;
            int device_id = msg.major;
            if (impl_list[device_id].InUse == FALSE ||
                impl_list[device_id].PID == 0) {
                // log the error
                msg.major = DEV_STATUS_ERROR;
                SEND_BACK(msg);
                break;
            }
            switch (type) {
            case DEV_READ:
                msg.type = impl_list[device_id].ID_Read_func;
                break;
            case DEV_WRITE:
                msg.type = impl_list[device_id].ID_Write_func;
                break;
            case DEV_OPEN:
                msg.type = impl_list[device_id].ID_Open_func;
                break;
            case DEV_CLOSE:
                msg.type = impl_list[device_id].ID_Close_func;
                break;
            case DEV_INFO:
                msg.type = impl_list[device_id].ID_Info_func;
                break;
            default:
                // never reach here
                break;
            }

            msg.receiver         = impl_list[device_id].PID;
            msg.data.uint_arr.d4 = src;
            if (impl_list[device_id].Has_Permission_To_Write_Directly) {
                send_msg(&msg);
                recv_msg(&msg, impl_list[device_id].PID);
                int status = msg.major;
                if (status != DEV_STATUS_WAIT) {
                    msg.receiver = src;
                    send_msg(&msg); // send unblock the app proc
                } else {
                    // TODO: append to wait list
                    struct DeviceRequest *req = (struct DeviceRequest *)kmalloc(
                        sizeof(struct DeviceRequest));
                    memset(req, 0, sizeof(struct DeviceRequest));
                    req->app_pid  = src;
                    req->impl_pid = impl_list[device_id].PID;
                    req->type     = type;
                    if (impl_list[device_id].wait_list)
                        req->next = impl_list[device_id].wait_list;
                    impl_list[device_id].wait_list = req;
                }
            } else {
                // TODO: impl this
                panic("Not impl yet");
            }
            break;
        }
        case DEV_UPD: {
            // only read could use this branch
            int status = msg.major;
            if (status == DEV_STATUS_WAIT) {
                // you cannot update with a wait status
                break;
            }
            pid_t              impl_pid       = msg.sender;
            pid_t              app_pid        = msg.data.uint_arr.d4;
            BOOL               found          = FALSE;
            BOOL               has_permission = FALSE;
            struct DeviceImpl *impl           = impl_use_list;
            uint               type           = 0;

            while (impl && !found) {
                if (impl->PID == impl_pid) {
                    struct DeviceRequest *req  = impl->wait_list;
                    struct DeviceRequest *preq = req;
                    while (req) {
                        assert(req->impl_pid == impl_pid);
                        if (req->app_pid == app_pid) {
                            found = TRUE;
                            if (req != impl->wait_list) {
                                preq->next = req->next;
                            } else {
                                impl->wait_list = req->next;
                            }
                            type = req->type;
                            has_permission =
                                impl->Has_Permission_To_Write_Directly;
                            kfree((char *)req);
                            break;
                        }
                        preq = req;
                        req  = req->next;
                    }
                }
                impl = impl->next;
            }

            if (!found) {
                panic("Application PID is not in wait list.");
                break;
            }

            msg.receiver = app_pid;
            msg.type     = type;
            if (has_permission) {
                send_msg(&msg); // to unblock app proc
            } else {
                panic("Not impl yet");
            }
            break;
        }
        case DEV_LIST:
            panic("Not impl yet");
            break;
        default:
            break;
        }
    }
}
