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
u8 hd_buf[1024];
struct HD_Info hd_info[1];

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
  printf("HD size: %dMB\n", sectors * 512 / 1000000);
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
  hd_info[drv].open_cnt++;
}

void Task_HD() {
  message msg;
  if (reg_proc("TaskHD") != 0)
    printf("[HD] Cannot register as TaskHD");
  printf("\n[HD] Initialized\n");
  init_hd();
  while (1) {
    recv_msg(&msg, ANY);
    int src = msg.sender;
    switch (msg.type) {
    case INTERRUPT:
      update_status();
      break;
    case DEV_OPEN:
      hd_open(msg.major_data);
      break;
    }
  }
}
