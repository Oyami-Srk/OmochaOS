
build/kernel/kernel.out：     文件格式 elf32-i386


Disassembly of section .text:

80101000 <core_main>:
Descriptor gdt[GDT_SIZE];
struct tss tss;

uint beats = 0;

void core_main(multiboot_info_t *multiboot_header, u32 magic) {
80101000:	53                   	push   %ebx
80101001:	83 ec 08             	sub    $0x8,%esp
80101004:	8b 5c 24 10          	mov    0x10(%esp),%ebx
80101008:	8b 44 24 14          	mov    0x14(%esp),%eax
    if (magic == MULTIBOOT_HEADER_MAGIC)
8010100c:	3d 02 b0 ad 1b       	cmp    $0x1badb002,%eax
80101011:	0f 84 c0 00 00 00    	je     801010d7 <core_main+0xd7>
        kprintf("Venlafaxos is booting by multiboot bootloader.\n");
    else
        kprintf("Venlafaxos is booting. magic is 0x%08x\n", magic);
80101017:	83 ec 08             	sub    $0x8,%esp
8010101a:	50                   	push   %eax
8010101b:	68 30 30 10 80       	push   $0x80103030
80101020:	e8 14 10 00 00       	call   80102039 <kprintf>
80101025:	83 c4 10             	add    $0x10,%esp
    kprintf("KERN_VEND: 0x%08x\n", KERN_VEND);
80101028:	83 ec 08             	sub    $0x8,%esp
8010102b:	68 d0 c0 10 80       	push   $0x8010c0d0
80101030:	68 81 30 10 80       	push   $0x80103081
80101035:	e8 ff 0f 00 00       	call   80102039 <kprintf>
    kprintf("MEM_LOWER is 0x%08x\nMEM_UPPER is 0x%08x\n",
8010103a:	83 c4 0c             	add    $0xc,%esp
8010103d:	ff 73 08             	pushl  0x8(%ebx)
80101040:	ff 73 04             	pushl  0x4(%ebx)
80101043:	68 58 30 10 80       	push   $0x80103058
80101048:	e8 ec 0f 00 00       	call   80102039 <kprintf>
            multiboot_header->mem_lower, multiboot_header->mem_upper);
    core_init_memory(KERN_VEND,
8010104d:	83 c4 08             	add    $0x8,%esp
80101050:	68 00 00 40 80       	push   $0x80400000
80101055:	68 d0 c0 10 80       	push   $0x8010c0d0
8010105a:	e8 49 02 00 00       	call   801012a8 <core_init_memory>
                     KP2V(4 * 1024 * 1024)); // to 4MB, the first remap
    core_init_gdt(gdt, GDT_SIZE, &tss);
8010105f:	83 c4 0c             	add    $0xc,%esp
80101062:	68 40 b8 10 80       	push   $0x8010b840
80101067:	68 80 00 00 00       	push   $0x80
8010106c:	68 40 b4 10 80       	push   $0x8010b440
80101071:	e8 c8 02 00 00       	call   8010133e <core_init_gdt>
    kprintf("GDT Initialized.\n");
80101076:	c7 04 24 94 30 10 80 	movl   $0x80103094,(%esp)
8010107d:	e8 b7 0f 00 00       	call   80102039 <kprintf>
    core_init_interrupt(idt, IVT_COUNT);
80101082:	83 c4 08             	add    $0x8,%esp
80101085:	68 00 01 00 00       	push   $0x100
8010108a:	68 c0 b8 10 80       	push   $0x8010b8c0
8010108f:	e8 1e 01 00 00       	call   801011b2 <core_init_interrupt>
    kprintf("IDT Initialized.\n");
80101094:	c7 04 24 a6 30 10 80 	movl   $0x801030a6,(%esp)
8010109b:	e8 99 0f 00 00       	call   80102039 <kprintf>
    kprintf("gdt is located in 0x%08x", gdt);
801010a0:	83 c4 08             	add    $0x8,%esp
801010a3:	68 40 b4 10 80       	push   $0x8010b440
801010a8:	68 b8 30 10 80       	push   $0x801030b8
801010ad:	e8 87 0f 00 00       	call   80102039 <kprintf>
801010b2:	83 c4 10             	add    $0x10,%esp
    while (1) {
        kprintf("%d.", beats);
801010b5:	83 ec 08             	sub    $0x8,%esp
801010b8:	ff 35 20 94 10 80    	pushl  0x80109420
801010be:	68 d1 30 10 80       	push   $0x801030d1
801010c3:	e8 71 0f 00 00       	call   80102039 <kprintf>
801010c8:	83 c4 10             	add    $0x10,%esp
801010cb:	b8 40 42 0f 00       	mov    $0xf4240,%eax
        for (uint i = 0; i < 1000000; i++)
801010d0:	83 e8 01             	sub    $0x1,%eax
801010d3:	75 fb                	jne    801010d0 <core_main+0xd0>
801010d5:	eb de                	jmp    801010b5 <core_main+0xb5>
        kprintf("Venlafaxos is booting by multiboot bootloader.\n");
801010d7:	83 ec 0c             	sub    $0xc,%esp
801010da:	68 00 30 10 80       	push   $0x80103000
801010df:	e8 55 0f 00 00       	call   80102039 <kprintf>
801010e4:	83 c4 10             	add    $0x10,%esp
801010e7:	e9 3c ff ff ff       	jmp    80101028 <core_main+0x28>

801010ec <init_8259A>:
extern uint vector_table[];

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }
static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

void init_8259A() {
801010ec:	53                   	push   %ebx
                 : "0"(addr), "1"(size), "a"(data)
                 : "memory", "cc");
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
801010ed:	b8 11 00 00 00       	mov    $0x11,%eax
801010f2:	ba 20 00 00 00       	mov    $0x20,%edx
801010f7:	ee                   	out    %al,(%dx)
801010f8:	ba a0 00 00 00       	mov    $0xa0,%edx
801010fd:	ee                   	out    %al,(%dx)
801010fe:	bb 21 00 00 00       	mov    $0x21,%ebx
80101103:	b8 20 00 00 00       	mov    $0x20,%eax
80101108:	89 da                	mov    %ebx,%edx
8010110a:	ee                   	out    %al,(%dx)
8010110b:	b9 a1 00 00 00       	mov    $0xa1,%ecx
80101110:	b8 28 00 00 00       	mov    $0x28,%eax
80101115:	89 ca                	mov    %ecx,%edx
80101117:	ee                   	out    %al,(%dx)
80101118:	b8 04 00 00 00       	mov    $0x4,%eax
8010111d:	89 da                	mov    %ebx,%edx
8010111f:	ee                   	out    %al,(%dx)
80101120:	b8 02 00 00 00       	mov    $0x2,%eax
80101125:	89 ca                	mov    %ecx,%edx
80101127:	ee                   	out    %al,(%dx)
80101128:	b8 01 00 00 00       	mov    $0x1,%eax
8010112d:	89 da                	mov    %ebx,%edx
8010112f:	ee                   	out    %al,(%dx)
80101130:	89 ca                	mov    %ecx,%edx
80101132:	ee                   	out    %al,(%dx)
80101133:	b8 fe ff ff ff       	mov    $0xfffffffe,%eax
80101138:	89 da                	mov    %ebx,%edx
8010113a:	ee                   	out    %al,(%dx)
8010113b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
80101140:	89 ca                	mov    %ecx,%edx
80101142:	ee                   	out    %al,(%dx)
    outb(IO_PIC_M + 1, 0x1);
    outb(IO_PIC_S + 1, 0x1);

    outb(IO_PIC_M + 1, 0xFE);
    outb(IO_PIC_S + 1, 0xFF);
}
80101143:	5b                   	pop    %ebx
80101144:	c3                   	ret    

80101145 <enable_irq>:

void enable_irq(uint irq) {
80101145:	53                   	push   %ebx
80101146:	8b 4c 24 08          	mov    0x8(%esp),%ecx
    if (irq < 8)
8010114a:	83 f9 07             	cmp    $0x7,%ecx
8010114d:	77 17                	ja     80101166 <enable_irq+0x21>
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char r;
    asm volatile("in %1, %0" : "=a"(r) : "d"(port));
8010114f:	ba 20 00 00 00       	mov    $0x20,%edx
80101154:	ec                   	in     (%dx),%al
        outb(IO_PIC_M + 1, inb(IO_PIC_M) & ~(1 << irq));
80101155:	bb fe ff ff ff       	mov    $0xfffffffe,%ebx
8010115a:	d3 c3                	rol    %cl,%ebx
8010115c:	21 d8                	and    %ebx,%eax
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
8010115e:	ba 21 00 00 00       	mov    $0x21,%edx
80101163:	ee                   	out    %al,(%dx)
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) & ~(1 << irq));
}
80101164:	5b                   	pop    %ebx
80101165:	c3                   	ret    
    asm volatile("in %1, %0" : "=a"(r) : "d"(port));
80101166:	ba a0 00 00 00       	mov    $0xa0,%edx
8010116b:	ec                   	in     (%dx),%al
        outb(IO_PIC_S + 1, inb(IO_PIC_S) & ~(1 << irq));
8010116c:	bb fe ff ff ff       	mov    $0xfffffffe,%ebx
80101171:	d3 c3                	rol    %cl,%ebx
80101173:	21 d8                	and    %ebx,%eax
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
80101175:	ba a1 00 00 00       	mov    $0xa1,%edx
8010117a:	ee                   	out    %al,(%dx)
}
8010117b:	eb e7                	jmp    80101164 <enable_irq+0x1f>

8010117d <disable_irq>:

void disable_irq(uint irq) {
8010117d:	8b 4c 24 04          	mov    0x4(%esp),%ecx
    if (irq < 8)
80101181:	83 f9 07             	cmp    $0x7,%ecx
80101184:	77 16                	ja     8010119c <disable_irq+0x1f>
    asm volatile("in %1, %0" : "=a"(r) : "d"(port));
80101186:	ba 20 00 00 00       	mov    $0x20,%edx
8010118b:	ec                   	in     (%dx),%al
        outb(IO_PIC_M + 1, inb(IO_PIC_M) | (1 << irq));
8010118c:	ba 01 00 00 00       	mov    $0x1,%edx
80101191:	d3 e2                	shl    %cl,%edx
80101193:	09 d0                	or     %edx,%eax
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
80101195:	ba 21 00 00 00       	mov    $0x21,%edx
8010119a:	ee                   	out    %al,(%dx)
}
8010119b:	c3                   	ret    
    asm volatile("in %1, %0" : "=a"(r) : "d"(port));
8010119c:	ba a0 00 00 00       	mov    $0xa0,%edx
801011a1:	ec                   	in     (%dx),%al
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) | (1 << irq));
801011a2:	ba 01 00 00 00       	mov    $0x1,%edx
801011a7:	d3 e2                	shl    %cl,%edx
801011a9:	09 d0                	or     %edx,%eax
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
801011ab:	ba a1 00 00 00       	mov    $0xa1,%edx
801011b0:	ee                   	out    %al,(%dx)
}
801011b1:	c3                   	ret    

801011b2 <core_init_interrupt>:

void core_init_interrupt(Gate *idt, size_t count) {
801011b2:	56                   	push   %esi
801011b3:	53                   	push   %ebx
801011b4:	83 ec 10             	sub    $0x10,%esp
801011b7:	8b 5c 24 1c          	mov    0x1c(%esp),%ebx
801011bb:	8b 74 24 20          	mov    0x20(%esp),%esi
    if (count < IVT_COUNT) {
801011bf:	81 fe ff 00 00 00    	cmp    $0xff,%esi
801011c5:	77 05                	ja     801011cc <core_init_interrupt+0x1a>
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "cc");
}

#ifdef __DEBUG__
static inline void magic_break(void) { asm volatile("xchgw %bx, %bx"); }
801011c7:	66 87 db             	xchg   %bx,%bx
        magic_break();
        while (1)
801011ca:	eb fe                	jmp    801011ca <core_init_interrupt+0x18>
801011cc:	89 d8                	mov    %ebx,%eax
            ; // panic
    }
    for (uint i = 0; i < IVT_COUNT; i++) {
801011ce:	b9 00 00 00 00       	mov    $0x0,%ecx
        make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL0, GATE_INT32);
801011d3:	8b 14 8d 00 80 10 80 	mov    -0x7fef8000(,%ecx,4),%edx
    return pDesc;
}

static inline Gate *make_gate(Gate *pGate, u16 Selector, u32 Offset, uint DPL,
                              uint Type) {
    pGate->offset_l = Offset & 0xFFFF;
801011da:	66 89 10             	mov    %dx,(%eax)
    pGate->offset_h = (Offset >> 16) & 0xFFFF;
801011dd:	c1 ea 10             	shr    $0x10,%edx
801011e0:	66 89 50 06          	mov    %dx,0x6(%eax)
    pGate->selector = Selector;
801011e4:	66 c7 40 02 08 00    	movw   $0x8,0x2(%eax)
    pGate->zero     = 0;
801011ea:	c6 40 04 00          	movb   $0x0,0x4(%eax)
    pGate->type     = Type;
    pGate->s        = 0;
    pGate->dpl      = DPL;
    pGate->p        = 1;
801011ee:	c6 40 05 8e          	movb   $0x8e,0x5(%eax)
    for (uint i = 0; i < IVT_COUNT; i++) {
801011f2:	83 c1 01             	add    $0x1,%ecx
801011f5:	83 c0 08             	add    $0x8,%eax
801011f8:	81 f9 00 01 00 00    	cmp    $0x100,%ecx
801011fe:	75 d3                	jne    801011d3 <core_init_interrupt+0x21>
    }
    uchar idt_ptr[6];
    u16 * idt_limit = (u16 *)(&idt_ptr[0]);
    u32 * idt_base  = (u32 *)(&idt_ptr[2]);
    *idt_limit      = count * sizeof(Gate) - 1;
80101200:	8d 04 f5 ff ff ff ff 	lea    -0x1(,%esi,8),%eax
80101207:	66 89 44 24 0a       	mov    %ax,0xa(%esp)
    *idt_base       = (u32)KV2P(idt);
8010120c:	8d 83 00 00 00 80    	lea    -0x80000000(%ebx),%eax
80101212:	89 44 24 0c          	mov    %eax,0xc(%esp)
    asm volatile("lidt (%0)" ::"r"(idt_ptr));
80101216:	8d 44 24 0a          	lea    0xa(%esp),%eax
8010121a:	0f 01 18             	lidtl  (%eax)
    init_8259A();
8010121d:	e8 ca fe ff ff       	call   801010ec <init_8259A>
    asm("sti");
80101222:	fb                   	sti    
}
80101223:	83 c4 10             	add    $0x10,%esp
80101226:	5b                   	pop    %ebx
80101227:	5e                   	pop    %esi
80101228:	c3                   	ret    

80101229 <interrupt_handler>:
extern uint beats;

void interrupt_handler(stack_frame *intf) {
    if (intf->trap_no <= IRQ0 + HW_IRQ_COUNT && intf->trap_no > IRQ_TIMER) {
    }
    switch (intf->trap_no) {
80101229:	8b 44 24 04          	mov    0x4(%esp),%eax
8010122d:	83 78 30 20          	cmpl   $0x20,0x30(%eax)
80101231:	74 01                	je     80101234 <interrupt_handler+0xb>
        EOI_M();
        break;
    default:
        break;
    }
}
80101233:	c3                   	ret    
        beats++;
80101234:	83 05 20 94 10 80 01 	addl   $0x1,0x80109420
    asm volatile("out %0, %1" ::"a"(data), "d"(port));
8010123b:	b8 20 00 00 00       	mov    $0x20,%eax
80101240:	ba 20 00 00 00       	mov    $0x20,%edx
80101245:	ee                   	out    %al,(%dx)
}
80101246:	eb eb                	jmp    80101233 <interrupt_handler+0xa>

80101248 <kfree>:
#include "lib/string.h"

list *freemem;

// free a page
void kfree(char *p) {
80101248:	53                   	push   %ebx
80101249:	83 ec 08             	sub    $0x8,%esp
8010124c:	8b 5c 24 10          	mov    0x10(%esp),%ebx
    list *current;
    // only free page higher then Kernel's end and lower than 0xE00000 in phy.
    // and p must be start of a page.
    if ((uint)p % PG_SIZE || (uint)p < (uint)KERN_VEND ||
80101250:	f7 c3 ff 0f 00 00    	test   $0xfff,%ebx
80101256:	75 15                	jne    8010126d <kfree+0x25>
80101258:	81 fb d0 c0 10 80    	cmp    $0x8010c0d0,%ebx
8010125e:	72 0d                	jb     8010126d <kfree+0x25>
        (uint)KV2P(p) >= 0xE00000) {
80101260:	8d 83 00 00 00 80    	lea    -0x80000000(%ebx),%eax
    if ((uint)p % PG_SIZE || (uint)p < (uint)KERN_VEND ||
80101266:	3d ff ff df 00       	cmp    $0xdfffff,%eax
8010126b:	76 05                	jbe    80101272 <kfree+0x2a>
static inline void magic_break(void) { asm volatile("xchgw %bx, %bx"); }
8010126d:	66 87 db             	xchg   %bx,%bx
        magic_break();
        while (1)
80101270:	eb fe                	jmp    80101270 <kfree+0x28>
            ;
    }
    memset(p, 0, PG_SIZE);
80101272:	83 ec 04             	sub    $0x4,%esp
80101275:	68 00 10 00 00       	push   $0x1000
8010127a:	6a 00                	push   $0x0
8010127c:	53                   	push   %ebx
8010127d:	e8 5b 11 00 00       	call   801023dd <memset>

    // insert to head of freememlist
    current       = (list *)p;
    current->next = freemem;
80101282:	a1 c0 c0 10 80       	mov    0x8010c0c0,%eax
80101287:	89 43 04             	mov    %eax,0x4(%ebx)
    freemem       = current;
8010128a:	89 1d c0 c0 10 80    	mov    %ebx,0x8010c0c0
}
80101290:	83 c4 18             	add    $0x18,%esp
80101293:	5b                   	pop    %ebx
80101294:	c3                   	ret    

80101295 <kalloc>:

// allocate a page
char *kalloc(void) {
    list *current;
    current = freemem;
80101295:	a1 c0 c0 10 80       	mov    0x8010c0c0,%eax
    if (current)
8010129a:	85 c0                	test   %eax,%eax
8010129c:	74 09                	je     801012a7 <kalloc+0x12>
        freemem = current->next;
8010129e:	8b 50 04             	mov    0x4(%eax),%edx
801012a1:	89 15 c0 c0 10 80    	mov    %edx,0x8010c0c0
    return (char *)current;
}
801012a7:	c3                   	ret    

801012a8 <core_init_memory>:

// init memory for very first time being
void core_init_memory(void *vstart, void *vend) {
801012a8:	56                   	push   %esi
801012a9:	53                   	push   %ebx
801012aa:	83 ec 04             	sub    $0x4,%esp
801012ad:	8b 74 24 14          	mov    0x14(%esp),%esi
    // align to higher addr than vstart
    void *p = (void *)((uint)(vstart + PG_SIZE - 1) & ~(PG_SIZE - 1));
801012b1:	8b 44 24 10          	mov    0x10(%esp),%eax
801012b5:	8d 98 ff 0f 00 00    	lea    0xfff(%eax),%ebx
801012bb:	81 e3 00 f0 ff ff    	and    $0xfffff000,%ebx
    for (; p + PG_SIZE <= vend; p += PG_SIZE)
801012c1:	81 c3 00 10 00 00    	add    $0x1000,%ebx
801012c7:	39 de                	cmp    %ebx,%esi
801012c9:	72 1c                	jb     801012e7 <core_init_memory+0x3f>
        kfree(p);
801012cb:	83 ec 0c             	sub    $0xc,%esp
801012ce:	8d 83 00 f0 ff ff    	lea    -0x1000(%ebx),%eax
801012d4:	50                   	push   %eax
801012d5:	e8 6e ff ff ff       	call   80101248 <kfree>
    for (; p + PG_SIZE <= vend; p += PG_SIZE)
801012da:	81 c3 00 10 00 00    	add    $0x1000,%ebx
801012e0:	83 c4 10             	add    $0x10,%esp
801012e3:	39 f3                	cmp    %esi,%ebx
801012e5:	76 e4                	jbe    801012cb <core_init_memory+0x23>
}
801012e7:	83 c4 04             	add    $0x4,%esp
801012ea:	5b                   	pop    %ebx
801012eb:	5e                   	pop    %esi
801012ec:	c3                   	ret    

801012ed <__lgdt__>:
Descriptor *_GDT_start;
Descriptor *_GDT_current; // point to next empty slot;
Descriptor *_GDT_end;

// must be called after set _GDT_start and _GDT_end
void __lgdt__(BOOL has_tss, u16 selector_tss) {
801012ed:	83 ec 10             	sub    $0x10,%esp
801012f0:	8b 4c 24 18          	mov    0x18(%esp),%ecx
    uchar gdt_ptr[6];
    u16 * gdt_limit = (u16 *)(&gdt_ptr[0]);
    u32 * gdt_base  = (u32 *)(&gdt_ptr[2]);

    *gdt_limit = (_GDT_end - _GDT_start + 1) * sizeof(Descriptor) - 1;
801012f4:	8b 15 c8 c0 10 80    	mov    0x8010c0c8,%edx
801012fa:	a1 c4 c0 10 80       	mov    0x8010c0c4,%eax
801012ff:	29 d0                	sub    %edx,%eax
80101301:	83 c0 07             	add    $0x7,%eax
80101304:	66 89 44 24 0a       	mov    %ax,0xa(%esp)
    *gdt_base  = (u32)_GDT_start;
80101309:	89 54 24 0c          	mov    %edx,0xc(%esp)

    asm volatile("lgdtl (%0)" ::"r"(gdt_ptr));
8010130d:	8d 44 24 0a          	lea    0xa(%esp),%eax
80101311:	0f 01 10             	lgdtl  (%eax)
    asm volatile("lgdtl (%0)" ::"r"(gdt_ptr));
80101314:	0f 01 10             	lgdtl  (%eax)
    asm("\n"
80101317:	6a 08                	push   $0x8
80101319:	68 1f 13 10 80       	push   $0x8010131f
8010131e:	cb                   	lret   
8010131f:	b8 10 00 00 00       	mov    $0x10,%eax
80101324:	8e d8                	mov    %eax,%ds
80101326:	8e c0                	mov    %eax,%es
80101328:	8e e0                	mov    %eax,%fs
8010132a:	8e e8                	mov    %eax,%gs
8010132c:	8e d0                	mov    %eax,%ss
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        : "g"(0x08), "g"(0x10)
        : "eax", "memory"); // flush the cpu
    if (has_tss)
8010132e:	83 7c 24 14 00       	cmpl   $0x0,0x14(%esp)
80101333:	74 05                	je     8010133a <__lgdt__+0x4d>
        asm volatile("ltr %%ax" ::"a"(selector_tss));
80101335:	89 c8                	mov    %ecx,%eax
80101337:	0f 00 d8             	ltr    %ax
}
8010133a:	83 c4 10             	add    $0x10,%esp
8010133d:	c3                   	ret    

8010133e <core_init_gdt>:

void core_init_gdt(Descriptor *GDT, size_t gdt_count, struct tss *tss) {
8010133e:	55                   	push   %ebp
8010133f:	57                   	push   %edi
80101340:	56                   	push   %esi
80101341:	53                   	push   %ebx
80101342:	83 ec 1c             	sub    $0x1c,%esp
80101345:	8b 44 24 30          	mov    0x30(%esp),%eax
80101349:	8b 54 24 34          	mov    0x34(%esp),%edx
    size_t size         = sizeof(KERN_GDT) / sizeof(KERN_GDT[0]);
    BOOL   has_tss      = FALSE;
    uint   gdt_n        = 0;
    u16    selector_tss = 0;

    if (size > gdt_count) {
8010134d:	83 fa 04             	cmp    $0x4,%edx
80101350:	77 05                	ja     80101357 <core_init_gdt+0x19>
80101352:	66 87 db             	xchg   %bx,%bx
        magic_break();
        while (1)
80101355:	eb fe                	jmp    80101355 <core_init_gdt+0x17>
            ;
    }

    _GDT_start   = GDT;
80101357:	a3 c8 c0 10 80       	mov    %eax,0x8010c0c8
    _GDT_current = _GDT_start;
8010135c:	a3 cc c0 10 80       	mov    %eax,0x8010c0cc
    _GDT_end     = _GDT_start + gdt_count - 1;
80101361:	8d 44 d0 f8          	lea    -0x8(%eax,%edx,8),%eax
80101365:	a3 c4 c0 10 80       	mov    %eax,0x8010c0c4
        case DA_386TSS:
            has_tss      = TRUE;
            selector_tss = i << 3;
            memset(tss, 0, sizeof(struct tss));
            tss->ss0 = 2 << 3; // data selector, hard code is bad
            make_descriptor(_GDT_current, (uint)(KV2P(tss)),
8010136a:	8b 44 24 38          	mov    0x38(%esp),%eax
8010136e:	8d b0 00 00 00 80    	lea    -0x80000000(%eax),%esi
    pDesc->base_m        = (Base >> 16) & 0x0FF;
80101374:	89 f0                	mov    %esi,%eax
80101376:	c1 e8 10             	shr    $0x10,%eax
80101379:	88 44 24 07          	mov    %al,0x7(%esp)
    pDesc->base_h        = (Base >> 24) & 0x0FF;
8010137d:	89 f0                	mov    %esi,%eax
8010137f:	c1 e8 18             	shr    $0x18,%eax
80101382:	89 44 24 08          	mov    %eax,0x8(%esp)
80101386:	bb e0 30 10 80       	mov    $0x801030e0,%ebx
8010138b:	bd 00 00 00 00       	mov    $0x0,%ebp
    u16    selector_tss = 0;
80101390:	66 c7 44 24 04 00 00 	movw   $0x0,0x4(%esp)
    BOOL   has_tss      = FALSE;
80101397:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
            has_tss      = TRUE;
8010139e:	89 74 24 0c          	mov    %esi,0xc(%esp)
801013a2:	eb 69                	jmp    8010140d <core_init_gdt+0xcf>
                            sizeof(struct tss) - 1, DA_386TSS);
            _GDT_current++;
            break;
        case DA_LDT:
            __lgdt__(has_tss, selector_tss);
801013a4:	83 ec 08             	sub    $0x8,%esp
801013a7:	0f b7 44 24 0c       	movzwl 0xc(%esp),%eax
801013ac:	50                   	push   %eax
801013ad:	ff 74 24 0c          	pushl  0xc(%esp)
801013b1:	e8 37 ff ff ff       	call   801012ed <__lgdt__>
            return;
801013b6:	83 c4 10             	add    $0x10,%esp
            _GDT_current++;
            break;
        }

    __lgdt__(has_tss, selector_tss);
}
801013b9:	83 c4 1c             	add    $0x1c,%esp
801013bc:	5b                   	pop    %ebx
801013bd:	5e                   	pop    %esi
801013be:	5f                   	pop    %edi
801013bf:	5d                   	pop    %ebp
801013c0:	c3                   	ret    
            make_descriptor(_GDT_current, KERN_GDT[i][0], KERN_GDT[i][1],
801013c1:	8b 7b 04             	mov    0x4(%ebx),%edi
801013c4:	8b 0b                	mov    (%ebx),%ecx
801013c6:	8b 15 cc c0 10 80    	mov    0x8010c0cc,%edx
    pDesc->limit_l       = Limit & 0xFFFF;
801013cc:	66 89 3a             	mov    %di,(%edx)
    pDesc->base_l        = Base & 0xFFFF;
801013cf:	66 89 4a 02          	mov    %cx,0x2(%edx)
    pDesc->base_m        = (Base >> 16) & 0x0FF;
801013d3:	89 ce                	mov    %ecx,%esi
801013d5:	c1 e9 10             	shr    $0x10,%ecx
801013d8:	88 4a 04             	mov    %cl,0x4(%edx)
    pDesc->base_h        = (Base >> 24) & 0x0FF;
801013db:	89 f1                	mov    %esi,%ecx
801013dd:	c1 e9 18             	shr    $0x18,%ecx
801013e0:	88 4a 07             	mov    %cl,0x7(%edx)
    pDesc->attr1         = Attr & 0xFF;
801013e3:	88 42 05             	mov    %al,0x5(%edx)
    pDesc->limit_h_attr2 = ((Limit >> 16) & 0x0F) | ((Attr >> 8) & 0xF0);
801013e6:	c1 ef 10             	shr    $0x10,%edi
801013e9:	89 f9                	mov    %edi,%ecx
801013eb:	83 e1 0f             	and    $0xf,%ecx
801013ee:	66 c1 e8 08          	shr    $0x8,%ax
801013f2:	83 e0 f0             	and    $0xfffffff0,%eax
801013f5:	09 c8                	or     %ecx,%eax
801013f7:	88 42 06             	mov    %al,0x6(%edx)
            _GDT_current++;
801013fa:	83 05 cc c0 10 80 08 	addl   $0x8,0x8010c0cc
    for (uint i = 0; i < size; i++)
80101401:	83 c3 0c             	add    $0xc,%ebx
80101404:	83 c5 08             	add    $0x8,%ebp
80101407:	66 83 fd 28          	cmp    $0x28,%bp
8010140b:	74 6f                	je     8010147c <core_init_gdt+0x13e>
        switch (KERN_GDT[i][2]) {
8010140d:	8b 43 08             	mov    0x8(%ebx),%eax
80101410:	3d 82 00 00 00       	cmp    $0x82,%eax
80101415:	74 8d                	je     801013a4 <core_init_gdt+0x66>
80101417:	3d 89 00 00 00       	cmp    $0x89,%eax
8010141c:	75 a3                	jne    801013c1 <core_init_gdt+0x83>
            memset(tss, 0, sizeof(struct tss));
8010141e:	83 ec 04             	sub    $0x4,%esp
80101421:	6a 68                	push   $0x68
80101423:	6a 00                	push   $0x0
80101425:	ff 74 24 44          	pushl  0x44(%esp)
80101429:	e8 af 0f 00 00       	call   801023dd <memset>
            tss->ss0 = 2 << 3; // data selector, hard code is bad
8010142e:	8b 44 24 48          	mov    0x48(%esp),%eax
80101432:	c7 40 08 10 00 00 00 	movl   $0x10,0x8(%eax)
            make_descriptor(_GDT_current, (uint)(KV2P(tss)),
80101439:	a1 cc c0 10 80       	mov    0x8010c0cc,%eax
    pDesc->limit_l       = Limit & 0xFFFF;
8010143e:	66 c7 00 67 00       	movw   $0x67,(%eax)
    pDesc->base_l        = Base & 0xFFFF;
80101443:	0f b7 7c 24 1c       	movzwl 0x1c(%esp),%edi
80101448:	66 89 78 02          	mov    %di,0x2(%eax)
    pDesc->base_m        = (Base >> 16) & 0x0FF;
8010144c:	0f b6 4c 24 17       	movzbl 0x17(%esp),%ecx
80101451:	88 48 04             	mov    %cl,0x4(%eax)
    pDesc->base_h        = (Base >> 24) & 0x0FF;
80101454:	0f b6 4c 24 18       	movzbl 0x18(%esp),%ecx
80101459:	88 48 07             	mov    %cl,0x7(%eax)
    pDesc->attr1         = Attr & 0xFF;
8010145c:	c6 40 05 89          	movb   $0x89,0x5(%eax)
    pDesc->limit_h_attr2 = ((Limit >> 16) & 0x0F) | ((Attr >> 8) & 0xF0);
80101460:	c6 40 06 00          	movb   $0x0,0x6(%eax)
            _GDT_current++;
80101464:	83 05 cc c0 10 80 08 	addl   $0x8,0x8010c0cc
            break;
8010146b:	83 c4 10             	add    $0x10,%esp
            selector_tss = i << 3;
8010146e:	66 89 6c 24 04       	mov    %bp,0x4(%esp)
            has_tss      = TRUE;
80101473:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
            break;
8010147a:	eb 85                	jmp    80101401 <core_init_gdt+0xc3>
    __lgdt__(has_tss, selector_tss);
8010147c:	83 ec 08             	sub    $0x8,%esp
8010147f:	0f b7 44 24 0c       	movzwl 0xc(%esp),%eax
80101484:	50                   	push   %eax
80101485:	ff 74 24 0c          	pushl  0xc(%esp)
80101489:	e8 5f fe ff ff       	call   801012ed <__lgdt__>
8010148e:	83 c4 10             	add    $0x10,%esp
80101491:	e9 23 ff ff ff       	jmp    801013b9 <core_init_gdt+0x7b>
80101496:	66 90                	xchg   %ax,%ax
80101498:	66 90                	xchg   %ax,%ax
8010149a:	66 90                	xchg   %ax,%ax
8010149c:	66 90                	xchg   %ax,%ax
8010149e:	66 90                	xchg   %ax,%ax

801014a0 <vector0>:
801014a0:	6a 00                	push   $0x0
801014a2:	6a 00                	push   $0x0
801014a4:	e9 b7 0a 00 00       	jmp    80101f60 <vector_handler>

801014a9 <vector1>:
801014a9:	6a 00                	push   $0x0
801014ab:	6a 01                	push   $0x1
801014ad:	e9 ae 0a 00 00       	jmp    80101f60 <vector_handler>

801014b2 <vector2>:
801014b2:	6a 00                	push   $0x0
801014b4:	6a 02                	push   $0x2
801014b6:	e9 a5 0a 00 00       	jmp    80101f60 <vector_handler>

801014bb <vector3>:
801014bb:	6a 00                	push   $0x0
801014bd:	6a 03                	push   $0x3
801014bf:	e9 9c 0a 00 00       	jmp    80101f60 <vector_handler>

801014c4 <vector4>:
801014c4:	6a 00                	push   $0x0
801014c6:	6a 04                	push   $0x4
801014c8:	e9 93 0a 00 00       	jmp    80101f60 <vector_handler>

801014cd <vector5>:
801014cd:	6a 00                	push   $0x0
801014cf:	6a 05                	push   $0x5
801014d1:	e9 8a 0a 00 00       	jmp    80101f60 <vector_handler>

801014d6 <vector6>:
801014d6:	6a 00                	push   $0x0
801014d8:	6a 06                	push   $0x6
801014da:	e9 81 0a 00 00       	jmp    80101f60 <vector_handler>

801014df <vector7>:
801014df:	6a 00                	push   $0x0
801014e1:	6a 07                	push   $0x7
801014e3:	e9 78 0a 00 00       	jmp    80101f60 <vector_handler>

801014e8 <vector8>:
801014e8:	6a 08                	push   $0x8
801014ea:	e9 71 0a 00 00       	jmp    80101f60 <vector_handler>

801014ef <vector9>:
801014ef:	6a 00                	push   $0x0
801014f1:	6a 09                	push   $0x9
801014f3:	e9 68 0a 00 00       	jmp    80101f60 <vector_handler>

801014f8 <vector10>:
801014f8:	6a 0a                	push   $0xa
801014fa:	e9 61 0a 00 00       	jmp    80101f60 <vector_handler>

801014ff <vector11>:
801014ff:	6a 0b                	push   $0xb
80101501:	e9 5a 0a 00 00       	jmp    80101f60 <vector_handler>

80101506 <vector12>:
80101506:	6a 0c                	push   $0xc
80101508:	e9 53 0a 00 00       	jmp    80101f60 <vector_handler>

8010150d <vector13>:
8010150d:	6a 0d                	push   $0xd
8010150f:	e9 4c 0a 00 00       	jmp    80101f60 <vector_handler>

80101514 <vector14>:
80101514:	6a 0e                	push   $0xe
80101516:	e9 45 0a 00 00       	jmp    80101f60 <vector_handler>

8010151b <vector15>:
8010151b:	6a 00                	push   $0x0
8010151d:	6a 0f                	push   $0xf
8010151f:	e9 3c 0a 00 00       	jmp    80101f60 <vector_handler>

80101524 <vector16>:
80101524:	6a 00                	push   $0x0
80101526:	6a 10                	push   $0x10
80101528:	e9 33 0a 00 00       	jmp    80101f60 <vector_handler>

8010152d <vector17>:
8010152d:	6a 11                	push   $0x11
8010152f:	e9 2c 0a 00 00       	jmp    80101f60 <vector_handler>

80101534 <vector18>:
80101534:	6a 00                	push   $0x0
80101536:	6a 12                	push   $0x12
80101538:	e9 23 0a 00 00       	jmp    80101f60 <vector_handler>

8010153d <vector19>:
8010153d:	6a 00                	push   $0x0
8010153f:	6a 13                	push   $0x13
80101541:	e9 1a 0a 00 00       	jmp    80101f60 <vector_handler>

80101546 <vector20>:
80101546:	6a 00                	push   $0x0
80101548:	6a 14                	push   $0x14
8010154a:	e9 11 0a 00 00       	jmp    80101f60 <vector_handler>

8010154f <vector21>:
8010154f:	6a 00                	push   $0x0
80101551:	6a 15                	push   $0x15
80101553:	e9 08 0a 00 00       	jmp    80101f60 <vector_handler>

80101558 <vector22>:
80101558:	6a 00                	push   $0x0
8010155a:	6a 16                	push   $0x16
8010155c:	e9 ff 09 00 00       	jmp    80101f60 <vector_handler>

80101561 <vector23>:
80101561:	6a 00                	push   $0x0
80101563:	6a 17                	push   $0x17
80101565:	e9 f6 09 00 00       	jmp    80101f60 <vector_handler>

8010156a <vector24>:
8010156a:	6a 00                	push   $0x0
8010156c:	6a 18                	push   $0x18
8010156e:	e9 ed 09 00 00       	jmp    80101f60 <vector_handler>

80101573 <vector25>:
80101573:	6a 00                	push   $0x0
80101575:	6a 19                	push   $0x19
80101577:	e9 e4 09 00 00       	jmp    80101f60 <vector_handler>

8010157c <vector26>:
8010157c:	6a 00                	push   $0x0
8010157e:	6a 1a                	push   $0x1a
80101580:	e9 db 09 00 00       	jmp    80101f60 <vector_handler>

80101585 <vector27>:
80101585:	6a 00                	push   $0x0
80101587:	6a 1b                	push   $0x1b
80101589:	e9 d2 09 00 00       	jmp    80101f60 <vector_handler>

8010158e <vector28>:
8010158e:	6a 00                	push   $0x0
80101590:	6a 1c                	push   $0x1c
80101592:	e9 c9 09 00 00       	jmp    80101f60 <vector_handler>

80101597 <vector29>:
80101597:	6a 00                	push   $0x0
80101599:	6a 1d                	push   $0x1d
8010159b:	e9 c0 09 00 00       	jmp    80101f60 <vector_handler>

801015a0 <vector30>:
801015a0:	6a 00                	push   $0x0
801015a2:	6a 1e                	push   $0x1e
801015a4:	e9 b7 09 00 00       	jmp    80101f60 <vector_handler>

801015a9 <vector31>:
801015a9:	6a 00                	push   $0x0
801015ab:	6a 1f                	push   $0x1f
801015ad:	e9 ae 09 00 00       	jmp    80101f60 <vector_handler>

801015b2 <vector32>:
801015b2:	6a 00                	push   $0x0
801015b4:	6a 20                	push   $0x20
801015b6:	e9 a5 09 00 00       	jmp    80101f60 <vector_handler>

801015bb <vector33>:
801015bb:	6a 00                	push   $0x0
801015bd:	6a 21                	push   $0x21
801015bf:	e9 9c 09 00 00       	jmp    80101f60 <vector_handler>

801015c4 <vector34>:
801015c4:	6a 00                	push   $0x0
801015c6:	6a 22                	push   $0x22
801015c8:	e9 93 09 00 00       	jmp    80101f60 <vector_handler>

801015cd <vector35>:
801015cd:	6a 00                	push   $0x0
801015cf:	6a 23                	push   $0x23
801015d1:	e9 8a 09 00 00       	jmp    80101f60 <vector_handler>

801015d6 <vector36>:
801015d6:	6a 00                	push   $0x0
801015d8:	6a 24                	push   $0x24
801015da:	e9 81 09 00 00       	jmp    80101f60 <vector_handler>

801015df <vector37>:
801015df:	6a 00                	push   $0x0
801015e1:	6a 25                	push   $0x25
801015e3:	e9 78 09 00 00       	jmp    80101f60 <vector_handler>

801015e8 <vector38>:
801015e8:	6a 00                	push   $0x0
801015ea:	6a 26                	push   $0x26
801015ec:	e9 6f 09 00 00       	jmp    80101f60 <vector_handler>

801015f1 <vector39>:
801015f1:	6a 00                	push   $0x0
801015f3:	6a 27                	push   $0x27
801015f5:	e9 66 09 00 00       	jmp    80101f60 <vector_handler>

801015fa <vector40>:
801015fa:	6a 00                	push   $0x0
801015fc:	6a 28                	push   $0x28
801015fe:	e9 5d 09 00 00       	jmp    80101f60 <vector_handler>

80101603 <vector41>:
80101603:	6a 00                	push   $0x0
80101605:	6a 29                	push   $0x29
80101607:	e9 54 09 00 00       	jmp    80101f60 <vector_handler>

8010160c <vector42>:
8010160c:	6a 00                	push   $0x0
8010160e:	6a 2a                	push   $0x2a
80101610:	e9 4b 09 00 00       	jmp    80101f60 <vector_handler>

80101615 <vector43>:
80101615:	6a 00                	push   $0x0
80101617:	6a 2b                	push   $0x2b
80101619:	e9 42 09 00 00       	jmp    80101f60 <vector_handler>

8010161e <vector44>:
8010161e:	6a 00                	push   $0x0
80101620:	6a 2c                	push   $0x2c
80101622:	e9 39 09 00 00       	jmp    80101f60 <vector_handler>

80101627 <vector45>:
80101627:	6a 00                	push   $0x0
80101629:	6a 2d                	push   $0x2d
8010162b:	e9 30 09 00 00       	jmp    80101f60 <vector_handler>

80101630 <vector46>:
80101630:	6a 00                	push   $0x0
80101632:	6a 2e                	push   $0x2e
80101634:	e9 27 09 00 00       	jmp    80101f60 <vector_handler>

80101639 <vector47>:
80101639:	6a 00                	push   $0x0
8010163b:	6a 2f                	push   $0x2f
8010163d:	e9 1e 09 00 00       	jmp    80101f60 <vector_handler>

80101642 <vector48>:
80101642:	6a 00                	push   $0x0
80101644:	6a 30                	push   $0x30
80101646:	e9 15 09 00 00       	jmp    80101f60 <vector_handler>

8010164b <vector49>:
8010164b:	6a 00                	push   $0x0
8010164d:	6a 31                	push   $0x31
8010164f:	e9 0c 09 00 00       	jmp    80101f60 <vector_handler>

80101654 <vector50>:
80101654:	6a 00                	push   $0x0
80101656:	6a 32                	push   $0x32
80101658:	e9 03 09 00 00       	jmp    80101f60 <vector_handler>

8010165d <vector51>:
8010165d:	6a 00                	push   $0x0
8010165f:	6a 33                	push   $0x33
80101661:	e9 fa 08 00 00       	jmp    80101f60 <vector_handler>

80101666 <vector52>:
80101666:	6a 00                	push   $0x0
80101668:	6a 34                	push   $0x34
8010166a:	e9 f1 08 00 00       	jmp    80101f60 <vector_handler>

8010166f <vector53>:
8010166f:	6a 00                	push   $0x0
80101671:	6a 35                	push   $0x35
80101673:	e9 e8 08 00 00       	jmp    80101f60 <vector_handler>

80101678 <vector54>:
80101678:	6a 00                	push   $0x0
8010167a:	6a 36                	push   $0x36
8010167c:	e9 df 08 00 00       	jmp    80101f60 <vector_handler>

80101681 <vector55>:
80101681:	6a 00                	push   $0x0
80101683:	6a 37                	push   $0x37
80101685:	e9 d6 08 00 00       	jmp    80101f60 <vector_handler>

8010168a <vector56>:
8010168a:	6a 00                	push   $0x0
8010168c:	6a 38                	push   $0x38
8010168e:	e9 cd 08 00 00       	jmp    80101f60 <vector_handler>

80101693 <vector57>:
80101693:	6a 00                	push   $0x0
80101695:	6a 39                	push   $0x39
80101697:	e9 c4 08 00 00       	jmp    80101f60 <vector_handler>

8010169c <vector58>:
8010169c:	6a 00                	push   $0x0
8010169e:	6a 3a                	push   $0x3a
801016a0:	e9 bb 08 00 00       	jmp    80101f60 <vector_handler>

801016a5 <vector59>:
801016a5:	6a 00                	push   $0x0
801016a7:	6a 3b                	push   $0x3b
801016a9:	e9 b2 08 00 00       	jmp    80101f60 <vector_handler>

801016ae <vector60>:
801016ae:	6a 00                	push   $0x0
801016b0:	6a 3c                	push   $0x3c
801016b2:	e9 a9 08 00 00       	jmp    80101f60 <vector_handler>

801016b7 <vector61>:
801016b7:	6a 00                	push   $0x0
801016b9:	6a 3d                	push   $0x3d
801016bb:	e9 a0 08 00 00       	jmp    80101f60 <vector_handler>

801016c0 <vector62>:
801016c0:	6a 00                	push   $0x0
801016c2:	6a 3e                	push   $0x3e
801016c4:	e9 97 08 00 00       	jmp    80101f60 <vector_handler>

801016c9 <vector63>:
801016c9:	6a 00                	push   $0x0
801016cb:	6a 3f                	push   $0x3f
801016cd:	e9 8e 08 00 00       	jmp    80101f60 <vector_handler>

801016d2 <vector64>:
801016d2:	6a 00                	push   $0x0
801016d4:	6a 40                	push   $0x40
801016d6:	e9 85 08 00 00       	jmp    80101f60 <vector_handler>

801016db <vector65>:
801016db:	6a 00                	push   $0x0
801016dd:	6a 41                	push   $0x41
801016df:	e9 7c 08 00 00       	jmp    80101f60 <vector_handler>

801016e4 <vector66>:
801016e4:	6a 00                	push   $0x0
801016e6:	6a 42                	push   $0x42
801016e8:	e9 73 08 00 00       	jmp    80101f60 <vector_handler>

801016ed <vector67>:
801016ed:	6a 00                	push   $0x0
801016ef:	6a 43                	push   $0x43
801016f1:	e9 6a 08 00 00       	jmp    80101f60 <vector_handler>

801016f6 <vector68>:
801016f6:	6a 00                	push   $0x0
801016f8:	6a 44                	push   $0x44
801016fa:	e9 61 08 00 00       	jmp    80101f60 <vector_handler>

801016ff <vector69>:
801016ff:	6a 00                	push   $0x0
80101701:	6a 45                	push   $0x45
80101703:	e9 58 08 00 00       	jmp    80101f60 <vector_handler>

80101708 <vector70>:
80101708:	6a 00                	push   $0x0
8010170a:	6a 46                	push   $0x46
8010170c:	e9 4f 08 00 00       	jmp    80101f60 <vector_handler>

80101711 <vector71>:
80101711:	6a 00                	push   $0x0
80101713:	6a 47                	push   $0x47
80101715:	e9 46 08 00 00       	jmp    80101f60 <vector_handler>

8010171a <vector72>:
8010171a:	6a 00                	push   $0x0
8010171c:	6a 48                	push   $0x48
8010171e:	e9 3d 08 00 00       	jmp    80101f60 <vector_handler>

80101723 <vector73>:
80101723:	6a 00                	push   $0x0
80101725:	6a 49                	push   $0x49
80101727:	e9 34 08 00 00       	jmp    80101f60 <vector_handler>

8010172c <vector74>:
8010172c:	6a 00                	push   $0x0
8010172e:	6a 4a                	push   $0x4a
80101730:	e9 2b 08 00 00       	jmp    80101f60 <vector_handler>

80101735 <vector75>:
80101735:	6a 00                	push   $0x0
80101737:	6a 4b                	push   $0x4b
80101739:	e9 22 08 00 00       	jmp    80101f60 <vector_handler>

8010173e <vector76>:
8010173e:	6a 00                	push   $0x0
80101740:	6a 4c                	push   $0x4c
80101742:	e9 19 08 00 00       	jmp    80101f60 <vector_handler>

80101747 <vector77>:
80101747:	6a 00                	push   $0x0
80101749:	6a 4d                	push   $0x4d
8010174b:	e9 10 08 00 00       	jmp    80101f60 <vector_handler>

80101750 <vector78>:
80101750:	6a 00                	push   $0x0
80101752:	6a 4e                	push   $0x4e
80101754:	e9 07 08 00 00       	jmp    80101f60 <vector_handler>

80101759 <vector79>:
80101759:	6a 00                	push   $0x0
8010175b:	6a 4f                	push   $0x4f
8010175d:	e9 fe 07 00 00       	jmp    80101f60 <vector_handler>

80101762 <vector80>:
80101762:	6a 00                	push   $0x0
80101764:	6a 50                	push   $0x50
80101766:	e9 f5 07 00 00       	jmp    80101f60 <vector_handler>

8010176b <vector81>:
8010176b:	6a 00                	push   $0x0
8010176d:	6a 51                	push   $0x51
8010176f:	e9 ec 07 00 00       	jmp    80101f60 <vector_handler>

80101774 <vector82>:
80101774:	6a 00                	push   $0x0
80101776:	6a 52                	push   $0x52
80101778:	e9 e3 07 00 00       	jmp    80101f60 <vector_handler>

8010177d <vector83>:
8010177d:	6a 00                	push   $0x0
8010177f:	6a 53                	push   $0x53
80101781:	e9 da 07 00 00       	jmp    80101f60 <vector_handler>

80101786 <vector84>:
80101786:	6a 00                	push   $0x0
80101788:	6a 54                	push   $0x54
8010178a:	e9 d1 07 00 00       	jmp    80101f60 <vector_handler>

8010178f <vector85>:
8010178f:	6a 00                	push   $0x0
80101791:	6a 55                	push   $0x55
80101793:	e9 c8 07 00 00       	jmp    80101f60 <vector_handler>

80101798 <vector86>:
80101798:	6a 00                	push   $0x0
8010179a:	6a 56                	push   $0x56
8010179c:	e9 bf 07 00 00       	jmp    80101f60 <vector_handler>

801017a1 <vector87>:
801017a1:	6a 00                	push   $0x0
801017a3:	6a 57                	push   $0x57
801017a5:	e9 b6 07 00 00       	jmp    80101f60 <vector_handler>

801017aa <vector88>:
801017aa:	6a 00                	push   $0x0
801017ac:	6a 58                	push   $0x58
801017ae:	e9 ad 07 00 00       	jmp    80101f60 <vector_handler>

801017b3 <vector89>:
801017b3:	6a 00                	push   $0x0
801017b5:	6a 59                	push   $0x59
801017b7:	e9 a4 07 00 00       	jmp    80101f60 <vector_handler>

801017bc <vector90>:
801017bc:	6a 00                	push   $0x0
801017be:	6a 5a                	push   $0x5a
801017c0:	e9 9b 07 00 00       	jmp    80101f60 <vector_handler>

801017c5 <vector91>:
801017c5:	6a 00                	push   $0x0
801017c7:	6a 5b                	push   $0x5b
801017c9:	e9 92 07 00 00       	jmp    80101f60 <vector_handler>

801017ce <vector92>:
801017ce:	6a 00                	push   $0x0
801017d0:	6a 5c                	push   $0x5c
801017d2:	e9 89 07 00 00       	jmp    80101f60 <vector_handler>

801017d7 <vector93>:
801017d7:	6a 00                	push   $0x0
801017d9:	6a 5d                	push   $0x5d
801017db:	e9 80 07 00 00       	jmp    80101f60 <vector_handler>

801017e0 <vector94>:
801017e0:	6a 00                	push   $0x0
801017e2:	6a 5e                	push   $0x5e
801017e4:	e9 77 07 00 00       	jmp    80101f60 <vector_handler>

801017e9 <vector95>:
801017e9:	6a 00                	push   $0x0
801017eb:	6a 5f                	push   $0x5f
801017ed:	e9 6e 07 00 00       	jmp    80101f60 <vector_handler>

801017f2 <vector96>:
801017f2:	6a 00                	push   $0x0
801017f4:	6a 60                	push   $0x60
801017f6:	e9 65 07 00 00       	jmp    80101f60 <vector_handler>

801017fb <vector97>:
801017fb:	6a 00                	push   $0x0
801017fd:	6a 61                	push   $0x61
801017ff:	e9 5c 07 00 00       	jmp    80101f60 <vector_handler>

80101804 <vector98>:
80101804:	6a 00                	push   $0x0
80101806:	6a 62                	push   $0x62
80101808:	e9 53 07 00 00       	jmp    80101f60 <vector_handler>

8010180d <vector99>:
8010180d:	6a 00                	push   $0x0
8010180f:	6a 63                	push   $0x63
80101811:	e9 4a 07 00 00       	jmp    80101f60 <vector_handler>

80101816 <vector100>:
80101816:	6a 00                	push   $0x0
80101818:	6a 64                	push   $0x64
8010181a:	e9 41 07 00 00       	jmp    80101f60 <vector_handler>

8010181f <vector101>:
8010181f:	6a 00                	push   $0x0
80101821:	6a 65                	push   $0x65
80101823:	e9 38 07 00 00       	jmp    80101f60 <vector_handler>

80101828 <vector102>:
80101828:	6a 00                	push   $0x0
8010182a:	6a 66                	push   $0x66
8010182c:	e9 2f 07 00 00       	jmp    80101f60 <vector_handler>

80101831 <vector103>:
80101831:	6a 00                	push   $0x0
80101833:	6a 67                	push   $0x67
80101835:	e9 26 07 00 00       	jmp    80101f60 <vector_handler>

8010183a <vector104>:
8010183a:	6a 00                	push   $0x0
8010183c:	6a 68                	push   $0x68
8010183e:	e9 1d 07 00 00       	jmp    80101f60 <vector_handler>

80101843 <vector105>:
80101843:	6a 00                	push   $0x0
80101845:	6a 69                	push   $0x69
80101847:	e9 14 07 00 00       	jmp    80101f60 <vector_handler>

8010184c <vector106>:
8010184c:	6a 00                	push   $0x0
8010184e:	6a 6a                	push   $0x6a
80101850:	e9 0b 07 00 00       	jmp    80101f60 <vector_handler>

80101855 <vector107>:
80101855:	6a 00                	push   $0x0
80101857:	6a 6b                	push   $0x6b
80101859:	e9 02 07 00 00       	jmp    80101f60 <vector_handler>

8010185e <vector108>:
8010185e:	6a 00                	push   $0x0
80101860:	6a 6c                	push   $0x6c
80101862:	e9 f9 06 00 00       	jmp    80101f60 <vector_handler>

80101867 <vector109>:
80101867:	6a 00                	push   $0x0
80101869:	6a 6d                	push   $0x6d
8010186b:	e9 f0 06 00 00       	jmp    80101f60 <vector_handler>

80101870 <vector110>:
80101870:	6a 00                	push   $0x0
80101872:	6a 6e                	push   $0x6e
80101874:	e9 e7 06 00 00       	jmp    80101f60 <vector_handler>

80101879 <vector111>:
80101879:	6a 00                	push   $0x0
8010187b:	6a 6f                	push   $0x6f
8010187d:	e9 de 06 00 00       	jmp    80101f60 <vector_handler>

80101882 <vector112>:
80101882:	6a 00                	push   $0x0
80101884:	6a 70                	push   $0x70
80101886:	e9 d5 06 00 00       	jmp    80101f60 <vector_handler>

8010188b <vector113>:
8010188b:	6a 00                	push   $0x0
8010188d:	6a 71                	push   $0x71
8010188f:	e9 cc 06 00 00       	jmp    80101f60 <vector_handler>

80101894 <vector114>:
80101894:	6a 00                	push   $0x0
80101896:	6a 72                	push   $0x72
80101898:	e9 c3 06 00 00       	jmp    80101f60 <vector_handler>

8010189d <vector115>:
8010189d:	6a 00                	push   $0x0
8010189f:	6a 73                	push   $0x73
801018a1:	e9 ba 06 00 00       	jmp    80101f60 <vector_handler>

801018a6 <vector116>:
801018a6:	6a 00                	push   $0x0
801018a8:	6a 74                	push   $0x74
801018aa:	e9 b1 06 00 00       	jmp    80101f60 <vector_handler>

801018af <vector117>:
801018af:	6a 00                	push   $0x0
801018b1:	6a 75                	push   $0x75
801018b3:	e9 a8 06 00 00       	jmp    80101f60 <vector_handler>

801018b8 <vector118>:
801018b8:	6a 00                	push   $0x0
801018ba:	6a 76                	push   $0x76
801018bc:	e9 9f 06 00 00       	jmp    80101f60 <vector_handler>

801018c1 <vector119>:
801018c1:	6a 00                	push   $0x0
801018c3:	6a 77                	push   $0x77
801018c5:	e9 96 06 00 00       	jmp    80101f60 <vector_handler>

801018ca <vector120>:
801018ca:	6a 00                	push   $0x0
801018cc:	6a 78                	push   $0x78
801018ce:	e9 8d 06 00 00       	jmp    80101f60 <vector_handler>

801018d3 <vector121>:
801018d3:	6a 00                	push   $0x0
801018d5:	6a 79                	push   $0x79
801018d7:	e9 84 06 00 00       	jmp    80101f60 <vector_handler>

801018dc <vector122>:
801018dc:	6a 00                	push   $0x0
801018de:	6a 7a                	push   $0x7a
801018e0:	e9 7b 06 00 00       	jmp    80101f60 <vector_handler>

801018e5 <vector123>:
801018e5:	6a 00                	push   $0x0
801018e7:	6a 7b                	push   $0x7b
801018e9:	e9 72 06 00 00       	jmp    80101f60 <vector_handler>

801018ee <vector124>:
801018ee:	6a 00                	push   $0x0
801018f0:	6a 7c                	push   $0x7c
801018f2:	e9 69 06 00 00       	jmp    80101f60 <vector_handler>

801018f7 <vector125>:
801018f7:	6a 00                	push   $0x0
801018f9:	6a 7d                	push   $0x7d
801018fb:	e9 60 06 00 00       	jmp    80101f60 <vector_handler>

80101900 <vector126>:
80101900:	6a 00                	push   $0x0
80101902:	6a 7e                	push   $0x7e
80101904:	e9 57 06 00 00       	jmp    80101f60 <vector_handler>

80101909 <vector127>:
80101909:	6a 00                	push   $0x0
8010190b:	6a 7f                	push   $0x7f
8010190d:	e9 4e 06 00 00       	jmp    80101f60 <vector_handler>

80101912 <vector128>:
80101912:	6a 00                	push   $0x0
80101914:	68 80 00 00 00       	push   $0x80
80101919:	e9 42 06 00 00       	jmp    80101f60 <vector_handler>

8010191e <vector129>:
8010191e:	6a 00                	push   $0x0
80101920:	68 81 00 00 00       	push   $0x81
80101925:	e9 36 06 00 00       	jmp    80101f60 <vector_handler>

8010192a <vector130>:
8010192a:	6a 00                	push   $0x0
8010192c:	68 82 00 00 00       	push   $0x82
80101931:	e9 2a 06 00 00       	jmp    80101f60 <vector_handler>

80101936 <vector131>:
80101936:	6a 00                	push   $0x0
80101938:	68 83 00 00 00       	push   $0x83
8010193d:	e9 1e 06 00 00       	jmp    80101f60 <vector_handler>

80101942 <vector132>:
80101942:	6a 00                	push   $0x0
80101944:	68 84 00 00 00       	push   $0x84
80101949:	e9 12 06 00 00       	jmp    80101f60 <vector_handler>

8010194e <vector133>:
8010194e:	6a 00                	push   $0x0
80101950:	68 85 00 00 00       	push   $0x85
80101955:	e9 06 06 00 00       	jmp    80101f60 <vector_handler>

8010195a <vector134>:
8010195a:	6a 00                	push   $0x0
8010195c:	68 86 00 00 00       	push   $0x86
80101961:	e9 fa 05 00 00       	jmp    80101f60 <vector_handler>

80101966 <vector135>:
80101966:	6a 00                	push   $0x0
80101968:	68 87 00 00 00       	push   $0x87
8010196d:	e9 ee 05 00 00       	jmp    80101f60 <vector_handler>

80101972 <vector136>:
80101972:	6a 00                	push   $0x0
80101974:	68 88 00 00 00       	push   $0x88
80101979:	e9 e2 05 00 00       	jmp    80101f60 <vector_handler>

8010197e <vector137>:
8010197e:	6a 00                	push   $0x0
80101980:	68 89 00 00 00       	push   $0x89
80101985:	e9 d6 05 00 00       	jmp    80101f60 <vector_handler>

8010198a <vector138>:
8010198a:	6a 00                	push   $0x0
8010198c:	68 8a 00 00 00       	push   $0x8a
80101991:	e9 ca 05 00 00       	jmp    80101f60 <vector_handler>

80101996 <vector139>:
80101996:	6a 00                	push   $0x0
80101998:	68 8b 00 00 00       	push   $0x8b
8010199d:	e9 be 05 00 00       	jmp    80101f60 <vector_handler>

801019a2 <vector140>:
801019a2:	6a 00                	push   $0x0
801019a4:	68 8c 00 00 00       	push   $0x8c
801019a9:	e9 b2 05 00 00       	jmp    80101f60 <vector_handler>

801019ae <vector141>:
801019ae:	6a 00                	push   $0x0
801019b0:	68 8d 00 00 00       	push   $0x8d
801019b5:	e9 a6 05 00 00       	jmp    80101f60 <vector_handler>

801019ba <vector142>:
801019ba:	6a 00                	push   $0x0
801019bc:	68 8e 00 00 00       	push   $0x8e
801019c1:	e9 9a 05 00 00       	jmp    80101f60 <vector_handler>

801019c6 <vector143>:
801019c6:	6a 00                	push   $0x0
801019c8:	68 8f 00 00 00       	push   $0x8f
801019cd:	e9 8e 05 00 00       	jmp    80101f60 <vector_handler>

801019d2 <vector144>:
801019d2:	6a 00                	push   $0x0
801019d4:	68 90 00 00 00       	push   $0x90
801019d9:	e9 82 05 00 00       	jmp    80101f60 <vector_handler>

801019de <vector145>:
801019de:	6a 00                	push   $0x0
801019e0:	68 91 00 00 00       	push   $0x91
801019e5:	e9 76 05 00 00       	jmp    80101f60 <vector_handler>

801019ea <vector146>:
801019ea:	6a 00                	push   $0x0
801019ec:	68 92 00 00 00       	push   $0x92
801019f1:	e9 6a 05 00 00       	jmp    80101f60 <vector_handler>

801019f6 <vector147>:
801019f6:	6a 00                	push   $0x0
801019f8:	68 93 00 00 00       	push   $0x93
801019fd:	e9 5e 05 00 00       	jmp    80101f60 <vector_handler>

80101a02 <vector148>:
80101a02:	6a 00                	push   $0x0
80101a04:	68 94 00 00 00       	push   $0x94
80101a09:	e9 52 05 00 00       	jmp    80101f60 <vector_handler>

80101a0e <vector149>:
80101a0e:	6a 00                	push   $0x0
80101a10:	68 95 00 00 00       	push   $0x95
80101a15:	e9 46 05 00 00       	jmp    80101f60 <vector_handler>

80101a1a <vector150>:
80101a1a:	6a 00                	push   $0x0
80101a1c:	68 96 00 00 00       	push   $0x96
80101a21:	e9 3a 05 00 00       	jmp    80101f60 <vector_handler>

80101a26 <vector151>:
80101a26:	6a 00                	push   $0x0
80101a28:	68 97 00 00 00       	push   $0x97
80101a2d:	e9 2e 05 00 00       	jmp    80101f60 <vector_handler>

80101a32 <vector152>:
80101a32:	6a 00                	push   $0x0
80101a34:	68 98 00 00 00       	push   $0x98
80101a39:	e9 22 05 00 00       	jmp    80101f60 <vector_handler>

80101a3e <vector153>:
80101a3e:	6a 00                	push   $0x0
80101a40:	68 99 00 00 00       	push   $0x99
80101a45:	e9 16 05 00 00       	jmp    80101f60 <vector_handler>

80101a4a <vector154>:
80101a4a:	6a 00                	push   $0x0
80101a4c:	68 9a 00 00 00       	push   $0x9a
80101a51:	e9 0a 05 00 00       	jmp    80101f60 <vector_handler>

80101a56 <vector155>:
80101a56:	6a 00                	push   $0x0
80101a58:	68 9b 00 00 00       	push   $0x9b
80101a5d:	e9 fe 04 00 00       	jmp    80101f60 <vector_handler>

80101a62 <vector156>:
80101a62:	6a 00                	push   $0x0
80101a64:	68 9c 00 00 00       	push   $0x9c
80101a69:	e9 f2 04 00 00       	jmp    80101f60 <vector_handler>

80101a6e <vector157>:
80101a6e:	6a 00                	push   $0x0
80101a70:	68 9d 00 00 00       	push   $0x9d
80101a75:	e9 e6 04 00 00       	jmp    80101f60 <vector_handler>

80101a7a <vector158>:
80101a7a:	6a 00                	push   $0x0
80101a7c:	68 9e 00 00 00       	push   $0x9e
80101a81:	e9 da 04 00 00       	jmp    80101f60 <vector_handler>

80101a86 <vector159>:
80101a86:	6a 00                	push   $0x0
80101a88:	68 9f 00 00 00       	push   $0x9f
80101a8d:	e9 ce 04 00 00       	jmp    80101f60 <vector_handler>

80101a92 <vector160>:
80101a92:	6a 00                	push   $0x0
80101a94:	68 a0 00 00 00       	push   $0xa0
80101a99:	e9 c2 04 00 00       	jmp    80101f60 <vector_handler>

80101a9e <vector161>:
80101a9e:	6a 00                	push   $0x0
80101aa0:	68 a1 00 00 00       	push   $0xa1
80101aa5:	e9 b6 04 00 00       	jmp    80101f60 <vector_handler>

80101aaa <vector162>:
80101aaa:	6a 00                	push   $0x0
80101aac:	68 a2 00 00 00       	push   $0xa2
80101ab1:	e9 aa 04 00 00       	jmp    80101f60 <vector_handler>

80101ab6 <vector163>:
80101ab6:	6a 00                	push   $0x0
80101ab8:	68 a3 00 00 00       	push   $0xa3
80101abd:	e9 9e 04 00 00       	jmp    80101f60 <vector_handler>

80101ac2 <vector164>:
80101ac2:	6a 00                	push   $0x0
80101ac4:	68 a4 00 00 00       	push   $0xa4
80101ac9:	e9 92 04 00 00       	jmp    80101f60 <vector_handler>

80101ace <vector165>:
80101ace:	6a 00                	push   $0x0
80101ad0:	68 a5 00 00 00       	push   $0xa5
80101ad5:	e9 86 04 00 00       	jmp    80101f60 <vector_handler>

80101ada <vector166>:
80101ada:	6a 00                	push   $0x0
80101adc:	68 a6 00 00 00       	push   $0xa6
80101ae1:	e9 7a 04 00 00       	jmp    80101f60 <vector_handler>

80101ae6 <vector167>:
80101ae6:	6a 00                	push   $0x0
80101ae8:	68 a7 00 00 00       	push   $0xa7
80101aed:	e9 6e 04 00 00       	jmp    80101f60 <vector_handler>

80101af2 <vector168>:
80101af2:	6a 00                	push   $0x0
80101af4:	68 a8 00 00 00       	push   $0xa8
80101af9:	e9 62 04 00 00       	jmp    80101f60 <vector_handler>

80101afe <vector169>:
80101afe:	6a 00                	push   $0x0
80101b00:	68 a9 00 00 00       	push   $0xa9
80101b05:	e9 56 04 00 00       	jmp    80101f60 <vector_handler>

80101b0a <vector170>:
80101b0a:	6a 00                	push   $0x0
80101b0c:	68 aa 00 00 00       	push   $0xaa
80101b11:	e9 4a 04 00 00       	jmp    80101f60 <vector_handler>

80101b16 <vector171>:
80101b16:	6a 00                	push   $0x0
80101b18:	68 ab 00 00 00       	push   $0xab
80101b1d:	e9 3e 04 00 00       	jmp    80101f60 <vector_handler>

80101b22 <vector172>:
80101b22:	6a 00                	push   $0x0
80101b24:	68 ac 00 00 00       	push   $0xac
80101b29:	e9 32 04 00 00       	jmp    80101f60 <vector_handler>

80101b2e <vector173>:
80101b2e:	6a 00                	push   $0x0
80101b30:	68 ad 00 00 00       	push   $0xad
80101b35:	e9 26 04 00 00       	jmp    80101f60 <vector_handler>

80101b3a <vector174>:
80101b3a:	6a 00                	push   $0x0
80101b3c:	68 ae 00 00 00       	push   $0xae
80101b41:	e9 1a 04 00 00       	jmp    80101f60 <vector_handler>

80101b46 <vector175>:
80101b46:	6a 00                	push   $0x0
80101b48:	68 af 00 00 00       	push   $0xaf
80101b4d:	e9 0e 04 00 00       	jmp    80101f60 <vector_handler>

80101b52 <vector176>:
80101b52:	6a 00                	push   $0x0
80101b54:	68 b0 00 00 00       	push   $0xb0
80101b59:	e9 02 04 00 00       	jmp    80101f60 <vector_handler>

80101b5e <vector177>:
80101b5e:	6a 00                	push   $0x0
80101b60:	68 b1 00 00 00       	push   $0xb1
80101b65:	e9 f6 03 00 00       	jmp    80101f60 <vector_handler>

80101b6a <vector178>:
80101b6a:	6a 00                	push   $0x0
80101b6c:	68 b2 00 00 00       	push   $0xb2
80101b71:	e9 ea 03 00 00       	jmp    80101f60 <vector_handler>

80101b76 <vector179>:
80101b76:	6a 00                	push   $0x0
80101b78:	68 b3 00 00 00       	push   $0xb3
80101b7d:	e9 de 03 00 00       	jmp    80101f60 <vector_handler>

80101b82 <vector180>:
80101b82:	6a 00                	push   $0x0
80101b84:	68 b4 00 00 00       	push   $0xb4
80101b89:	e9 d2 03 00 00       	jmp    80101f60 <vector_handler>

80101b8e <vector181>:
80101b8e:	6a 00                	push   $0x0
80101b90:	68 b5 00 00 00       	push   $0xb5
80101b95:	e9 c6 03 00 00       	jmp    80101f60 <vector_handler>

80101b9a <vector182>:
80101b9a:	6a 00                	push   $0x0
80101b9c:	68 b6 00 00 00       	push   $0xb6
80101ba1:	e9 ba 03 00 00       	jmp    80101f60 <vector_handler>

80101ba6 <vector183>:
80101ba6:	6a 00                	push   $0x0
80101ba8:	68 b7 00 00 00       	push   $0xb7
80101bad:	e9 ae 03 00 00       	jmp    80101f60 <vector_handler>

80101bb2 <vector184>:
80101bb2:	6a 00                	push   $0x0
80101bb4:	68 b8 00 00 00       	push   $0xb8
80101bb9:	e9 a2 03 00 00       	jmp    80101f60 <vector_handler>

80101bbe <vector185>:
80101bbe:	6a 00                	push   $0x0
80101bc0:	68 b9 00 00 00       	push   $0xb9
80101bc5:	e9 96 03 00 00       	jmp    80101f60 <vector_handler>

80101bca <vector186>:
80101bca:	6a 00                	push   $0x0
80101bcc:	68 ba 00 00 00       	push   $0xba
80101bd1:	e9 8a 03 00 00       	jmp    80101f60 <vector_handler>

80101bd6 <vector187>:
80101bd6:	6a 00                	push   $0x0
80101bd8:	68 bb 00 00 00       	push   $0xbb
80101bdd:	e9 7e 03 00 00       	jmp    80101f60 <vector_handler>

80101be2 <vector188>:
80101be2:	6a 00                	push   $0x0
80101be4:	68 bc 00 00 00       	push   $0xbc
80101be9:	e9 72 03 00 00       	jmp    80101f60 <vector_handler>

80101bee <vector189>:
80101bee:	6a 00                	push   $0x0
80101bf0:	68 bd 00 00 00       	push   $0xbd
80101bf5:	e9 66 03 00 00       	jmp    80101f60 <vector_handler>

80101bfa <vector190>:
80101bfa:	6a 00                	push   $0x0
80101bfc:	68 be 00 00 00       	push   $0xbe
80101c01:	e9 5a 03 00 00       	jmp    80101f60 <vector_handler>

80101c06 <vector191>:
80101c06:	6a 00                	push   $0x0
80101c08:	68 bf 00 00 00       	push   $0xbf
80101c0d:	e9 4e 03 00 00       	jmp    80101f60 <vector_handler>

80101c12 <vector192>:
80101c12:	6a 00                	push   $0x0
80101c14:	68 c0 00 00 00       	push   $0xc0
80101c19:	e9 42 03 00 00       	jmp    80101f60 <vector_handler>

80101c1e <vector193>:
80101c1e:	6a 00                	push   $0x0
80101c20:	68 c1 00 00 00       	push   $0xc1
80101c25:	e9 36 03 00 00       	jmp    80101f60 <vector_handler>

80101c2a <vector194>:
80101c2a:	6a 00                	push   $0x0
80101c2c:	68 c2 00 00 00       	push   $0xc2
80101c31:	e9 2a 03 00 00       	jmp    80101f60 <vector_handler>

80101c36 <vector195>:
80101c36:	6a 00                	push   $0x0
80101c38:	68 c3 00 00 00       	push   $0xc3
80101c3d:	e9 1e 03 00 00       	jmp    80101f60 <vector_handler>

80101c42 <vector196>:
80101c42:	6a 00                	push   $0x0
80101c44:	68 c4 00 00 00       	push   $0xc4
80101c49:	e9 12 03 00 00       	jmp    80101f60 <vector_handler>

80101c4e <vector197>:
80101c4e:	6a 00                	push   $0x0
80101c50:	68 c5 00 00 00       	push   $0xc5
80101c55:	e9 06 03 00 00       	jmp    80101f60 <vector_handler>

80101c5a <vector198>:
80101c5a:	6a 00                	push   $0x0
80101c5c:	68 c6 00 00 00       	push   $0xc6
80101c61:	e9 fa 02 00 00       	jmp    80101f60 <vector_handler>

80101c66 <vector199>:
80101c66:	6a 00                	push   $0x0
80101c68:	68 c7 00 00 00       	push   $0xc7
80101c6d:	e9 ee 02 00 00       	jmp    80101f60 <vector_handler>

80101c72 <vector200>:
80101c72:	6a 00                	push   $0x0
80101c74:	68 c8 00 00 00       	push   $0xc8
80101c79:	e9 e2 02 00 00       	jmp    80101f60 <vector_handler>

80101c7e <vector201>:
80101c7e:	6a 00                	push   $0x0
80101c80:	68 c9 00 00 00       	push   $0xc9
80101c85:	e9 d6 02 00 00       	jmp    80101f60 <vector_handler>

80101c8a <vector202>:
80101c8a:	6a 00                	push   $0x0
80101c8c:	68 ca 00 00 00       	push   $0xca
80101c91:	e9 ca 02 00 00       	jmp    80101f60 <vector_handler>

80101c96 <vector203>:
80101c96:	6a 00                	push   $0x0
80101c98:	68 cb 00 00 00       	push   $0xcb
80101c9d:	e9 be 02 00 00       	jmp    80101f60 <vector_handler>

80101ca2 <vector204>:
80101ca2:	6a 00                	push   $0x0
80101ca4:	68 cc 00 00 00       	push   $0xcc
80101ca9:	e9 b2 02 00 00       	jmp    80101f60 <vector_handler>

80101cae <vector205>:
80101cae:	6a 00                	push   $0x0
80101cb0:	68 cd 00 00 00       	push   $0xcd
80101cb5:	e9 a6 02 00 00       	jmp    80101f60 <vector_handler>

80101cba <vector206>:
80101cba:	6a 00                	push   $0x0
80101cbc:	68 ce 00 00 00       	push   $0xce
80101cc1:	e9 9a 02 00 00       	jmp    80101f60 <vector_handler>

80101cc6 <vector207>:
80101cc6:	6a 00                	push   $0x0
80101cc8:	68 cf 00 00 00       	push   $0xcf
80101ccd:	e9 8e 02 00 00       	jmp    80101f60 <vector_handler>

80101cd2 <vector208>:
80101cd2:	6a 00                	push   $0x0
80101cd4:	68 d0 00 00 00       	push   $0xd0
80101cd9:	e9 82 02 00 00       	jmp    80101f60 <vector_handler>

80101cde <vector209>:
80101cde:	6a 00                	push   $0x0
80101ce0:	68 d1 00 00 00       	push   $0xd1
80101ce5:	e9 76 02 00 00       	jmp    80101f60 <vector_handler>

80101cea <vector210>:
80101cea:	6a 00                	push   $0x0
80101cec:	68 d2 00 00 00       	push   $0xd2
80101cf1:	e9 6a 02 00 00       	jmp    80101f60 <vector_handler>

80101cf6 <vector211>:
80101cf6:	6a 00                	push   $0x0
80101cf8:	68 d3 00 00 00       	push   $0xd3
80101cfd:	e9 5e 02 00 00       	jmp    80101f60 <vector_handler>

80101d02 <vector212>:
80101d02:	6a 00                	push   $0x0
80101d04:	68 d4 00 00 00       	push   $0xd4
80101d09:	e9 52 02 00 00       	jmp    80101f60 <vector_handler>

80101d0e <vector213>:
80101d0e:	6a 00                	push   $0x0
80101d10:	68 d5 00 00 00       	push   $0xd5
80101d15:	e9 46 02 00 00       	jmp    80101f60 <vector_handler>

80101d1a <vector214>:
80101d1a:	6a 00                	push   $0x0
80101d1c:	68 d6 00 00 00       	push   $0xd6
80101d21:	e9 3a 02 00 00       	jmp    80101f60 <vector_handler>

80101d26 <vector215>:
80101d26:	6a 00                	push   $0x0
80101d28:	68 d7 00 00 00       	push   $0xd7
80101d2d:	e9 2e 02 00 00       	jmp    80101f60 <vector_handler>

80101d32 <vector216>:
80101d32:	6a 00                	push   $0x0
80101d34:	68 d8 00 00 00       	push   $0xd8
80101d39:	e9 22 02 00 00       	jmp    80101f60 <vector_handler>

80101d3e <vector217>:
80101d3e:	6a 00                	push   $0x0
80101d40:	68 d9 00 00 00       	push   $0xd9
80101d45:	e9 16 02 00 00       	jmp    80101f60 <vector_handler>

80101d4a <vector218>:
80101d4a:	6a 00                	push   $0x0
80101d4c:	68 da 00 00 00       	push   $0xda
80101d51:	e9 0a 02 00 00       	jmp    80101f60 <vector_handler>

80101d56 <vector219>:
80101d56:	6a 00                	push   $0x0
80101d58:	68 db 00 00 00       	push   $0xdb
80101d5d:	e9 fe 01 00 00       	jmp    80101f60 <vector_handler>

80101d62 <vector220>:
80101d62:	6a 00                	push   $0x0
80101d64:	68 dc 00 00 00       	push   $0xdc
80101d69:	e9 f2 01 00 00       	jmp    80101f60 <vector_handler>

80101d6e <vector221>:
80101d6e:	6a 00                	push   $0x0
80101d70:	68 dd 00 00 00       	push   $0xdd
80101d75:	e9 e6 01 00 00       	jmp    80101f60 <vector_handler>

80101d7a <vector222>:
80101d7a:	6a 00                	push   $0x0
80101d7c:	68 de 00 00 00       	push   $0xde
80101d81:	e9 da 01 00 00       	jmp    80101f60 <vector_handler>

80101d86 <vector223>:
80101d86:	6a 00                	push   $0x0
80101d88:	68 df 00 00 00       	push   $0xdf
80101d8d:	e9 ce 01 00 00       	jmp    80101f60 <vector_handler>

80101d92 <vector224>:
80101d92:	6a 00                	push   $0x0
80101d94:	68 e0 00 00 00       	push   $0xe0
80101d99:	e9 c2 01 00 00       	jmp    80101f60 <vector_handler>

80101d9e <vector225>:
80101d9e:	6a 00                	push   $0x0
80101da0:	68 e1 00 00 00       	push   $0xe1
80101da5:	e9 b6 01 00 00       	jmp    80101f60 <vector_handler>

80101daa <vector226>:
80101daa:	6a 00                	push   $0x0
80101dac:	68 e2 00 00 00       	push   $0xe2
80101db1:	e9 aa 01 00 00       	jmp    80101f60 <vector_handler>

80101db6 <vector227>:
80101db6:	6a 00                	push   $0x0
80101db8:	68 e3 00 00 00       	push   $0xe3
80101dbd:	e9 9e 01 00 00       	jmp    80101f60 <vector_handler>

80101dc2 <vector228>:
80101dc2:	6a 00                	push   $0x0
80101dc4:	68 e4 00 00 00       	push   $0xe4
80101dc9:	e9 92 01 00 00       	jmp    80101f60 <vector_handler>

80101dce <vector229>:
80101dce:	6a 00                	push   $0x0
80101dd0:	68 e5 00 00 00       	push   $0xe5
80101dd5:	e9 86 01 00 00       	jmp    80101f60 <vector_handler>

80101dda <vector230>:
80101dda:	6a 00                	push   $0x0
80101ddc:	68 e6 00 00 00       	push   $0xe6
80101de1:	e9 7a 01 00 00       	jmp    80101f60 <vector_handler>

80101de6 <vector231>:
80101de6:	6a 00                	push   $0x0
80101de8:	68 e7 00 00 00       	push   $0xe7
80101ded:	e9 6e 01 00 00       	jmp    80101f60 <vector_handler>

80101df2 <vector232>:
80101df2:	6a 00                	push   $0x0
80101df4:	68 e8 00 00 00       	push   $0xe8
80101df9:	e9 62 01 00 00       	jmp    80101f60 <vector_handler>

80101dfe <vector233>:
80101dfe:	6a 00                	push   $0x0
80101e00:	68 e9 00 00 00       	push   $0xe9
80101e05:	e9 56 01 00 00       	jmp    80101f60 <vector_handler>

80101e0a <vector234>:
80101e0a:	6a 00                	push   $0x0
80101e0c:	68 ea 00 00 00       	push   $0xea
80101e11:	e9 4a 01 00 00       	jmp    80101f60 <vector_handler>

80101e16 <vector235>:
80101e16:	6a 00                	push   $0x0
80101e18:	68 eb 00 00 00       	push   $0xeb
80101e1d:	e9 3e 01 00 00       	jmp    80101f60 <vector_handler>

80101e22 <vector236>:
80101e22:	6a 00                	push   $0x0
80101e24:	68 ec 00 00 00       	push   $0xec
80101e29:	e9 32 01 00 00       	jmp    80101f60 <vector_handler>

80101e2e <vector237>:
80101e2e:	6a 00                	push   $0x0
80101e30:	68 ed 00 00 00       	push   $0xed
80101e35:	e9 26 01 00 00       	jmp    80101f60 <vector_handler>

80101e3a <vector238>:
80101e3a:	6a 00                	push   $0x0
80101e3c:	68 ee 00 00 00       	push   $0xee
80101e41:	e9 1a 01 00 00       	jmp    80101f60 <vector_handler>

80101e46 <vector239>:
80101e46:	6a 00                	push   $0x0
80101e48:	68 ef 00 00 00       	push   $0xef
80101e4d:	e9 0e 01 00 00       	jmp    80101f60 <vector_handler>

80101e52 <vector240>:
80101e52:	6a 00                	push   $0x0
80101e54:	68 f0 00 00 00       	push   $0xf0
80101e59:	e9 02 01 00 00       	jmp    80101f60 <vector_handler>

80101e5e <vector241>:
80101e5e:	6a 00                	push   $0x0
80101e60:	68 f1 00 00 00       	push   $0xf1
80101e65:	e9 f6 00 00 00       	jmp    80101f60 <vector_handler>

80101e6a <vector242>:
80101e6a:	6a 00                	push   $0x0
80101e6c:	68 f2 00 00 00       	push   $0xf2
80101e71:	e9 ea 00 00 00       	jmp    80101f60 <vector_handler>

80101e76 <vector243>:
80101e76:	6a 00                	push   $0x0
80101e78:	68 f3 00 00 00       	push   $0xf3
80101e7d:	e9 de 00 00 00       	jmp    80101f60 <vector_handler>

80101e82 <vector244>:
80101e82:	6a 00                	push   $0x0
80101e84:	68 f4 00 00 00       	push   $0xf4
80101e89:	e9 d2 00 00 00       	jmp    80101f60 <vector_handler>

80101e8e <vector245>:
80101e8e:	6a 00                	push   $0x0
80101e90:	68 f5 00 00 00       	push   $0xf5
80101e95:	e9 c6 00 00 00       	jmp    80101f60 <vector_handler>

80101e9a <vector246>:
80101e9a:	6a 00                	push   $0x0
80101e9c:	68 f6 00 00 00       	push   $0xf6
80101ea1:	e9 ba 00 00 00       	jmp    80101f60 <vector_handler>

80101ea6 <vector247>:
80101ea6:	6a 00                	push   $0x0
80101ea8:	68 f7 00 00 00       	push   $0xf7
80101ead:	e9 ae 00 00 00       	jmp    80101f60 <vector_handler>

80101eb2 <vector248>:
80101eb2:	6a 00                	push   $0x0
80101eb4:	68 f8 00 00 00       	push   $0xf8
80101eb9:	e9 a2 00 00 00       	jmp    80101f60 <vector_handler>

80101ebe <vector249>:
80101ebe:	6a 00                	push   $0x0
80101ec0:	68 f9 00 00 00       	push   $0xf9
80101ec5:	e9 96 00 00 00       	jmp    80101f60 <vector_handler>

80101eca <vector250>:
80101eca:	6a 00                	push   $0x0
80101ecc:	68 fa 00 00 00       	push   $0xfa
80101ed1:	e9 8a 00 00 00       	jmp    80101f60 <vector_handler>

80101ed6 <vector251>:
80101ed6:	6a 00                	push   $0x0
80101ed8:	68 fb 00 00 00       	push   $0xfb
80101edd:	e9 7e 00 00 00       	jmp    80101f60 <vector_handler>

80101ee2 <vector252>:
80101ee2:	6a 00                	push   $0x0
80101ee4:	68 fc 00 00 00       	push   $0xfc
80101ee9:	e9 72 00 00 00       	jmp    80101f60 <vector_handler>

80101eee <vector253>:
80101eee:	6a 00                	push   $0x0
80101ef0:	68 fd 00 00 00       	push   $0xfd
80101ef5:	e9 66 00 00 00       	jmp    80101f60 <vector_handler>

80101efa <vector254>:
80101efa:	6a 00                	push   $0x0
80101efc:	68 fe 00 00 00       	push   $0xfe
80101f01:	e9 5a 00 00 00       	jmp    80101f60 <vector_handler>

80101f06 <vector255>:
80101f06:	6a 00                	push   $0x0
80101f08:	68 ff 00 00 00       	push   $0xff
80101f0d:	e9 4e 00 00 00       	jmp    80101f60 <vector_handler>
80101f12:	66 90                	xchg   %ax,%ax
80101f14:	66 90                	xchg   %ax,%ax
80101f16:	66 90                	xchg   %ax,%ax
80101f18:	66 90                	xchg   %ax,%ax
80101f1a:	66 90                	xchg   %ax,%ax
80101f1c:	66 90                	xchg   %ax,%ax
80101f1e:	66 90                	xchg   %ax,%ax

80101f20 <_start>:
80101f20:	0f 20 e1             	mov    %cr4,%ecx
80101f23:	83 c9 10             	or     $0x10,%ecx
80101f26:	0f 22 e1             	mov    %ecx,%cr4
80101f29:	b9 00 40 10 00       	mov    $0x104000,%ecx
80101f2e:	0f 22 d9             	mov    %ecx,%cr3
80101f31:	0f 20 c1             	mov    %cr0,%ecx
80101f34:	81 c9 00 00 00 80    	or     $0x80000000,%ecx
80101f3a:	0f 22 c1             	mov    %ecx,%cr0
80101f3d:	bc 30 b4 10 80       	mov    $0x8010b430,%esp
80101f42:	50                   	push   %eax
80101f43:	81 c3 00 00 00 80    	add    $0x80000000,%ebx
80101f49:	53                   	push   %ebx
80101f4a:	e9 b1 f0 ff ff       	jmp    80101000 <core_main>
80101f4f:	f4                   	hlt    

80101f50 <_start.end>:
80101f50:	eb fe                	jmp    80101f50 <_start.end>
80101f52:	66 90                	xchg   %ax,%ax
80101f54:	66 90                	xchg   %ax,%ax
80101f56:	66 90                	xchg   %ax,%ax
80101f58:	66 90                	xchg   %ax,%ax
80101f5a:	66 90                	xchg   %ax,%ax
80101f5c:	66 90                	xchg   %ax,%ax
80101f5e:	66 90                	xchg   %ax,%ax

80101f60 <vector_handler>:
80101f60:	60                   	pusha  
80101f61:	1e                   	push   %ds
80101f62:	06                   	push   %es
80101f63:	0f a0                	push   %fs
80101f65:	0f a8                	push   %gs
80101f67:	89 d6                	mov    %edx,%esi
80101f69:	66 b8 10 00          	mov    $0x10,%ax
80101f6d:	8e d8                	mov    %eax,%ds
80101f6f:	8e c0                	mov    %eax,%es
80101f71:	8e e0                	mov    %eax,%fs
80101f73:	8e e8                	mov    %eax,%gs
80101f75:	89 e1                	mov    %esp,%ecx
80101f77:	83 3d 00 84 10 80 00 	cmpl   $0x0,0x80108400
80101f7e:	75 05                	jne    80101f85 <vector_handler.non_zero>
80101f80:	bc 04 94 10 80       	mov    $0x80109404,%esp

80101f85 <vector_handler.non_zero>:
80101f85:	ff 05 00 84 10 80    	incl   0x80108400
80101f8b:	51                   	push   %ecx
80101f8c:	e8 98 f2 ff ff       	call   80101229 <interrupt_handler>
80101f91:	83 c4 04             	add    $0x4,%esp

80101f94 <vector_handler_ret>:
80101f94:	ff 0d 00 84 10 80    	decl   0x80108400
80101f9a:	83 3d 00 84 10 80 00 	cmpl   $0x0,0x80108400
80101fa1:	75 00                	jne    80101fa3 <vector_handler_ret.non_zero>

80101fa3 <vector_handler_ret.non_zero>:
80101fa3:	0f a9                	pop    %gs
80101fa5:	0f a1                	pop    %fs
80101fa7:	07                   	pop    %es
80101fa8:	1f                   	pop    %ds
80101fa9:	61                   	popa   
80101faa:	83 c4 08             	add    $0x8,%esp
80101fad:	cf                   	iret   

80101fae <GRAPHIC_write_color_string_to_vm>:
    buf[i]      = 0;
    disp_pos    = GRAPHIC_write_color_string_to_vm(
        disp_pos, COLOR(BLACK, WHITE | BRIGHT), buf);
}

uint GRAPHIC_write_color_string_to_vm(u32 offset, u16 color, const char *str) {
80101fae:	55                   	push   %ebp
80101faf:	57                   	push   %edi
80101fb0:	56                   	push   %esi
80101fb1:	53                   	push   %ebx
80101fb2:	8b 6c 24 18          	mov    0x18(%esp),%ebp
80101fb6:	8b 4c 24 1c          	mov    0x1c(%esp),%ecx
    volatile char *pCh = (volatile char *)((uint)vm_start + offset);
80101fba:	a1 04 94 10 80       	mov    0x80109404,%eax
80101fbf:	03 44 24 14          	add    0x14(%esp),%eax
    char           ch  = *str;
80101fc3:	0f b6 11             	movzbl (%ecx),%edx
    while (*str != 0) {
80101fc6:	84 d2                	test   %dl,%dl
80101fc8:	74 57                	je     80102021 <GRAPHIC_write_color_string_to_vm+0x73>
80101fca:	89 ee                	mov    %ebp,%esi
        switch (ch) {
        case '\n':
            pCh =
                (volatile char *)(vm_start +
                                  SCREEN_WIDTH * 2 *
                                      (((uint)pCh - (uint)vm_start) / 160 + 1));
80101fcc:	bf cd cc cc cc       	mov    $0xcccccccd,%edi
80101fd1:	eb 2c                	jmp    80101fff <GRAPHIC_write_color_string_to_vm+0x51>
            break;
        case '\b':
            if (pCh - 2 < vm_start)
80101fd3:	8d 50 fe             	lea    -0x2(%eax),%edx
80101fd6:	3b 15 04 94 10 80    	cmp    0x80109404,%edx
80101fdc:	72 17                	jb     80101ff5 <GRAPHIC_write_color_string_to_vm+0x47>
                break;
            *(pCh - 2) = ' ';
80101fde:	c6 40 fe 20          	movb   $0x20,-0x2(%eax)
            *(pCh - 1) = color;
80101fe2:	89 f3                	mov    %esi,%ebx
80101fe4:	88 58 ff             	mov    %bl,-0x1(%eax)
            pCh -= 2;
80101fe7:	89 d0                	mov    %edx,%eax
            break;
80101fe9:	eb 0a                	jmp    80101ff5 <GRAPHIC_write_color_string_to_vm+0x47>
        default:
            *pCh++ = ch;
80101feb:	88 10                	mov    %dl,(%eax)
            *pCh++ = color;
80101fed:	89 f3                	mov    %esi,%ebx
80101fef:	88 58 01             	mov    %bl,0x1(%eax)
80101ff2:	8d 40 02             	lea    0x2(%eax),%eax
            break;
        }
        ch = *++str;
80101ff5:	83 c1 01             	add    $0x1,%ecx
80101ff8:	0f b6 11             	movzbl (%ecx),%edx
    while (*str != 0) {
80101ffb:	84 d2                	test   %dl,%dl
80101ffd:	74 22                	je     80102021 <GRAPHIC_write_color_string_to_vm+0x73>
        switch (ch) {
80101fff:	80 fa 08             	cmp    $0x8,%dl
80102002:	74 cf                	je     80101fd3 <GRAPHIC_write_color_string_to_vm+0x25>
80102004:	80 fa 0a             	cmp    $0xa,%dl
80102007:	75 e2                	jne    80101feb <GRAPHIC_write_color_string_to_vm+0x3d>
                (volatile char *)(vm_start +
80102009:	8b 1d 04 94 10 80    	mov    0x80109404,%ebx
                                      (((uint)pCh - (uint)vm_start) / 160 + 1));
8010200f:	29 d8                	sub    %ebx,%eax
80102011:	f7 e7                	mul    %edi
80102013:	c1 ea 07             	shr    $0x7,%edx
                                  SCREEN_WIDTH * 2 *
80102016:	8d 44 92 05          	lea    0x5(%edx,%edx,4),%eax
8010201a:	c1 e0 05             	shl    $0x5,%eax
            pCh =
8010201d:	01 d8                	add    %ebx,%eax
            break;
8010201f:	eb d4                	jmp    80101ff5 <GRAPHIC_write_color_string_to_vm+0x47>
    }
    if (*(pCh + 1) == 0)
80102021:	0f b6 50 01          	movzbl 0x1(%eax),%edx
80102025:	84 d2                	test   %dl,%dl
80102027:	75 05                	jne    8010202e <GRAPHIC_write_color_string_to_vm+0x80>
        *(pCh + 1) = color; // make cursor visible
80102029:	89 eb                	mov    %ebp,%ebx
8010202b:	88 58 01             	mov    %bl,0x1(%eax)
    return (uint)pCh - (uint)vm_start;
8010202e:	2b 05 04 94 10 80    	sub    0x80109404,%eax
}
80102034:	5b                   	pop    %ebx
80102035:	5e                   	pop    %esi
80102036:	5f                   	pop    %edi
80102037:	5d                   	pop    %ebp
80102038:	c3                   	ret    

80102039 <kprintf>:
void kprintf(const char *fmt, ...) {
80102039:	53                   	push   %ebx
8010203a:	81 ec 0c 01 00 00    	sub    $0x10c,%esp
    i           = vsprintf(buf, fmt, arg);
80102040:	8d 84 24 18 01 00 00 	lea    0x118(%esp),%eax
80102047:	50                   	push   %eax
80102048:	ff b4 24 18 01 00 00 	pushl  0x118(%esp)
8010204f:	8d 5c 24 0c          	lea    0xc(%esp),%ebx
80102053:	53                   	push   %ebx
80102054:	e8 8a 00 00 00       	call   801020e3 <vsprintf>
    buf[i]      = 0;
80102059:	c6 44 04 10 00       	movb   $0x0,0x10(%esp,%eax,1)
    disp_pos    = GRAPHIC_write_color_string_to_vm(
8010205e:	83 c4 0c             	add    $0xc,%esp
80102061:	53                   	push   %ebx
80102062:	6a 0f                	push   $0xf
80102064:	ff 35 30 b4 10 80    	pushl  0x8010b430
8010206a:	e8 3f ff ff ff       	call   80101fae <GRAPHIC_write_color_string_to_vm>
8010206f:	a3 30 b4 10 80       	mov    %eax,0x8010b430
}
80102074:	81 c4 18 01 00 00    	add    $0x118,%esp
8010207a:	5b                   	pop    %ebx
8010207b:	c3                   	ret    

8010207c <itoa>:
#include "generic/typedefs.h"
#include "lib/string.h"

char *itoa(uint value, char *str, int base) {
8010207c:	55                   	push   %ebp
8010207d:	57                   	push   %edi
8010207e:	56                   	push   %esi
8010207f:	53                   	push   %ebx
80102080:	8b 74 24 14          	mov    0x14(%esp),%esi
80102084:	8b 7c 24 18          	mov    0x18(%esp),%edi
80102088:	8b 44 24 1c          	mov    0x1c(%esp),%eax
    char *rc;
    char *ptr;
    char *low;
    if (base < 2 || base > 36) {
8010208c:	89 c5                	mov    %eax,%ebp
8010208e:	83 e8 02             	sub    $0x2,%eax
        *str = '\0';
        return str;
    }
    rc = ptr = str;
80102091:	89 fb                	mov    %edi,%ebx
    if (base < 2 || base > 36) {
80102093:	83 f8 22             	cmp    $0x22,%eax
80102096:	77 46                	ja     801020de <itoa+0x62>
    if (value < 0 && base == 10)
        *ptr++ = '-';
    low = ptr;
    do {
        *ptr++ =
80102098:	89 d9                	mov    %ebx,%ecx
8010209a:	83 c3 01             	add    $0x1,%ebx
            "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRST"
            "UVWXYZ"[35 + value % base];
8010209d:	89 f0                	mov    %esi,%eax
8010209f:	ba 00 00 00 00       	mov    $0x0,%edx
801020a4:	f7 f5                	div    %ebp
        *ptr++ =
801020a6:	0f b6 92 3f 31 10 80 	movzbl -0x7fefcec1(%edx),%edx
801020ad:	88 53 ff             	mov    %dl,-0x1(%ebx)
        value /= base;
801020b0:	89 f2                	mov    %esi,%edx
801020b2:	89 c6                	mov    %eax,%esi
    } while (value);
801020b4:	39 d5                	cmp    %edx,%ebp
801020b6:	76 e0                	jbe    80102098 <itoa+0x1c>
    *ptr-- = '\0';
801020b8:	c6 03 00             	movb   $0x0,(%ebx)
    while (low < ptr) {
801020bb:	39 f9                	cmp    %edi,%ecx
801020bd:	76 18                	jbe    801020d7 <itoa+0x5b>
    low = ptr;
801020bf:	89 f8                	mov    %edi,%eax
        char tmp = *low;
801020c1:	0f b6 18             	movzbl (%eax),%ebx
        *low++   = *ptr;
801020c4:	83 c0 01             	add    $0x1,%eax
801020c7:	0f b6 11             	movzbl (%ecx),%edx
801020ca:	88 50 ff             	mov    %dl,-0x1(%eax)
        *ptr--   = tmp;
801020cd:	83 e9 01             	sub    $0x1,%ecx
801020d0:	88 59 01             	mov    %bl,0x1(%ecx)
    while (low < ptr) {
801020d3:	39 c8                	cmp    %ecx,%eax
801020d5:	72 ea                	jb     801020c1 <itoa+0x45>
    }
    return rc;
}
801020d7:	89 f8                	mov    %edi,%eax
801020d9:	5b                   	pop    %ebx
801020da:	5e                   	pop    %esi
801020db:	5f                   	pop    %edi
801020dc:	5d                   	pop    %ebp
801020dd:	c3                   	ret    
        *str = '\0';
801020de:	c6 07 00             	movb   $0x0,(%edi)
        return str;
801020e1:	eb f4                	jmp    801020d7 <itoa+0x5b>

801020e3 <vsprintf>:

int vsprintf(char *buf, const char *fmt, va_list args) {
801020e3:	55                   	push   %ebp
801020e4:	57                   	push   %edi
801020e5:	56                   	push   %esi
801020e6:	53                   	push   %ebx
801020e7:	81 ec 1c 04 00 00    	sub    $0x41c,%esp
801020ed:	8b 94 24 34 04 00 00 	mov    0x434(%esp),%edx
    char inner_buf[1024]; // hardcode is not good
    char cs;
    int  align;

    char *p;
    for (p = buf; *fmt; fmt++) {
801020f4:	0f b6 02             	movzbl (%edx),%eax
801020f7:	84 c0                	test   %al,%al
801020f9:	0f 84 ff 01 00 00    	je     801022fe <vsprintf+0x21b>
801020ff:	8b b4 24 30 04 00 00 	mov    0x430(%esp),%esi
            break;
        }

        for (int k = 0;
             k <
             ((align > strlen(inner_buf)) ? (align - strlen(inner_buf)) : 0);
80102106:	c7 44 24 08 00 00 00 	movl   $0x0,0x8(%esp)
8010210d:	00 
8010210e:	eb 18                	jmp    80102128 <vsprintf+0x45>
            *p++ = *fmt;
80102110:	88 06                	mov    %al,(%esi)
80102112:	8d 46 01             	lea    0x1(%esi),%eax
80102115:	89 c6                	mov    %eax,%esi
            continue;
80102117:	89 d5                	mov    %edx,%ebp
    for (p = buf; *fmt; fmt++) {
80102119:	8d 55 01             	lea    0x1(%ebp),%edx
8010211c:	0f b6 45 01          	movzbl 0x1(%ebp),%eax
80102120:	84 c0                	test   %al,%al
80102122:	0f 84 df 01 00 00    	je     80102307 <vsprintf+0x224>
        if (*fmt != '%') {
80102128:	3c 25                	cmp    $0x25,%al
8010212a:	75 e4                	jne    80102110 <vsprintf+0x2d>
        fmt++;
8010212c:	8d 6a 01             	lea    0x1(%edx),%ebp
        if (*fmt == '%') {
8010212f:	0f b6 42 01          	movzbl 0x1(%edx),%eax
80102133:	88 44 24 07          	mov    %al,0x7(%esp)
80102137:	3c 25                	cmp    $0x25,%al
80102139:	0f 84 9d 00 00 00    	je     801021dc <vsprintf+0xf9>
        } else if (*fmt == '0') {
8010213f:	80 7c 24 07 30       	cmpb   $0x30,0x7(%esp)
80102144:	0f 84 9f 00 00 00    	je     801021e9 <vsprintf+0x106>
            cs = ' ';
8010214a:	c6 44 24 07 20       	movb   $0x20,0x7(%esp)
        while (((unsigned char)(*fmt) >= '0') &&
8010214f:	0f b6 45 00          	movzbl 0x0(%ebp),%eax
80102153:	8d 50 d0             	lea    -0x30(%eax),%edx
80102156:	80 fa 09             	cmp    $0x9,%dl
80102159:	0f 87 92 00 00 00    	ja     801021f1 <vsprintf+0x10e>
8010215f:	bf 00 00 00 00       	mov    $0x0,%edi
            align *= 10;
80102164:	8d 14 bf             	lea    (%edi,%edi,4),%edx
            align += *fmt - '0';
80102167:	0f be c0             	movsbl %al,%eax
8010216a:	8d 7c 50 d0          	lea    -0x30(%eax,%edx,2),%edi
            fmt++;
8010216e:	83 c5 01             	add    $0x1,%ebp
        while (((unsigned char)(*fmt) >= '0') &&
80102171:	0f b6 45 00          	movzbl 0x0(%ebp),%eax
80102175:	8d 50 d0             	lea    -0x30(%eax),%edx
80102178:	80 fa 09             	cmp    $0x9,%dl
8010217b:	76 e7                	jbe    80102164 <vsprintf+0x81>
        memset((void *)q, 0, sizeof(inner_buf));
8010217d:	83 ec 04             	sub    $0x4,%esp
80102180:	68 00 04 00 00       	push   $0x400
80102185:	6a 00                	push   $0x0
80102187:	8d 44 24 1c          	lea    0x1c(%esp),%eax
8010218b:	50                   	push   %eax
8010218c:	e8 4c 02 00 00       	call   801023dd <memset>
        switch (*fmt) {
80102191:	0f b6 45 00          	movzbl 0x0(%ebp),%eax
80102195:	83 c4 10             	add    $0x10,%esp
80102198:	3c 73                	cmp    $0x73,%al
8010219a:	0f 84 b4 00 00 00    	je     80102254 <vsprintf+0x171>
801021a0:	7f 56                	jg     801021f8 <vsprintf+0x115>
801021a2:	3c 63                	cmp    $0x63,%al
801021a4:	74 7b                	je     80102221 <vsprintf+0x13e>
801021a6:	3c 64                	cmp    $0x64,%al
801021a8:	0f 85 89 00 00 00    	jne    80102237 <vsprintf+0x154>
            m = *((int *)arg);
801021ae:	8b 84 24 38 04 00 00 	mov    0x438(%esp),%eax
801021b5:	8b 00                	mov    (%eax),%eax
        char *q = inner_buf;
801021b7:	8d 54 24 10          	lea    0x10(%esp),%edx
            if (m < 0) {
801021bb:	85 c0                	test   %eax,%eax
801021bd:	0f 88 81 00 00 00    	js     80102244 <vsprintf+0x161>
            itoa(m, q, 10);
801021c3:	83 ec 04             	sub    $0x4,%esp
801021c6:	6a 0a                	push   $0xa
801021c8:	52                   	push   %edx
801021c9:	50                   	push   %eax
801021ca:	e8 ad fe ff ff       	call   8010207c <itoa>
            arg += 4;
801021cf:	83 84 24 48 04 00 00 	addl   $0x4,0x448(%esp)
801021d6:	04 
            break;
801021d7:	83 c4 10             	add    $0x10,%esp
801021da:	eb 5b                	jmp    80102237 <vsprintf+0x154>
            *p++ = *fmt;
801021dc:	c6 06 25             	movb   $0x25,(%esi)
801021df:	8d 46 01             	lea    0x1(%esi),%eax
801021e2:	89 c6                	mov    %eax,%esi
            continue;
801021e4:	e9 30 ff ff ff       	jmp    80102119 <vsprintf+0x36>
            fmt++;
801021e9:	8d 6a 02             	lea    0x2(%edx),%ebp
801021ec:	e9 5e ff ff ff       	jmp    8010214f <vsprintf+0x6c>
        while (((unsigned char)(*fmt) >= '0') &&
801021f1:	bf 00 00 00 00       	mov    $0x0,%edi
801021f6:	eb 85                	jmp    8010217d <vsprintf+0x9a>
        switch (*fmt) {
801021f8:	3c 78                	cmp    $0x78,%al
801021fa:	75 3b                	jne    80102237 <vsprintf+0x154>
            itoa(m, q, 16);
801021fc:	83 ec 04             	sub    $0x4,%esp
801021ff:	6a 10                	push   $0x10
80102201:	8d 44 24 18          	lea    0x18(%esp),%eax
80102205:	50                   	push   %eax
80102206:	8b 84 24 44 04 00 00 	mov    0x444(%esp),%eax
8010220d:	ff 30                	pushl  (%eax)
8010220f:	e8 68 fe ff ff       	call   8010207c <itoa>
            arg += 4;
80102214:	83 84 24 48 04 00 00 	addl   $0x4,0x448(%esp)
8010221b:	04 
            break;
8010221c:	83 c4 10             	add    $0x10,%esp
8010221f:	eb 16                	jmp    80102237 <vsprintf+0x154>
            *q++ = *((char *)arg);
80102221:	8b 84 24 38 04 00 00 	mov    0x438(%esp),%eax
80102228:	0f b6 00             	movzbl (%eax),%eax
8010222b:	88 44 24 10          	mov    %al,0x10(%esp)
            arg += 4;
8010222f:	83 84 24 38 04 00 00 	addl   $0x4,0x438(%esp)
80102236:	04 
        for (int k = 0;
80102237:	bb 00 00 00 00       	mov    $0x0,%ebx
8010223c:	89 6c 24 0c          	mov    %ebp,0xc(%esp)
80102240:	89 f5                	mov    %esi,%ebp
80102242:	eb 6d                	jmp    801022b1 <vsprintf+0x1ce>
                m    = m * (-1);
80102244:	f7 d8                	neg    %eax
                *q++ = '-';
80102246:	c6 44 24 10 2d       	movb   $0x2d,0x10(%esp)
8010224b:	8d 54 24 11          	lea    0x11(%esp),%edx
8010224f:	e9 6f ff ff ff       	jmp    801021c3 <vsprintf+0xe0>
            strcpy(q, (*((char **)arg)));
80102254:	83 ec 08             	sub    $0x8,%esp
80102257:	8b 84 24 40 04 00 00 	mov    0x440(%esp),%eax
8010225e:	ff 30                	pushl  (%eax)
80102260:	8d 44 24 1c          	lea    0x1c(%esp),%eax
80102264:	50                   	push   %eax
80102265:	e8 32 01 00 00       	call   8010239c <strcpy>
            q += strlen(*((char **)arg));
8010226a:	83 c4 04             	add    $0x4,%esp
8010226d:	8b 84 24 44 04 00 00 	mov    0x444(%esp),%eax
80102274:	ff 30                	pushl  (%eax)
80102276:	e8 0c 01 00 00       	call   80102387 <strlen>
            arg += 4;
8010227b:	83 84 24 48 04 00 00 	addl   $0x4,0x448(%esp)
80102282:	04 
80102283:	83 c4 10             	add    $0x10,%esp
80102286:	eb af                	jmp    80102237 <vsprintf+0x154>
             ((align > strlen(inner_buf)) ? (align - strlen(inner_buf)) : 0);
80102288:	83 ec 0c             	sub    $0xc,%esp
8010228b:	8d 44 24 1c          	lea    0x1c(%esp),%eax
8010228f:	50                   	push   %eax
80102290:	e8 f2 00 00 00       	call   80102387 <strlen>
80102295:	89 f9                	mov    %edi,%ecx
80102297:	29 c1                	sub    %eax,%ecx
80102299:	89 c8                	mov    %ecx,%eax
8010229b:	83 c4 10             	add    $0x10,%esp
        for (int k = 0;
8010229e:	39 c3                	cmp    %eax,%ebx
801022a0:	7d 32                	jge    801022d4 <vsprintf+0x1f1>
             k++)
            *p++ = cs;
801022a2:	8b 0c 24             	mov    (%esp),%ecx
801022a5:	0f b6 44 24 07       	movzbl 0x7(%esp),%eax
801022aa:	88 44 0d 00          	mov    %al,0x0(%ebp,%ecx,1)
             k++)
801022ae:	83 c3 01             	add    $0x1,%ebx
801022b1:	89 1c 24             	mov    %ebx,(%esp)
801022b4:	8d 74 1d 00          	lea    0x0(%ebp,%ebx,1),%esi
             ((align > strlen(inner_buf)) ? (align - strlen(inner_buf)) : 0);
801022b8:	83 ec 0c             	sub    $0xc,%esp
801022bb:	8d 44 24 1c          	lea    0x1c(%esp),%eax
801022bf:	50                   	push   %eax
801022c0:	e8 c2 00 00 00       	call   80102387 <strlen>
801022c5:	89 c2                	mov    %eax,%edx
801022c7:	83 c4 10             	add    $0x10,%esp
801022ca:	8b 44 24 08          	mov    0x8(%esp),%eax
801022ce:	39 fa                	cmp    %edi,%edx
801022d0:	7d cc                	jge    8010229e <vsprintf+0x1bb>
801022d2:	eb b4                	jmp    80102288 <vsprintf+0x1a5>
801022d4:	8b 6c 24 0c          	mov    0xc(%esp),%ebp
        q = inner_buf;
        while (*q)
801022d8:	0f b6 44 24 10       	movzbl 0x10(%esp),%eax
801022dd:	84 c0                	test   %al,%al
801022df:	0f 84 34 fe ff ff    	je     80102119 <vsprintf+0x36>
        q = inner_buf;
801022e5:	8d 54 24 10          	lea    0x10(%esp),%edx
            *p++ = *q++;
801022e9:	83 c2 01             	add    $0x1,%edx
801022ec:	83 c6 01             	add    $0x1,%esi
801022ef:	88 46 ff             	mov    %al,-0x1(%esi)
        while (*q)
801022f2:	0f b6 02             	movzbl (%edx),%eax
801022f5:	84 c0                	test   %al,%al
801022f7:	75 f0                	jne    801022e9 <vsprintf+0x206>
801022f9:	e9 1b fe ff ff       	jmp    80102119 <vsprintf+0x36>
    for (p = buf; *fmt; fmt++) {
801022fe:	8b 84 24 30 04 00 00 	mov    0x430(%esp),%eax
80102305:	89 c6                	mov    %eax,%esi
    }

    *p = 0;
80102307:	89 f0                	mov    %esi,%eax
80102309:	c6 06 00             	movb   $0x0,(%esi)
    return (p - buf);
8010230c:	2b 84 24 30 04 00 00 	sub    0x430(%esp),%eax
}
80102313:	81 c4 1c 04 00 00    	add    $0x41c,%esp
80102319:	5b                   	pop    %ebx
8010231a:	5e                   	pop    %esi
8010231b:	5f                   	pop    %edi
8010231c:	5d                   	pop    %ebp
8010231d:	c3                   	ret    

8010231e <sprintf>:

int sprintf(char *buf, const char *fmt, ...) {
8010231e:	83 ec 10             	sub    $0x10,%esp
    va_list arg = (va_list)((char *)(&fmt) + 4);
    return vsprintf(buf, fmt, arg);
80102321:	8d 44 24 1c          	lea    0x1c(%esp),%eax
80102325:	50                   	push   %eax
80102326:	ff 74 24 1c          	pushl  0x1c(%esp)
8010232a:	ff 74 24 1c          	pushl  0x1c(%esp)
8010232e:	e8 b0 fd ff ff       	call   801020e3 <vsprintf>
}
80102333:	83 c4 1c             	add    $0x1c,%esp
80102336:	c3                   	ret    

80102337 <memcpy>:
#include "lib/string.h"
#include "generic/asm.h"

void *memcpy(void *dst, const void *src, int size) {
80102337:	56                   	push   %esi
80102338:	53                   	push   %ebx
80102339:	8b 74 24 0c          	mov    0xc(%esp),%esi
8010233d:	8b 44 24 10          	mov    0x10(%esp),%eax
80102341:	8b 4c 24 14          	mov    0x14(%esp),%ecx
    const char *s;
    char *      d;
    s = src;
    d = dst;
    if (s < d &&
80102345:	39 f0                	cmp    %esi,%eax
80102347:	72 1f                	jb     80102368 <memcpy+0x31>
        s += size;
        d += size;
        while (size-- > 0)
            *--d = *--s;
    } else
        while (size-- > 0)
80102349:	8d 1c 08             	lea    (%eax,%ecx,1),%ebx
8010234c:	89 f2                	mov    %esi,%edx
8010234e:	85 c9                	test   %ecx,%ecx
80102350:	7e 11                	jle    80102363 <memcpy+0x2c>
            *d++ = *s++;
80102352:	83 c0 01             	add    $0x1,%eax
80102355:	83 c2 01             	add    $0x1,%edx
80102358:	0f b6 48 ff          	movzbl -0x1(%eax),%ecx
8010235c:	88 4a ff             	mov    %cl,-0x1(%edx)
        while (size-- > 0)
8010235f:	39 c3                	cmp    %eax,%ebx
80102361:	75 ef                	jne    80102352 <memcpy+0x1b>
    return dst;
}
80102363:	89 f0                	mov    %esi,%eax
80102365:	5b                   	pop    %ebx
80102366:	5e                   	pop    %esi
80102367:	c3                   	ret    
        s + size > d) { // 当src和dst有重叠时从后向前拷贝，避免覆盖产生的错误
80102368:	8d 14 08             	lea    (%eax,%ecx,1),%edx
    if (s < d &&
8010236b:	39 d6                	cmp    %edx,%esi
8010236d:	73 da                	jae    80102349 <memcpy+0x12>
        while (size-- > 0)
8010236f:	8d 51 ff             	lea    -0x1(%ecx),%edx
80102372:	85 c9                	test   %ecx,%ecx
80102374:	7e ed                	jle    80102363 <memcpy+0x2c>
            *--d = *--s;
80102376:	0f b6 0c 10          	movzbl (%eax,%edx,1),%ecx
8010237a:	88 0c 16             	mov    %cl,(%esi,%edx,1)
        while (size-- > 0)
8010237d:	83 ea 01             	sub    $0x1,%edx
80102380:	83 fa ff             	cmp    $0xffffffff,%edx
80102383:	75 f1                	jne    80102376 <memcpy+0x3f>
80102385:	eb dc                	jmp    80102363 <memcpy+0x2c>

80102387 <strlen>:
    else
        memcpy(dst, src, strlen(src) + 1);
    return odst;
}

int strlen(const char *s) {
80102387:	8b 54 24 04          	mov    0x4(%esp),%edx
    const char *eos = s;
8010238b:	89 d0                	mov    %edx,%eax
    while (*eos++)
8010238d:	83 c0 01             	add    $0x1,%eax
80102390:	80 78 ff 00          	cmpb   $0x0,-0x1(%eax)
80102394:	75 f7                	jne    8010238d <strlen+0x6>
        ;
    return (eos - s - 1);
80102396:	29 d0                	sub    %edx,%eax
80102398:	83 e8 01             	sub    $0x1,%eax
}
8010239b:	c3                   	ret    

8010239c <strcpy>:
char *strcpy(char *dst, char *src) {
8010239c:	56                   	push   %esi
8010239d:	53                   	push   %ebx
8010239e:	8b 74 24 0c          	mov    0xc(%esp),%esi
801023a2:	8b 5c 24 10          	mov    0x10(%esp),%ebx
    if (src < dst)
801023a6:	39 f3                	cmp    %esi,%ebx
801023a8:	73 1b                	jae    801023c5 <strcpy+0x29>
801023aa:	b8 00 00 00 00       	mov    $0x0,%eax
        while ((*dst++ = *src++) != '\0')
801023af:	0f b6 14 03          	movzbl (%ebx,%eax,1),%edx
801023b3:	88 14 06             	mov    %dl,(%esi,%eax,1)
801023b6:	83 c0 01             	add    $0x1,%eax
801023b9:	84 d2                	test   %dl,%dl
801023bb:	75 f2                	jne    801023af <strcpy+0x13>
}
801023bd:	b8 00 00 00 00       	mov    $0x0,%eax
801023c2:	5b                   	pop    %ebx
801023c3:	5e                   	pop    %esi
801023c4:	c3                   	ret    
        memcpy(dst, src, strlen(src) + 1);
801023c5:	53                   	push   %ebx
801023c6:	e8 bc ff ff ff       	call   80102387 <strlen>
801023cb:	83 c0 01             	add    $0x1,%eax
801023ce:	89 04 24             	mov    %eax,(%esp)
801023d1:	53                   	push   %ebx
801023d2:	56                   	push   %esi
801023d3:	e8 5f ff ff ff       	call   80102337 <memcpy>
801023d8:	83 c4 0c             	add    $0xc,%esp
    return odst;
801023db:	eb e0                	jmp    801023bd <strcpy+0x21>

801023dd <memset>:

void *memset(void *dst, char ch, int size) {
801023dd:	57                   	push   %edi
801023de:	8b 54 24 08          	mov    0x8(%esp),%edx
801023e2:	8b 44 24 0c          	mov    0xc(%esp),%eax
801023e6:	8b 4c 24 10          	mov    0x10(%esp),%ecx
    if ((int)dst % 4 == 0 &&
801023ea:	89 d7                	mov    %edx,%edi
801023ec:	09 cf                	or     %ecx,%edi
801023ee:	f7 c7 03 00 00 00    	test   $0x3,%edi
801023f4:	74 09                	je     801023ff <memset+0x22>
    asm volatile("cld; rep stosb"
801023f6:	89 d7                	mov    %edx,%edi
801023f8:	fc                   	cld    
801023f9:	f3 aa                	rep stos %al,%es:(%edi)
        ch &= 0xFF;
        stosl(dst, (ch << 24) | (ch << 16) | (ch << 8) | ch, size / 4);
    } else
        stosb(dst, ch, size);
    return dst;
}
801023fb:	89 d0                	mov    %edx,%eax
801023fd:	5f                   	pop    %edi
801023fe:	c3                   	ret    
        stosl(dst, (ch << 24) | (ch << 16) | (ch << 8) | ch, size / 4);
801023ff:	8d 79 03             	lea    0x3(%ecx),%edi
80102402:	85 c9                	test   %ecx,%ecx
80102404:	0f 48 cf             	cmovs  %edi,%ecx
80102407:	c1 f9 02             	sar    $0x2,%ecx
    asm volatile("cld; rep stosl"
8010240a:	89 d7                	mov    %edx,%edi
8010240c:	fc                   	cld    
8010240d:	f3 ab                	rep stos %eax,%es:(%edi)
}
8010240f:	eb ea                	jmp    801023fb <memset+0x1e>

80102411 <strcmp>:

int strcmp(const char *cs, const char *ct) {
80102411:	56                   	push   %esi
80102412:	53                   	push   %ebx
80102413:	8b 74 24 0c          	mov    0xc(%esp),%esi
80102417:	8b 5c 24 10          	mov    0x10(%esp),%ebx
8010241b:	b8 00 00 00 00       	mov    $0x0,%eax
    unsigned char c1, c2;
    while (1) {
        c1 = *cs++;
80102420:	0f b6 14 06          	movzbl (%esi,%eax,1),%edx
        c2 = *ct++;
80102424:	0f b6 0c 03          	movzbl (%ebx,%eax,1),%ecx
        if (c1 != c2)
80102428:	38 ca                	cmp    %cl,%dl
8010242a:	75 0e                	jne    8010243a <strcmp+0x29>
            return c1 < c2 ? -1 : 1;
        if (!c1)
8010242c:	83 c0 01             	add    $0x1,%eax
8010242f:	84 d2                	test   %dl,%dl
80102431:	75 ed                	jne    80102420 <strcmp+0xf>
            break;
    }
    return 0;
80102433:	b8 00 00 00 00       	mov    $0x0,%eax
80102438:	eb 05                	jmp    8010243f <strcmp+0x2e>
            return c1 < c2 ? -1 : 1;
8010243a:	19 c0                	sbb    %eax,%eax
8010243c:	83 c8 01             	or     $0x1,%eax
}
8010243f:	5b                   	pop    %ebx
80102440:	5e                   	pop    %esi
80102441:	c3                   	ret    
