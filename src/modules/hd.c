/*
module:
  name: HD
  author: Shiroko
  summary: HD Process
  module_task: Task_HD
*/
#include "hd.h"
#include "lib/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "syscall/syscall.h"

u8 hd_status;
u8 hd_buf[HD_BUFFER_SIZE];
struct HD_Info hd_info[1];

size_t hd_rw(uint RW, u8 *buf, uint drv, uint lba, size_t count);

int wait_hd(int mask, int val, int timeout) {
  int t = get_ticks_msg();

  while (((get_ticks_msg() - t) * 1000 / 10) < timeout)
    if ((inb(HD_REG_STATUS) & mask) == val)
      return 1;
  return 0;
}

void HD_send_command(struct HD_Command *cmd) {
  if (!wait_hd(HD_STATUS_BSY, 0, HD_TIMEOUT))
    panic("HD Error");
  outb(HD_REG_DEV_CTRL, 0);
  outb(HD_REG_FEATURES, cmd->features);
  outb(HD_REG_NSECTOR, cmd->counts);
  outb(HD_REG_LBA_LOW, cmd->lba_low);
  outb(HD_REG_LBA_MID, cmd->lba_mid);
  outb(HD_REG_LBA_HIGH, cmd->lba_high);
  outb(HD_REG_DEVICE, cmd->device);
  outb(HD_REG_CMD, cmd->command);
}

void update_status(void) { hd_status = inb(HD_REG_STATUS); }

void print_identify_info(ushort *hdinfo) {
  int i, k;
  char s[64];

  struct iden_info_ascii {
    int idx;
    int len;
    char *desc;
  } iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
               {27, 40, "HD Model"} /* Model number in ASCII */};

  for (k = 0; k < sizeof(iinfo) / sizeof(iinfo[0]); k++) {
    char *p = (char *)&hdinfo[iinfo[k].idx];
    for (i = 0; i < iinfo[k].len / 2; i++) {
      s[i * 2 + 1] = *p++;
      s[i * 2] = *p++;
    }
    s[i * 2] = 0;
    printf("%s: %s\n", iinfo[k].desc, s);
  }

  int capabilities = hdinfo[49];
  printf("LBA supported: %s\n", (capabilities & 0x0200) ? "Yes" : "No");

  int cmd_set_supported = hdinfo[83];
  printf("LBA48 supported: %s\n", (cmd_set_supported & 0x0400) ? "Yes" : "No");

  int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
  printf("HD size: %dMB\n", sectors * 512 / 1024 / 1024);
}

void hd_identify(int drv) {
  delay_ms(10);
  struct HD_Command cmd;
  HD_make_command(&cmd, 0, 0, drv, 0, ATA_IDENTIFY);
  HD_send_command(&cmd);
  message msg;
  recv_msg(&msg, INTERRUPT);
  update_status();
  insl(HD_REG_DATA, hd_buf, 512 / 4);
  print_identify_info((ushort *)hd_buf);

  hd_info[drv].primary[0].base = 0;
  hd_info[drv].primary[0].size =
      ((int)((u16 *)(hd_buf))[61] << 16) + ((u16 *)(hd_buf))[60];
}

void part_identify(int drv) {}

void part_table_read(int drv, uint sect, struct HD_PartTableEntity *ent) {
  u8 buf[512];
  hd_rw(0, buf, drv, sect, 512);
  memcpy(ent, buf + 0x1BE, sizeof(struct HD_PartTableEntity) * PART_PER_DRIVE);
}

void init_partitions(int drv, int type, uint extend_id) {
  struct HD_Info *info = &hd_info[drv];
  struct HD_PartTableEntity part_table[SUB_PER_DRIVE];
  memset(part_table, 0, sizeof(struct HD_PartTableEntity));

  if (type != HD_PART_TYPE_EXTEND) { // primary
    part_table_read(drv, 0, part_table);
    size_t prim_parts = 0;
    for (uint i = 0; i < PART_PER_DRIVE; i++) {
      if (part_table[i].fsid == HD_PART_TYPE_NONE)
        continue;
      prim_parts++;
      info->primary[i + 1].base = part_table[i].lba_begin;
      info->primary[i + 1].size = part_table[i].size;

      if (part_table[i].fsid == HD_PART_TYPE_EXTEND)
        init_partitions(drv, HD_PART_TYPE_EXTEND, i + 1);
    }
    if (prim_parts == 0)
      panic("No Part Table Found!");
  } else {
    uint ext_base = info->primary[extend_id].base;
    uint start_ext_part = (extend_id - 1) * SUB_PER_PART;
    uint sect = ext_base;

    for (uint i = 0; i < SUB_PER_PART; i++) {
      part_table_read(drv, sect, part_table);
      info->logical[start_ext_part + i].base = sect + part_table[0].lba_begin;
      info->logical[start_ext_part + i].size = part_table[0].size;

      sect = ext_base + part_table[1].lba_begin;

      if (part_table[1].fsid == HD_PART_TYPE_NONE)
        break;
    }
  }
}

void print_part_info(struct HD_Info *info) {
  for (uint i = 0; i < PART_PER_DRIVE + 1; i++) {
    printf("%sPART_%d: Begin %d(0x%x) Size %d(0x%x)\n", i == 0 ? " " : "    ",
           i, info->primary[i].base, info->primary[i].base,
           info->primary[i].size, info->primary[i].size);
  }
  for (uint i = 0; i < SUB_PER_DRIVE; i++) {
    if (info->logical[i].size == 0)
      continue;
    printf("        "
           "%d: Begin %d(0x%x) Size %d(0x%x)\n",
           i, info->logical[i].base, info->logical[i].base,
           info->logical[i].size, info->logical[i].size);
  }
}

void init_hd() {
  uchar *pDrives_n = (uchar *)0x475;
  printf("[HD] Drives num: %d\n", *pDrives_n);
  if (reg_int_msg(HW_IRQ_AT))
    printf("[HD] Cannot attach to interrupt AT\n");
  enable_irq(2); // Cascade
  enable_irq(HW_IRQ_AT);
  for (int i = 0; i < (sizeof(hd_info) / sizeof(hd_info[0])); i++) {
    memset(&hd_info[i], 0, sizeof(hd_info[0]));
    hd_info[i].open_cnt = 0;
  }
}

void hd_open(int drv) {
  hd_identify(drv);
  if (hd_info[drv].open_cnt++ == 0) {
    init_partitions(drv, 0, 0);
    print_part_info(&hd_info[drv]);
  }
}

// RW: 0-> Read 1->Write
size_t hd_rw(uint RW, u8 *buf, uint drv, uint lba, size_t count) {
  if (RW && count % 512)
    hd_rw(0, hd_buf, drv, lba + (count + SECTOR_SIZE - 1) / SECTOR_SIZE - 1,
          SECTOR_SIZE);
  struct HD_Command cmd;
  HD_make_command(&cmd, lba, 0, drv, (count + SECTOR_SIZE - 1) / SECTOR_SIZE,
                  RW ? ATA_WRITE : ATA_READ);
  HD_send_command(&cmd);
  message msg;
  size_t bytes_left = count;
  void *addr = (void *)buf; // 当定址空间分开的时候 or 
                            // 实现了Memory Manager的时候
                            // 这里应该获取硬件地址

  while (bytes_left) {
    size_t bytes = MIN(SECTOR_SIZE, bytes_left);
    if (RW == 0) { // Read
      recv_msg(&msg, INTERRUPT);
      insl(HD_REG_DATA, hd_buf, SECTOR_SIZE / 4);
      memcpy(addr, hd_buf, bytes);
    } else { // Write
      if (!wait_hd(HD_STATUS_DRQ, HD_STATUS_DRQ, HD_TIMEOUT))
        panic("Disk Error");
      if (bytes == SECTOR_SIZE)
        outsl(HD_REG_DATA, addr, bytes / 4);
      else {
        memcpy(hd_buf, addr, bytes);
        outsl(HD_REG_DATA, hd_buf, SECTOR_SIZE / 4);
      }
      recv_msg(&msg, INTERRUPT);
    }
    bytes_left -= bytes;
    addr += bytes;
  }
  return count;
}
void Task_HD() {
  message msg;
  if (reg_proc("TaskHD") != 0)
    printf("[HD] Cannot register as TaskHD");
  printf("\n[HD] Initialized\n");
  init_hd();
  delay_ms(200);
  while (1) {
    recv_msg(&msg, ANY);
    int src = msg.sender;
    switch (msg.type) {
    case INTERRUPT:
      printf(".");
      update_status();
      break;
    case DEV_OPEN:
      hd_open(msg.major_data);
      break;
    }
  }
}
