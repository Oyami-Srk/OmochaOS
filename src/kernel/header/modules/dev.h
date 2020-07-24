//
// Created by Shiroko on 2020/7/23.
//

#ifndef __MOD_DEV__
#define __MOD_DEV__

#include "generic/typedefs.h"

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

#define DEV_STATUS_READY 1
#define DEV_STATUS_WAIT  2
#define DEV_STATUS_ERROR 3

#endif // __MOD_DEV__
