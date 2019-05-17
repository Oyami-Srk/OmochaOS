#ifndef __MODULE_HD__
#define __MODULE_HD__

#include "kernel/type.h"

#define HD_REG_DATA 0x1F0
#define HD_REG_FEATURES 0x1F1
#define HD_REG_ERROR HD_REG_FEATURES
#define HD_REG_NSECTOR 0x1F2
#define HD_REG_LBA_LOW 0x1F3
#define HD_REG_LBA_MID 0x1F4
#define HD_REG_LBA_HIGH 0x1F5
#define HD_REG_DEVICE 0x1F6
/* Device Register
 * Bit 7: 1
 * Bit 6: L -> if L=0 Head select, 3-0bits contains head number
 *                L=1 3-0bits contains 24-27 of the LBA
 * Bit 5: 1
 * Bit 4: Drive -> if Drive=0 master is selected
 *                    Drive=1 slave is selected
 * Bit 3-0: CHS/LBA number
 */
#define HD_REG_STATUS 0x1F7
/* Status Register
 * Bit 7: Busy, if Busy=1, no other bits in the register are valid
 * Bit 6: DRDY Drive Ready
 * Bit 5: Device Fault / Stream Error
 * Bit 4: Command dependent.
 * Bit 3: Data Request. (ready to transfer data)
 * Bit 2: Obsolete
 * Bit 1: Obsolete
 * Bit 0: Error
 */
#define HD_STATUS_BSY 0x80
#define HD_STATUS_DRDY 0x40
#define HD_STATUS_DFSE 0x20
#define HD_STATUS_DSC 0x10
#define HD_STATUS_DRQ 0x08
#define HD_STATUS_CORR 0x04
#define HD_STATUS_IDX 0x02
#define HD_STATUS_ERR 0x01

#define HD_REG_CMD HD_REG_STATUS
/* Command Register
 *
 */

#define HD_REG_DEV_CTRL 0x3F6
#define HD_REG_ALT_STATUS HD_REG_DEV_CTRL
#define HD_REG_DRV_ADDR 0x3F7

struct HD_Command {
  u8 command;
  u8 features;
  u8 lba_low;
  u8 lba_mid;
  u8 lba_high;
  u8 counts;
  u8 device;
};

static inline struct HD_Command *HD_make_command(struct HD_Command *cmd,
                                                 uint LBA, u8 feature, u8 drive,
                                                 u8 count, u8 command) {
  cmd->counts = count;
  cmd->device = ((LBA >> 24) & 0xF) | 0xE0 | (drive << 4);
  cmd->lba_low = LBA & 0xFF;
  cmd->lba_mid = (LBA >> 8) & 0xFF;
  cmd->lba_high = (LBA >> 16) & 0xFF;
  cmd->features = feature;
  cmd->command = command;
  return cmd;
}
#define HD_TIMEOUT 10000 /* in millisec */
#define PARTITION_TABLE_OFFSET 0x1BE
#define ATA_IDENTIFY 0xEC
#define ATA_READ 0x20
#define ATA_WRITE 0x30

#define MAX_DRIVES 2
#define PART_PER_DRIVE 4
#define SUB_PER_PART 16
#define SUB_PER_DRIVE (SUB_PER_PART * PART_PER_DRIVE)
#define PRIM_PER_DRIVE (PART_PER_DRIVE + 1)

#define SECTOR_SIZE 512 // in byte
#define HD_BUFFER_SIZE 4096

struct HD_PartInfo {
  uint base;
  uint size;
};
struct HD_Info {
  uint open_cnt;
  struct HD_PartInfo primary[PRIM_PER_DRIVE];
  struct HD_PartInfo logical[SUB_PER_DRIVE];
};

struct HD_PartTableEntity {
  u8 bootable; // 80h = bootable, 00h = unbootable
  u8 head_begin;
  u8 sect_begin; // 0-5 is sector, 6-7 is upper two bits for cyli_begin
  u8 cyli_begin;
  u8 fsid;
  u8 head_end;
  u8 sect_end;
  u8 cyli_end;
  u32 lba_begin; // sector in lba
  u32 size;
};

#define HD_PART_TYPE_EXTEND 0x05
#define HD_PART_TYPE_NONE 0x00

// msg code

#define DEV_OPEN 1

#endif // __MODULE_HD__