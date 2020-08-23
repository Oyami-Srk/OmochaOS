#include <core/init.h>
#include <core/memory.h>
#include <driver/driver.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <lib/string.h>

#include <driver/graphic.h>

struct __proto_dc_dlist__ {
    Driver_Declaration *       data;
    struct __proto_dc_dlist__ *prev, *next;
};

typedef struct __proto_dc_dlist__ dc_dlist;

Driver_Declaration *g_drivers;

void core_init_driver(struct core_env *env, Driver_Declaration *drivers) {
    if (drivers->magic != DRIVER_DC_HEAD)
        magic_break();
    uint driver_count = drivers->major_ver;
    if (driver_count > PG_SIZE / sizeof(dlist))
        magic_break();
    Driver_Declaration *current_driver = drivers->next;
    g_drivers                          = drivers;

    // resort driver list
    size_t    dlist_size  = sizeof(dc_dlist) * driver_count;
    dc_dlist *dlist_block = (dc_dlist *)kalloc_pages(0);
    memset(dlist_block, 0, PG_SIZE);
    dc_dlist *head  = &dlist_block[0];
    head->data      = current_driver;
    head->next      = NULL;
    head->prev      = NULL;
    current_driver  = current_driver->next;
    uint      index = 1;
    dc_dlist *tail  = head;

    while (current_driver != NULL) {
        dc_dlist *this = &dlist_block[index];
        this->data     = current_driver;
        this->next = this->prev = NULL;

        int this_level = current_driver->level;
        if (tail->data->level <= this_level) {
            tail->next = this;
            this->prev = tail;
            tail       = this;
        } else {
            BOOL      inserted = FALSE;
            dc_dlist *p        = tail;
            while (p->prev) {
                if (p->data->level <= this_level) {
                    p->next->prev = this;
                    this->next    = p->next;
                    this->prev    = p;
                    p->next       = this;
                    inserted      = TRUE;
                    break;
                }
                p = p->prev;
            }
            if (inserted == FALSE) {
                head->prev = this;
                this->next = head;
                this->prev = NULL;
                head       = this;
            }
        }
        current_driver = current_driver->next;
        index++;
    }
    dc_dlist *d   = head;
    drivers->next = head->data;
    while (d) {
        d->data->next = d->next ? d->next->data : NULL;
        d             = d->next;
    }
    kfree_pages((void *)dlist_block);

    // init driver
    Driver_Declaration *driver = drivers->next;
    while (driver) {
        if (driver->magic != DRIVER_DC)
            magic_break();
        if (driver->level < 0) {
            driver = driver->next;
            continue;
        }
        if (driver->init == NULL || driver->init(env) == 0)
            driver->initialized = TRUE;
        else
            driver->initialized = FALSE;
        driver = driver->next;
    }

    // Graphic driver is loaded, print status
    kprintf("Driver finished loaded\n");
    driver = drivers->next;
    while (driver) {
        if (driver->magic != DRIVER_DC)
            magic_break();
        if (driver->level < 0) {
            driver = driver->next;
            continue;
        }
        kprintf("Driver(%d) %s: ", driver->level, driver->name);
        kprintf(driver->initialized ? "Loaded" : "NotLoaded");
        kprintf("\n");
        driver = driver->next;
    }
}

int check_driver_exists(const char *name, int major_ver, int minor_ver) {
    Driver_Declaration *driver = g_drivers->next;
    while (driver) {
        if (driver->magic != DRIVER_DC)
            magic_break();
        if (strcmp(name, driver->name) == 0) {
            int status = 0;
            if (major_ver != driver->major_ver ||
                minor_ver != driver->minor_ver)
                status |= 1;
            if (driver->initialized == FALSE)
                status |= 2;
            return status;
        }
        driver = driver->next;
    }
    return -1;
}