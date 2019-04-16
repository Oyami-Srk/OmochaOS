/*
module:
  name: HD
  author: Shiroko
  summary: HD Process
  module_task: Task_HD
*/
#include "modules/modules.h"
#include "HD.h"
#include "kernel/klib.h"

struct hd_info hd_info[1];
uchar hd_status;
uchar hdbuf[1024];

void initialize_hd(){
  uchar *pDrives_n = (uchar*)0x475;
  printf("[HD] Drives num: %d\n", *pDrives_n);
  if(register_to(HW_IRQ_AT, INT_HANDLE_METHOD_DISPATCH, 0) != 0)
    printf("[HD] Cannot attach to interrupt AT\n");
  enable_irq(2); // Cascade
  enable_irq(HW_IRQ_AT);
  for(int i = 0; i < (sizeof(hd_info)/sizeof(hd_info[0])); i++)
    memset(&hd_info[i], 0, sizeof(hd_info[0]));
  hd_info[0].open_cnt = 0;
}

int waitfor(int mask, int val, int timeout) {
	int t = get_ticks();

	while (((get_ticks_msg() - t) * 1000 / 10) < timeout)
		if ((inb(REG_STATUS) & mask) == val)
			return 1;
	return 0;
}

void update_status(void){
  hd_status = inb(REG_STATUS);
}

void hd_outcmd(struct hd_cmd *cmd){
  if(!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
    panic("hd error");
  outb(REG_DEV_CTRL, 0);
  outb(REG_FEATURES, cmd->features);
  outb(REG_NSECTOR, cmd->count);
  outb(REG_LBA_LOW, cmd->lba_low);
  outb(REG_LBA_MID, cmd->lba_mid);
  outb(REG_LBA_HIGH, cmd->lba_high);
  outb(REG_DEVICE, cmd->device);
  outb(REG_CMD, cmd->command);
}

void print_identify_info(ushort *hdinfo)
{
	int i, k;
	char s[64];

	struct iden_info_ascii
	{
		int idx;
		int len;
		char *desc;
	} iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
               {27, 40, "HD Model"} /* Model number in ASCII */};

	for (k = 0; k < sizeof(iinfo) / sizeof(iinfo[0]); k++)
    {
      char *p = (char *)&hdinfo[iinfo[k].idx];
      for (i = 0; i < iinfo[k].len / 2; i++)
        {
          s[i * 2 + 1] = *p++;
          s[i * 2] = *p++;
        }
      s[i * 2] = 0;
      printf("%s: %s\n", iinfo[k].desc, s);
    }

	int capabilities = hdinfo[49];
	printf("LBA supported: %s\n",
         (capabilities & 0x0200) ? "Yes" : "No");

	int cmd_set_supported = hdinfo[83];
	printf("LBA48 supported: %s\n",
         (cmd_set_supported & 0x0400) ? "Yes" : "No");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	printf("HD size: %dMB\n", sectors * 512 / 1000000);
}

void hd_identify(int dev){
  delay_ms(10);
  struct hd_cmd cmd;
  cmd.device = MAKE_DEVICE_REG(0, dev, 0);
  cmd.command = ATA_IDENTIFY;
  hd_outcmd(&cmd);
  message msg;
  recv_msg(&msg, INTERRUPT);
  update_status();
  insl(REG_DATA, hdbuf, 512 / 4);
  printf("Done");
  print_identify_info((ushort*)hdbuf);
}

void hd_open(int dev){
  hd_identify(0);
  hd_info[dev].open_cnt++;

}

void Task_HD(){
  message msg;
  if (register_proc("TaskHD") != 0)
    kprintf("Cannot register as TaskHD");
  get_ticks_msg();
  delay_ms(1000);
  printf("Initialized\n");
  initialize_hd();
  hd_open(0);

  while (1) {
    recv_msg(&msg, ANY);
    int src = msg.sender;
    switch(msg.type){
    case DEV_OPEN:
      hd_open(msg.major_data);
      break;
    case INTERRUPT:
      printf(".");
      update_status();
    }
  }
}
