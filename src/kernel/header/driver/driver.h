#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <core/environment.h>
#include <generic/typedefs.h>

#define DRIVER_DEP     0x7FDEDEF7
#define DRIVER_DC      0x7FDEDCF7
#define DRIVER_DC_HEAD 0x7FDEDC00

struct __Driver_Declaration {
    uint        magic;
    const char *name;
    uint        major_ver;
    uint        minor_ver;
    int         level; // -1 means not loading this driver
                       // loading order is from 0 to larger level

    struct __Driver_Declaration *next;
    init_fp                      init;
    BOOL                         initialized;
};

typedef struct __Driver_Declaration Driver_Declaration;

#define ADD_DRIVER(declaration)                                                \
    static Driver_Declaration *__ptr_##declaration                             \
        __attribute((used, section("DrvDclrList"))) = &declaration

#define section_foreach_entry(section_name, type_t, elem)                      \
    for (type_t *elem = ({                                                     \
             extern type_t __start_##section_name;                             \
             &__start_##section_name;                                          \
         });                                                                   \
         elem != ({                                                            \
             extern type_t __stop_##section_name;                              \
             &__stop_##section_name;                                           \
         });                                                                   \
         ++elem)

/* ret:
    0: driver found
    1: driver found, but version not matched
    2: driver found, but not initialized
    3: driver found, version not matched and not initialized
    -1: driver not found
*/

int check_driver_exists(const char *name, int major_ver, int minor_ver);

#endif // __DRIVER_H__