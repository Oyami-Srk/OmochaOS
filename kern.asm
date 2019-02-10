
build/kern/kernel.out:     file format elf32-i386


Disassembly of section .text:

80100000 <start>:
80100000:	0f 20 e0             	mov    %cr4,%eax
80100003:	83 c8 10             	or     $0x10,%eax
80100006:	0f 22 e0             	mov    %eax,%cr4
80100009:	b8 00 20 10 00       	mov    $0x102000,%eax
8010000e:	0f 22 d8             	mov    %eax,%cr3
80100011:	0f 20 c0             	mov    %cr0,%eax
80100014:	0d 00 00 00 80       	or     $0x80000000,%eax
80100019:	0f 22 c0             	mov    %eax,%cr0
8010001c:	bc c0 53 10 80       	mov    $0x801053c0,%esp
80100021:	b8 a3 00 10 80       	mov    $0x801000a3,%eax
80100026:	ff e0                	jmp    *%eax

80100028 <interrupt_handler>:
80100028:	8b 44 24 04          	mov    0x4(%esp),%eax
8010002c:	83 78 30 20          	cmpl   $0x20,0x30(%eax)
80100030:	74 01                	je     80100033 <interrupt_handler+0xb>
80100032:	c3                   	ret    
80100033:	b0 20                	mov    $0x20,%al
80100035:	ba 20 00 00 00       	mov    $0x20,%edx
8010003a:	ee                   	out    %al,(%dx)
8010003b:	eb f5                	jmp    80100032 <interrupt_handler+0xa>

8010003d <init_8259A>:
8010003d:	53                   	push   %ebx
8010003e:	b0 11                	mov    $0x11,%al
80100040:	ba 20 00 00 00       	mov    $0x20,%edx
80100045:	ee                   	out    %al,(%dx)
80100046:	ba a0 00 00 00       	mov    $0xa0,%edx
8010004b:	ee                   	out    %al,(%dx)
8010004c:	bb 21 00 00 00       	mov    $0x21,%ebx
80100051:	b0 20                	mov    $0x20,%al
80100053:	89 da                	mov    %ebx,%edx
80100055:	ee                   	out    %al,(%dx)
80100056:	b9 a1 00 00 00       	mov    $0xa1,%ecx
8010005b:	b0 28                	mov    $0x28,%al
8010005d:	89 ca                	mov    %ecx,%edx
8010005f:	ee                   	out    %al,(%dx)
80100060:	b0 04                	mov    $0x4,%al
80100062:	89 da                	mov    %ebx,%edx
80100064:	ee                   	out    %al,(%dx)
80100065:	b0 02                	mov    $0x2,%al
80100067:	89 ca                	mov    %ecx,%edx
80100069:	ee                   	out    %al,(%dx)
8010006a:	b0 01                	mov    $0x1,%al
8010006c:	89 da                	mov    %ebx,%edx
8010006e:	ee                   	out    %al,(%dx)
8010006f:	89 ca                	mov    %ecx,%edx
80100071:	ee                   	out    %al,(%dx)
80100072:	b0 fe                	mov    $0xfe,%al
80100074:	89 da                	mov    %ebx,%edx
80100076:	ee                   	out    %al,(%dx)
80100077:	b0 ff                	mov    $0xff,%al
80100079:	89 ca                	mov    %ecx,%edx
8010007b:	ee                   	out    %al,(%dx)
8010007c:	5b                   	pop    %ebx
8010007d:	c3                   	ret    

8010007e <test>:
8010007e:	83 ec 14             	sub    $0x14,%esp
80100081:	68 9c 10 10 80       	push   $0x8010109c
80100086:	6a 0f                	push   $0xf
80100088:	e8 d4 0e 00 00       	call   80100f61 <write_string>
8010008d:	cd 80                	int    $0x80
8010008f:	83 c4 08             	add    $0x8,%esp
80100092:	68 b0 10 10 80       	push   $0x801010b0
80100097:	6a 0f                	push   $0xf
80100099:	e8 c3 0e 00 00       	call   80100f61 <write_string>
8010009e:	83 c4 10             	add    $0x10,%esp
801000a1:	eb fe                	jmp    801000a1 <test+0x23>

801000a3 <main>:
801000a3:	8d 4c 24 04          	lea    0x4(%esp),%ecx
801000a7:	83 e4 f0             	and    $0xfffffff0,%esp
801000aa:	ff 71 fc             	pushl  -0x4(%ecx)
801000ad:	55                   	push   %ebp
801000ae:	89 e5                	mov    %esp,%ebp
801000b0:	53                   	push   %ebx
801000b1:	51                   	push   %ecx
801000b2:	83 ec 10             	sub    $0x10,%esp
801000b5:	e8 21 0f 00 00       	call   80100fdb <clr_scr>
801000ba:	66 c7 05 e0 53 10 80 	movw   $0x0,0x801053e0
801000c1:	00 00 
801000c3:	66 c7 05 e2 53 10 80 	movw   $0x0,0x801053e2
801000ca:	00 00 
801000cc:	c6 05 e4 53 10 80 00 	movb   $0x0,0x801053e4
801000d3:	c6 05 e7 53 10 80 00 	movb   $0x0,0x801053e7
801000da:	c6 05 e5 53 10 80 00 	movb   $0x0,0x801053e5
801000e1:	c6 05 e6 53 10 80 00 	movb   $0x0,0x801053e6
801000e8:	66 c7 05 e8 53 10 80 	movw   $0xffff,0x801053e8
801000ef:	ff ff 
801000f1:	66 c7 05 ea 53 10 80 	movw   $0x0,0x801053ea
801000f8:	00 00 
801000fa:	c6 05 ec 53 10 80 00 	movb   $0x0,0x801053ec
80100101:	c6 05 ef 53 10 80 00 	movb   $0x0,0x801053ef
80100108:	c6 05 ed 53 10 80 98 	movb   $0x98,0x801053ed
8010010f:	c6 05 ee 53 10 80 cf 	movb   $0xcf,0x801053ee
80100116:	66 c7 05 f0 53 10 80 	movw   $0xffff,0x801053f0
8010011d:	ff ff 
8010011f:	66 c7 05 f2 53 10 80 	movw   $0x0,0x801053f2
80100126:	00 00 
80100128:	c6 05 f4 53 10 80 00 	movb   $0x0,0x801053f4
8010012f:	c6 05 f7 53 10 80 00 	movb   $0x0,0x801053f7
80100136:	c6 05 f5 53 10 80 92 	movb   $0x92,0x801053f5
8010013d:	c6 05 f6 53 10 80 cf 	movb   $0xcf,0x801053f6
80100144:	66 c7 05 f8 53 10 80 	movw   $0xffff,0x801053f8
8010014b:	ff ff 
8010014d:	66 c7 05 fa 53 10 80 	movw   $0x8000,0x801053fa
80100154:	00 80 
80100156:	c6 05 fc 53 10 80 0b 	movb   $0xb,0x801053fc
8010015d:	c6 05 ff 53 10 80 00 	movb   $0x0,0x801053ff
80100164:	c6 05 fd 53 10 80 f2 	movb   $0xf2,0x801053fd
8010016b:	c6 05 fe 53 10 80 00 	movb   $0x0,0x801053fe
80100172:	66 c7 05 e0 57 10 80 	movw   $0x1fff,0x801057e0
80100179:	ff 1f 
8010017b:	c7 05 e2 57 10 80 e0 	movl   $0x1053e0,0x801057e2
80100182:	53 10 00 
80100185:	0f 01 15 e0 57 10 80 	lgdtl  0x801057e0
8010018c:	b8 00 00 00 00       	mov    $0x0,%eax
80100191:	8b 14 85 00 30 10 80 	mov    -0x7fefd000(,%eax,4),%edx
80100198:	88 14 c5 00 58 10 80 	mov    %dl,-0x7fefa800(,%eax,8)
8010019f:	88 34 c5 01 58 10 80 	mov    %dh,-0x7fefa7ff(,%eax,8)
801001a6:	c1 ea 10             	shr    $0x10,%edx
801001a9:	88 14 c5 06 58 10 80 	mov    %dl,-0x7fefa7fa(,%eax,8)
801001b0:	88 34 c5 07 58 10 80 	mov    %dh,-0x7fefa7f9(,%eax,8)
801001b7:	c6 04 c5 02 58 10 80 	movb   $0x8,-0x7fefa7fe(,%eax,8)
801001be:	08 
801001bf:	c6 04 c5 03 58 10 80 	movb   $0x0,-0x7fefa7fd(,%eax,8)
801001c6:	00 
801001c7:	c6 04 c5 04 58 10 80 	movb   $0x0,-0x7fefa7fc(,%eax,8)
801001ce:	00 
801001cf:	c6 04 c5 05 58 10 80 	movb   $0x8e,-0x7fefa7fb(,%eax,8)
801001d6:	8e 
801001d7:	40                   	inc    %eax
801001d8:	3d 00 01 00 00       	cmp    $0x100,%eax
801001dd:	75 b2                	jne    80100191 <main+0xee>
801001df:	66 c7 05 c0 53 10 80 	movw   $0x3fff,0x801053c0
801001e6:	ff 3f 
801001e8:	c7 05 c2 53 10 80 00 	movl   $0x105800,0x801053c2
801001ef:	58 10 00 
801001f2:	0f 01 1d c0 53 10 80 	lidtl  0x801053c0
801001f9:	e8 3f fe ff ff       	call   8010003d <init_8259A>
801001fe:	83 ec 08             	sub    $0x8,%esp
80100201:	68 a2 10 10 80       	push   $0x801010a2
80100206:	6a 0f                	push   $0xf
80100208:	e8 54 0d 00 00       	call   80100f61 <write_string>
8010020d:	83 c4 0c             	add    $0xc,%esp
80100210:	6a 0a                	push   $0xa
80100212:	8d 5d e8             	lea    -0x18(%ebp),%ebx
80100215:	53                   	push   %ebx
80100216:	6a 08                	push   $0x8
80100218:	e8 e5 0c 00 00       	call   80100f02 <itoa>
8010021d:	83 c4 08             	add    $0x8,%esp
80100220:	53                   	push   %ebx
80100221:	6a 0f                	push   $0xf
80100223:	e8 39 0d 00 00       	call   80100f61 <write_string>
80100228:	90                   	nop
80100229:	90                   	nop
8010022a:	90                   	nop
8010022b:	e8 4e fe ff ff       	call   8010007e <test>

80100230 <found_tbl_by_va>:
80100230:	8b 44 24 04          	mov    0x4(%esp),%eax
80100234:	c1 e8 16             	shr    $0x16,%eax
80100237:	c1 e0 02             	shl    $0x2,%eax
8010023a:	f6 80 00 0c 20 80 01 	testb  $0x1,-0x7fdff400(%eax)
80100241:	75 0a                	jne    8010024d <found_tbl_by_va+0x1d>
80100243:	c7 80 00 0c 20 80 00 	movl   $0x80201c00,-0x7fdff400(%eax)
8010024a:	1c 20 80 
8010024d:	8b 80 00 0c 20 80    	mov    -0x7fdff400(%eax),%eax
80100253:	c3                   	ret    

80100254 <vm_map>:
80100254:	55                   	push   %ebp
80100255:	57                   	push   %edi
80100256:	56                   	push   %esi
80100257:	53                   	push   %ebx
80100258:	83 ec 1c             	sub    $0x1c,%esp
8010025b:	8b 44 24 30          	mov    0x30(%esp),%eax
8010025f:	89 c5                	mov    %eax,%ebp
80100261:	81 e5 00 f0 ff ff    	and    $0xfffff000,%ebp
80100267:	8b 54 24 34          	mov    0x34(%esp),%edx
8010026b:	8d 74 10 ff          	lea    -0x1(%eax,%edx,1),%esi
8010026f:	81 e6 00 f0 ff ff    	and    $0xfffff000,%esi
80100275:	55                   	push   %ebp
80100276:	e8 b5 ff ff ff       	call   80100230 <found_tbl_by_va>
8010027b:	6a 10                	push   $0x10
8010027d:	8d 7c 24 08          	lea    0x8(%esp),%edi
80100281:	57                   	push   %edi
80100282:	50                   	push   %eax
80100283:	e8 7a 0c 00 00       	call   80100f02 <itoa>
80100288:	83 c4 08             	add    $0x8,%esp
8010028b:	57                   	push   %edi
8010028c:	6a 0f                	push   $0xf
8010028e:	e8 ce 0c 00 00       	call   80100f61 <write_string>
80100293:	83 c4 08             	add    $0x8,%esp
80100296:	68 ae 10 10 80       	push   $0x801010ae
8010029b:	6a 0f                	push   $0xf
8010029d:	e8 bf 0c 00 00       	call   80100f61 <write_string>
801002a2:	83 c4 10             	add    $0x10,%esp
801002a5:	39 f5                	cmp    %esi,%ebp
801002a7:	77 25                	ja     801002ce <vm_map+0x7a>
801002a9:	89 eb                	mov    %ebp,%ebx
801002ab:	8b 7c 24 38          	mov    0x38(%esp),%edi
801002af:	83 cf 07             	or     $0x7,%edi
801002b2:	29 ee                	sub    %ebp,%esi
801002b4:	83 ec 0c             	sub    $0xc,%esp
801002b7:	53                   	push   %ebx
801002b8:	e8 73 ff ff ff       	call   80100230 <found_tbl_by_va>
801002bd:	89 38                	mov    %edi,(%eax)
801002bf:	81 c3 00 40 00 00    	add    $0x4000,%ebx
801002c5:	83 c4 10             	add    $0x10,%esp
801002c8:	89 d8                	mov    %ebx,%eax
801002ca:	01 f0                	add    %esi,%eax
801002cc:	73 e6                	jae    801002b4 <vm_map+0x60>
801002ce:	83 c4 1c             	add    $0x1c,%esp
801002d1:	5b                   	pop    %ebx
801002d2:	5e                   	pop    %esi
801002d3:	5f                   	pop    %edi
801002d4:	5d                   	pop    %ebp
801002d5:	c3                   	ret    

801002d6 <kinit_paging>:
801002d6:	57                   	push   %edi
801002d7:	56                   	push   %esi
801002d8:	53                   	push   %ebx
801002d9:	83 ec 10             	sub    $0x10,%esp
801002dc:	8b 54 24 20          	mov    0x20(%esp),%edx
801002e0:	8b 4c 24 24          	mov    0x24(%esp),%ecx
801002e4:	8d 04 12             	lea    (%edx,%edx,1),%eax
801002e7:	01 d0                	add    %edx,%eax
801002e9:	8d 34 c1             	lea    (%ecx,%eax,8),%esi
801002ec:	39 f1                	cmp    %esi,%ecx
801002ee:	73 2b                	jae    8010031b <kinit_paging+0x45>
801002f0:	bb 00 00 00 00       	mov    $0x0,%ebx
801002f5:	eb 07                	jmp    801002fe <kinit_paging+0x28>
801002f7:	83 c1 18             	add    $0x18,%ecx
801002fa:	39 f1                	cmp    %esi,%ecx
801002fc:	73 22                	jae    80100320 <kinit_paging+0x4a>
801002fe:	8b 41 04             	mov    0x4(%ecx),%eax
80100301:	c1 e0 10             	shl    $0x10,%eax
80100304:	0b 01                	or     (%ecx),%eax
80100306:	89 c2                	mov    %eax,%edx
80100308:	8b 41 0c             	mov    0xc(%ecx),%eax
8010030b:	c1 e0 10             	shl    $0x10,%eax
8010030e:	0b 41 08             	or     0x8(%ecx),%eax
80100311:	01 d0                	add    %edx,%eax
80100313:	39 c3                	cmp    %eax,%ebx
80100315:	73 e0                	jae    801002f7 <kinit_paging+0x21>
80100317:	89 c3                	mov    %eax,%ebx
80100319:	eb dc                	jmp    801002f7 <kinit_paging+0x21>
8010031b:	bb 00 00 00 00       	mov    $0x0,%ebx
80100320:	83 ec 08             	sub    $0x8,%esp
80100323:	68 d0 10 10 80       	push   $0x801010d0
80100328:	6a 0f                	push   $0xf
8010032a:	e8 32 0c 00 00       	call   80100f61 <write_string>
8010032f:	83 c4 0c             	add    $0xc,%esp
80100332:	6a 0a                	push   $0xa
80100334:	8d 7c 24 08          	lea    0x8(%esp),%edi
80100338:	57                   	push   %edi
80100339:	89 d8                	mov    %ebx,%eax
8010033b:	c1 e8 14             	shr    $0x14,%eax
8010033e:	50                   	push   %eax
8010033f:	e8 be 0b 00 00       	call   80100f02 <itoa>
80100344:	83 c4 08             	add    $0x8,%esp
80100347:	57                   	push   %edi
80100348:	6a 0f                	push   $0xf
8010034a:	e8 12 0c 00 00       	call   80100f61 <write_string>
8010034f:	83 c4 08             	add    $0x8,%esp
80100352:	68 e0 10 10 80       	push   $0x801010e0
80100357:	6a 0f                	push   $0xf
80100359:	e8 03 0c 00 00       	call   80100f61 <write_string>
8010035e:	83 c4 0c             	add    $0xc,%esp
80100361:	f7 c3 ff ff 3f 00    	test   $0x3fffff,%ebx
80100367:	0f 95 c0             	setne  %al
8010036a:	0f b6 c0             	movzbl %al,%eax
8010036d:	c1 eb 16             	shr    $0x16,%ebx
80100370:	01 c3                	add    %eax,%ebx
80100372:	89 de                	mov    %ebx,%esi
80100374:	6a 0a                	push   $0xa
80100376:	57                   	push   %edi
80100377:	53                   	push   %ebx
80100378:	e8 85 0b 00 00       	call   80100f02 <itoa>
8010037d:	83 c4 08             	add    $0x8,%esp
80100380:	68 e5 10 10 80       	push   $0x801010e5
80100385:	6a 0f                	push   $0xf
80100387:	e8 d5 0b 00 00       	call   80100f61 <write_string>
8010038c:	83 c4 08             	add    $0x8,%esp
8010038f:	57                   	push   %edi
80100390:	6a 0f                	push   $0xf
80100392:	e8 ca 0b 00 00       	call   80100f61 <write_string>
80100397:	83 c4 08             	add    $0x8,%esp
8010039a:	68 ee 10 10 80       	push   $0x801010ee
8010039f:	6a 0f                	push   $0xf
801003a1:	e8 bb 0b 00 00       	call   80100f61 <write_string>
801003a6:	83 c4 10             	add    $0x10,%esp
801003a9:	85 db                	test   %ebx,%ebx
801003ab:	74 43                	je     801003f0 <kinit_paging+0x11a>
801003ad:	b9 00 00 00 00       	mov    $0x0,%ecx
801003b2:	ba 07 1c 20 00       	mov    $0x201c07,%edx
801003b7:	b8 00 0c 20 80       	mov    $0x80200c00,%eax
801003bc:	89 10                	mov    %edx,(%eax)
801003be:	81 c2 00 10 00 00    	add    $0x1000,%edx
801003c4:	41                   	inc    %ecx
801003c5:	8d 40 04             	lea    0x4(%eax),%eax
801003c8:	39 ce                	cmp    %ecx,%esi
801003ca:	75 f0                	jne    801003bc <kinit_paging+0xe6>
801003cc:	c1 e3 0a             	shl    $0xa,%ebx
801003cf:	74 1f                	je     801003f0 <kinit_paging+0x11a>
801003d1:	b9 00 00 00 00       	mov    $0x0,%ecx
801003d6:	ba 07 00 00 00       	mov    $0x7,%edx
801003db:	b8 00 1c 20 80       	mov    $0x80201c00,%eax
801003e0:	89 10                	mov    %edx,(%eax)
801003e2:	81 c2 00 10 00 00    	add    $0x1000,%edx
801003e8:	41                   	inc    %ecx
801003e9:	8d 40 04             	lea    0x4(%eax),%eax
801003ec:	39 cb                	cmp    %ecx,%ebx
801003ee:	75 f0                	jne    801003e0 <kinit_paging+0x10a>
801003f0:	83 ec 08             	sub    $0x8,%esp
801003f3:	68 0c 11 10 80       	push   $0x8010110c
801003f8:	6a 0f                	push   $0xf
801003fa:	e8 62 0b 00 00       	call   80100f61 <write_string>
801003ff:	83 c4 0c             	add    $0xc,%esp
80100402:	6a 00                	push   $0x0
80100404:	68 00 00 00 0f       	push   $0xf000000
80100409:	68 00 00 00 80       	push   $0x80000000
8010040e:	e8 41 fe ff ff       	call   80100254 <vm_map>
80100413:	83 c4 08             	add    $0x8,%esp
80100416:	68 21 11 10 80       	push   $0x80101121
8010041b:	6a 0f                	push   $0xf
8010041d:	e8 3f 0b 00 00       	call   80100f61 <write_string>
80100422:	83 c4 0c             	add    $0xc,%esp
80100425:	6a 10                	push   $0x10
80100427:	8d 5c 24 08          	lea    0x8(%esp),%ebx
8010042b:	53                   	push   %ebx
8010042c:	68 00 0c 20 00       	push   $0x200c00
80100431:	e8 cc 0a 00 00       	call   80100f02 <itoa>
80100436:	83 c4 08             	add    $0x8,%esp
80100439:	53                   	push   %ebx
8010043a:	6a 0f                	push   $0xf
8010043c:	e8 20 0b 00 00       	call   80100f61 <write_string>
80100441:	83 c4 08             	add    $0x8,%esp
80100444:	68 ae 10 10 80       	push   $0x801010ae
80100449:	6a 0f                	push   $0xf
8010044b:	e8 11 0b 00 00       	call   80100f61 <write_string>
80100450:	b8 00 0c 20 00       	mov    $0x200c00,%eax
80100455:	0f 22 d8             	mov    %eax,%cr3
80100458:	83 c4 20             	add    $0x20,%esp
8010045b:	5b                   	pop    %ebx
8010045c:	5e                   	pop    %esi
8010045d:	5f                   	pop    %edi
8010045e:	c3                   	ret    

8010045f <kinit_gdt>:
8010045f:	c3                   	ret    

80100460 <vector_handler>:
80100460:	60                   	pusha  
80100461:	1e                   	push   %ds
80100462:	06                   	push   %es
80100463:	0f a0                	push   %fs
80100465:	0f a8                	push   %gs
80100467:	89 d6                	mov    %edx,%esi
80100469:	66 b8 10 00          	mov    $0x10,%ax
8010046d:	8e d8                	mov    %eax,%ds
8010046f:	8e c0                	mov    %eax,%es
80100471:	89 f2                	mov    %esi,%edx
80100473:	54                   	push   %esp
80100474:	e8 af fb ff ff       	call   80100028 <interrupt_handler>
80100479:	83 c4 04             	add    $0x4,%esp

8010047c <vector_handler.vector_handler_ret>:
8010047c:	0f a9                	pop    %gs
8010047e:	0f a1                	pop    %fs
80100480:	07                   	pop    %es
80100481:	1f                   	pop    %ds
80100482:	61                   	popa   
80100483:	83 c4 08             	add    $0x8,%esp
80100486:	cf                   	iret   
80100487:	66 90                	xchg   %ax,%ax
80100489:	66 90                	xchg   %ax,%ax
8010048b:	66 90                	xchg   %ax,%ax
8010048d:	66 90                	xchg   %ax,%ax
8010048f:	90                   	nop

80100490 <vector0>:
80100490:	6a 00                	push   $0x0
80100492:	6a 00                	push   $0x0
80100494:	e9 c7 ff ff ff       	jmp    80100460 <vector_handler>

80100499 <vector1>:
80100499:	6a 00                	push   $0x0
8010049b:	6a 01                	push   $0x1
8010049d:	e9 be ff ff ff       	jmp    80100460 <vector_handler>

801004a2 <vector2>:
801004a2:	6a 00                	push   $0x0
801004a4:	6a 02                	push   $0x2
801004a6:	e9 b5 ff ff ff       	jmp    80100460 <vector_handler>

801004ab <vector3>:
801004ab:	6a 00                	push   $0x0
801004ad:	6a 03                	push   $0x3
801004af:	e9 ac ff ff ff       	jmp    80100460 <vector_handler>

801004b4 <vector4>:
801004b4:	6a 00                	push   $0x0
801004b6:	6a 04                	push   $0x4
801004b8:	e9 a3 ff ff ff       	jmp    80100460 <vector_handler>

801004bd <vector5>:
801004bd:	6a 00                	push   $0x0
801004bf:	6a 05                	push   $0x5
801004c1:	e9 9a ff ff ff       	jmp    80100460 <vector_handler>

801004c6 <vector6>:
801004c6:	6a 00                	push   $0x0
801004c8:	6a 06                	push   $0x6
801004ca:	e9 91 ff ff ff       	jmp    80100460 <vector_handler>

801004cf <vector7>:
801004cf:	6a 00                	push   $0x0
801004d1:	6a 07                	push   $0x7
801004d3:	e9 88 ff ff ff       	jmp    80100460 <vector_handler>

801004d8 <vector8>:
801004d8:	6a 08                	push   $0x8
801004da:	e9 81 ff ff ff       	jmp    80100460 <vector_handler>

801004df <vector9>:
801004df:	6a 00                	push   $0x0
801004e1:	6a 09                	push   $0x9
801004e3:	e9 78 ff ff ff       	jmp    80100460 <vector_handler>

801004e8 <vector10>:
801004e8:	6a 0a                	push   $0xa
801004ea:	e9 71 ff ff ff       	jmp    80100460 <vector_handler>

801004ef <vector11>:
801004ef:	6a 0b                	push   $0xb
801004f1:	e9 6a ff ff ff       	jmp    80100460 <vector_handler>

801004f6 <vector12>:
801004f6:	6a 0c                	push   $0xc
801004f8:	e9 63 ff ff ff       	jmp    80100460 <vector_handler>

801004fd <vector13>:
801004fd:	6a 0d                	push   $0xd
801004ff:	e9 5c ff ff ff       	jmp    80100460 <vector_handler>

80100504 <vector14>:
80100504:	6a 0e                	push   $0xe
80100506:	e9 55 ff ff ff       	jmp    80100460 <vector_handler>

8010050b <vector15>:
8010050b:	6a 00                	push   $0x0
8010050d:	6a 0f                	push   $0xf
8010050f:	e9 4c ff ff ff       	jmp    80100460 <vector_handler>

80100514 <vector16>:
80100514:	6a 00                	push   $0x0
80100516:	6a 10                	push   $0x10
80100518:	e9 43 ff ff ff       	jmp    80100460 <vector_handler>

8010051d <vector17>:
8010051d:	6a 11                	push   $0x11
8010051f:	e9 3c ff ff ff       	jmp    80100460 <vector_handler>

80100524 <vector18>:
80100524:	6a 00                	push   $0x0
80100526:	6a 12                	push   $0x12
80100528:	e9 33 ff ff ff       	jmp    80100460 <vector_handler>

8010052d <vector19>:
8010052d:	6a 00                	push   $0x0
8010052f:	6a 13                	push   $0x13
80100531:	e9 2a ff ff ff       	jmp    80100460 <vector_handler>

80100536 <vector20>:
80100536:	6a 00                	push   $0x0
80100538:	6a 14                	push   $0x14
8010053a:	e9 21 ff ff ff       	jmp    80100460 <vector_handler>

8010053f <vector21>:
8010053f:	6a 00                	push   $0x0
80100541:	6a 15                	push   $0x15
80100543:	e9 18 ff ff ff       	jmp    80100460 <vector_handler>

80100548 <vector22>:
80100548:	6a 00                	push   $0x0
8010054a:	6a 16                	push   $0x16
8010054c:	e9 0f ff ff ff       	jmp    80100460 <vector_handler>

80100551 <vector23>:
80100551:	6a 00                	push   $0x0
80100553:	6a 17                	push   $0x17
80100555:	e9 06 ff ff ff       	jmp    80100460 <vector_handler>

8010055a <vector24>:
8010055a:	6a 00                	push   $0x0
8010055c:	6a 18                	push   $0x18
8010055e:	e9 fd fe ff ff       	jmp    80100460 <vector_handler>

80100563 <vector25>:
80100563:	6a 00                	push   $0x0
80100565:	6a 19                	push   $0x19
80100567:	e9 f4 fe ff ff       	jmp    80100460 <vector_handler>

8010056c <vector26>:
8010056c:	6a 00                	push   $0x0
8010056e:	6a 1a                	push   $0x1a
80100570:	e9 eb fe ff ff       	jmp    80100460 <vector_handler>

80100575 <vector27>:
80100575:	6a 00                	push   $0x0
80100577:	6a 1b                	push   $0x1b
80100579:	e9 e2 fe ff ff       	jmp    80100460 <vector_handler>

8010057e <vector28>:
8010057e:	6a 00                	push   $0x0
80100580:	6a 1c                	push   $0x1c
80100582:	e9 d9 fe ff ff       	jmp    80100460 <vector_handler>

80100587 <vector29>:
80100587:	6a 00                	push   $0x0
80100589:	6a 1d                	push   $0x1d
8010058b:	e9 d0 fe ff ff       	jmp    80100460 <vector_handler>

80100590 <vector30>:
80100590:	6a 00                	push   $0x0
80100592:	6a 1e                	push   $0x1e
80100594:	e9 c7 fe ff ff       	jmp    80100460 <vector_handler>

80100599 <vector31>:
80100599:	6a 00                	push   $0x0
8010059b:	6a 1f                	push   $0x1f
8010059d:	e9 be fe ff ff       	jmp    80100460 <vector_handler>

801005a2 <vector32>:
801005a2:	6a 00                	push   $0x0
801005a4:	6a 20                	push   $0x20
801005a6:	e9 b5 fe ff ff       	jmp    80100460 <vector_handler>

801005ab <vector33>:
801005ab:	6a 00                	push   $0x0
801005ad:	6a 21                	push   $0x21
801005af:	e9 ac fe ff ff       	jmp    80100460 <vector_handler>

801005b4 <vector34>:
801005b4:	6a 00                	push   $0x0
801005b6:	6a 22                	push   $0x22
801005b8:	e9 a3 fe ff ff       	jmp    80100460 <vector_handler>

801005bd <vector35>:
801005bd:	6a 00                	push   $0x0
801005bf:	6a 23                	push   $0x23
801005c1:	e9 9a fe ff ff       	jmp    80100460 <vector_handler>

801005c6 <vector36>:
801005c6:	6a 00                	push   $0x0
801005c8:	6a 24                	push   $0x24
801005ca:	e9 91 fe ff ff       	jmp    80100460 <vector_handler>

801005cf <vector37>:
801005cf:	6a 00                	push   $0x0
801005d1:	6a 25                	push   $0x25
801005d3:	e9 88 fe ff ff       	jmp    80100460 <vector_handler>

801005d8 <vector38>:
801005d8:	6a 00                	push   $0x0
801005da:	6a 26                	push   $0x26
801005dc:	e9 7f fe ff ff       	jmp    80100460 <vector_handler>

801005e1 <vector39>:
801005e1:	6a 00                	push   $0x0
801005e3:	6a 27                	push   $0x27
801005e5:	e9 76 fe ff ff       	jmp    80100460 <vector_handler>

801005ea <vector40>:
801005ea:	6a 00                	push   $0x0
801005ec:	6a 28                	push   $0x28
801005ee:	e9 6d fe ff ff       	jmp    80100460 <vector_handler>

801005f3 <vector41>:
801005f3:	6a 00                	push   $0x0
801005f5:	6a 29                	push   $0x29
801005f7:	e9 64 fe ff ff       	jmp    80100460 <vector_handler>

801005fc <vector42>:
801005fc:	6a 00                	push   $0x0
801005fe:	6a 2a                	push   $0x2a
80100600:	e9 5b fe ff ff       	jmp    80100460 <vector_handler>

80100605 <vector43>:
80100605:	6a 00                	push   $0x0
80100607:	6a 2b                	push   $0x2b
80100609:	e9 52 fe ff ff       	jmp    80100460 <vector_handler>

8010060e <vector44>:
8010060e:	6a 00                	push   $0x0
80100610:	6a 2c                	push   $0x2c
80100612:	e9 49 fe ff ff       	jmp    80100460 <vector_handler>

80100617 <vector45>:
80100617:	6a 00                	push   $0x0
80100619:	6a 2d                	push   $0x2d
8010061b:	e9 40 fe ff ff       	jmp    80100460 <vector_handler>

80100620 <vector46>:
80100620:	6a 00                	push   $0x0
80100622:	6a 2e                	push   $0x2e
80100624:	e9 37 fe ff ff       	jmp    80100460 <vector_handler>

80100629 <vector47>:
80100629:	6a 00                	push   $0x0
8010062b:	6a 2f                	push   $0x2f
8010062d:	e9 2e fe ff ff       	jmp    80100460 <vector_handler>

80100632 <vector48>:
80100632:	6a 00                	push   $0x0
80100634:	6a 30                	push   $0x30
80100636:	e9 25 fe ff ff       	jmp    80100460 <vector_handler>

8010063b <vector49>:
8010063b:	6a 00                	push   $0x0
8010063d:	6a 31                	push   $0x31
8010063f:	e9 1c fe ff ff       	jmp    80100460 <vector_handler>

80100644 <vector50>:
80100644:	6a 00                	push   $0x0
80100646:	6a 32                	push   $0x32
80100648:	e9 13 fe ff ff       	jmp    80100460 <vector_handler>

8010064d <vector51>:
8010064d:	6a 00                	push   $0x0
8010064f:	6a 33                	push   $0x33
80100651:	e9 0a fe ff ff       	jmp    80100460 <vector_handler>

80100656 <vector52>:
80100656:	6a 00                	push   $0x0
80100658:	6a 34                	push   $0x34
8010065a:	e9 01 fe ff ff       	jmp    80100460 <vector_handler>

8010065f <vector53>:
8010065f:	6a 00                	push   $0x0
80100661:	6a 35                	push   $0x35
80100663:	e9 f8 fd ff ff       	jmp    80100460 <vector_handler>

80100668 <vector54>:
80100668:	6a 00                	push   $0x0
8010066a:	6a 36                	push   $0x36
8010066c:	e9 ef fd ff ff       	jmp    80100460 <vector_handler>

80100671 <vector55>:
80100671:	6a 00                	push   $0x0
80100673:	6a 37                	push   $0x37
80100675:	e9 e6 fd ff ff       	jmp    80100460 <vector_handler>

8010067a <vector56>:
8010067a:	6a 00                	push   $0x0
8010067c:	6a 38                	push   $0x38
8010067e:	e9 dd fd ff ff       	jmp    80100460 <vector_handler>

80100683 <vector57>:
80100683:	6a 00                	push   $0x0
80100685:	6a 39                	push   $0x39
80100687:	e9 d4 fd ff ff       	jmp    80100460 <vector_handler>

8010068c <vector58>:
8010068c:	6a 00                	push   $0x0
8010068e:	6a 3a                	push   $0x3a
80100690:	e9 cb fd ff ff       	jmp    80100460 <vector_handler>

80100695 <vector59>:
80100695:	6a 00                	push   $0x0
80100697:	6a 3b                	push   $0x3b
80100699:	e9 c2 fd ff ff       	jmp    80100460 <vector_handler>

8010069e <vector60>:
8010069e:	6a 00                	push   $0x0
801006a0:	6a 3c                	push   $0x3c
801006a2:	e9 b9 fd ff ff       	jmp    80100460 <vector_handler>

801006a7 <vector61>:
801006a7:	6a 00                	push   $0x0
801006a9:	6a 3d                	push   $0x3d
801006ab:	e9 b0 fd ff ff       	jmp    80100460 <vector_handler>

801006b0 <vector62>:
801006b0:	6a 00                	push   $0x0
801006b2:	6a 3e                	push   $0x3e
801006b4:	e9 a7 fd ff ff       	jmp    80100460 <vector_handler>

801006b9 <vector63>:
801006b9:	6a 00                	push   $0x0
801006bb:	6a 3f                	push   $0x3f
801006bd:	e9 9e fd ff ff       	jmp    80100460 <vector_handler>

801006c2 <vector64>:
801006c2:	6a 00                	push   $0x0
801006c4:	6a 40                	push   $0x40
801006c6:	e9 95 fd ff ff       	jmp    80100460 <vector_handler>

801006cb <vector65>:
801006cb:	6a 00                	push   $0x0
801006cd:	6a 41                	push   $0x41
801006cf:	e9 8c fd ff ff       	jmp    80100460 <vector_handler>

801006d4 <vector66>:
801006d4:	6a 00                	push   $0x0
801006d6:	6a 42                	push   $0x42
801006d8:	e9 83 fd ff ff       	jmp    80100460 <vector_handler>

801006dd <vector67>:
801006dd:	6a 00                	push   $0x0
801006df:	6a 43                	push   $0x43
801006e1:	e9 7a fd ff ff       	jmp    80100460 <vector_handler>

801006e6 <vector68>:
801006e6:	6a 00                	push   $0x0
801006e8:	6a 44                	push   $0x44
801006ea:	e9 71 fd ff ff       	jmp    80100460 <vector_handler>

801006ef <vector69>:
801006ef:	6a 00                	push   $0x0
801006f1:	6a 45                	push   $0x45
801006f3:	e9 68 fd ff ff       	jmp    80100460 <vector_handler>

801006f8 <vector70>:
801006f8:	6a 00                	push   $0x0
801006fa:	6a 46                	push   $0x46
801006fc:	e9 5f fd ff ff       	jmp    80100460 <vector_handler>

80100701 <vector71>:
80100701:	6a 00                	push   $0x0
80100703:	6a 47                	push   $0x47
80100705:	e9 56 fd ff ff       	jmp    80100460 <vector_handler>

8010070a <vector72>:
8010070a:	6a 00                	push   $0x0
8010070c:	6a 48                	push   $0x48
8010070e:	e9 4d fd ff ff       	jmp    80100460 <vector_handler>

80100713 <vector73>:
80100713:	6a 00                	push   $0x0
80100715:	6a 49                	push   $0x49
80100717:	e9 44 fd ff ff       	jmp    80100460 <vector_handler>

8010071c <vector74>:
8010071c:	6a 00                	push   $0x0
8010071e:	6a 4a                	push   $0x4a
80100720:	e9 3b fd ff ff       	jmp    80100460 <vector_handler>

80100725 <vector75>:
80100725:	6a 00                	push   $0x0
80100727:	6a 4b                	push   $0x4b
80100729:	e9 32 fd ff ff       	jmp    80100460 <vector_handler>

8010072e <vector76>:
8010072e:	6a 00                	push   $0x0
80100730:	6a 4c                	push   $0x4c
80100732:	e9 29 fd ff ff       	jmp    80100460 <vector_handler>

80100737 <vector77>:
80100737:	6a 00                	push   $0x0
80100739:	6a 4d                	push   $0x4d
8010073b:	e9 20 fd ff ff       	jmp    80100460 <vector_handler>

80100740 <vector78>:
80100740:	6a 00                	push   $0x0
80100742:	6a 4e                	push   $0x4e
80100744:	e9 17 fd ff ff       	jmp    80100460 <vector_handler>

80100749 <vector79>:
80100749:	6a 00                	push   $0x0
8010074b:	6a 4f                	push   $0x4f
8010074d:	e9 0e fd ff ff       	jmp    80100460 <vector_handler>

80100752 <vector80>:
80100752:	6a 00                	push   $0x0
80100754:	6a 50                	push   $0x50
80100756:	e9 05 fd ff ff       	jmp    80100460 <vector_handler>

8010075b <vector81>:
8010075b:	6a 00                	push   $0x0
8010075d:	6a 51                	push   $0x51
8010075f:	e9 fc fc ff ff       	jmp    80100460 <vector_handler>

80100764 <vector82>:
80100764:	6a 00                	push   $0x0
80100766:	6a 52                	push   $0x52
80100768:	e9 f3 fc ff ff       	jmp    80100460 <vector_handler>

8010076d <vector83>:
8010076d:	6a 00                	push   $0x0
8010076f:	6a 53                	push   $0x53
80100771:	e9 ea fc ff ff       	jmp    80100460 <vector_handler>

80100776 <vector84>:
80100776:	6a 00                	push   $0x0
80100778:	6a 54                	push   $0x54
8010077a:	e9 e1 fc ff ff       	jmp    80100460 <vector_handler>

8010077f <vector85>:
8010077f:	6a 00                	push   $0x0
80100781:	6a 55                	push   $0x55
80100783:	e9 d8 fc ff ff       	jmp    80100460 <vector_handler>

80100788 <vector86>:
80100788:	6a 00                	push   $0x0
8010078a:	6a 56                	push   $0x56
8010078c:	e9 cf fc ff ff       	jmp    80100460 <vector_handler>

80100791 <vector87>:
80100791:	6a 00                	push   $0x0
80100793:	6a 57                	push   $0x57
80100795:	e9 c6 fc ff ff       	jmp    80100460 <vector_handler>

8010079a <vector88>:
8010079a:	6a 00                	push   $0x0
8010079c:	6a 58                	push   $0x58
8010079e:	e9 bd fc ff ff       	jmp    80100460 <vector_handler>

801007a3 <vector89>:
801007a3:	6a 00                	push   $0x0
801007a5:	6a 59                	push   $0x59
801007a7:	e9 b4 fc ff ff       	jmp    80100460 <vector_handler>

801007ac <vector90>:
801007ac:	6a 00                	push   $0x0
801007ae:	6a 5a                	push   $0x5a
801007b0:	e9 ab fc ff ff       	jmp    80100460 <vector_handler>

801007b5 <vector91>:
801007b5:	6a 00                	push   $0x0
801007b7:	6a 5b                	push   $0x5b
801007b9:	e9 a2 fc ff ff       	jmp    80100460 <vector_handler>

801007be <vector92>:
801007be:	6a 00                	push   $0x0
801007c0:	6a 5c                	push   $0x5c
801007c2:	e9 99 fc ff ff       	jmp    80100460 <vector_handler>

801007c7 <vector93>:
801007c7:	6a 00                	push   $0x0
801007c9:	6a 5d                	push   $0x5d
801007cb:	e9 90 fc ff ff       	jmp    80100460 <vector_handler>

801007d0 <vector94>:
801007d0:	6a 00                	push   $0x0
801007d2:	6a 5e                	push   $0x5e
801007d4:	e9 87 fc ff ff       	jmp    80100460 <vector_handler>

801007d9 <vector95>:
801007d9:	6a 00                	push   $0x0
801007db:	6a 5f                	push   $0x5f
801007dd:	e9 7e fc ff ff       	jmp    80100460 <vector_handler>

801007e2 <vector96>:
801007e2:	6a 00                	push   $0x0
801007e4:	6a 60                	push   $0x60
801007e6:	e9 75 fc ff ff       	jmp    80100460 <vector_handler>

801007eb <vector97>:
801007eb:	6a 00                	push   $0x0
801007ed:	6a 61                	push   $0x61
801007ef:	e9 6c fc ff ff       	jmp    80100460 <vector_handler>

801007f4 <vector98>:
801007f4:	6a 00                	push   $0x0
801007f6:	6a 62                	push   $0x62
801007f8:	e9 63 fc ff ff       	jmp    80100460 <vector_handler>

801007fd <vector99>:
801007fd:	6a 00                	push   $0x0
801007ff:	6a 63                	push   $0x63
80100801:	e9 5a fc ff ff       	jmp    80100460 <vector_handler>

80100806 <vector100>:
80100806:	6a 00                	push   $0x0
80100808:	6a 64                	push   $0x64
8010080a:	e9 51 fc ff ff       	jmp    80100460 <vector_handler>

8010080f <vector101>:
8010080f:	6a 00                	push   $0x0
80100811:	6a 65                	push   $0x65
80100813:	e9 48 fc ff ff       	jmp    80100460 <vector_handler>

80100818 <vector102>:
80100818:	6a 00                	push   $0x0
8010081a:	6a 66                	push   $0x66
8010081c:	e9 3f fc ff ff       	jmp    80100460 <vector_handler>

80100821 <vector103>:
80100821:	6a 00                	push   $0x0
80100823:	6a 67                	push   $0x67
80100825:	e9 36 fc ff ff       	jmp    80100460 <vector_handler>

8010082a <vector104>:
8010082a:	6a 00                	push   $0x0
8010082c:	6a 68                	push   $0x68
8010082e:	e9 2d fc ff ff       	jmp    80100460 <vector_handler>

80100833 <vector105>:
80100833:	6a 00                	push   $0x0
80100835:	6a 69                	push   $0x69
80100837:	e9 24 fc ff ff       	jmp    80100460 <vector_handler>

8010083c <vector106>:
8010083c:	6a 00                	push   $0x0
8010083e:	6a 6a                	push   $0x6a
80100840:	e9 1b fc ff ff       	jmp    80100460 <vector_handler>

80100845 <vector107>:
80100845:	6a 00                	push   $0x0
80100847:	6a 6b                	push   $0x6b
80100849:	e9 12 fc ff ff       	jmp    80100460 <vector_handler>

8010084e <vector108>:
8010084e:	6a 00                	push   $0x0
80100850:	6a 6c                	push   $0x6c
80100852:	e9 09 fc ff ff       	jmp    80100460 <vector_handler>

80100857 <vector109>:
80100857:	6a 00                	push   $0x0
80100859:	6a 6d                	push   $0x6d
8010085b:	e9 00 fc ff ff       	jmp    80100460 <vector_handler>

80100860 <vector110>:
80100860:	6a 00                	push   $0x0
80100862:	6a 6e                	push   $0x6e
80100864:	e9 f7 fb ff ff       	jmp    80100460 <vector_handler>

80100869 <vector111>:
80100869:	6a 00                	push   $0x0
8010086b:	6a 6f                	push   $0x6f
8010086d:	e9 ee fb ff ff       	jmp    80100460 <vector_handler>

80100872 <vector112>:
80100872:	6a 00                	push   $0x0
80100874:	6a 70                	push   $0x70
80100876:	e9 e5 fb ff ff       	jmp    80100460 <vector_handler>

8010087b <vector113>:
8010087b:	6a 00                	push   $0x0
8010087d:	6a 71                	push   $0x71
8010087f:	e9 dc fb ff ff       	jmp    80100460 <vector_handler>

80100884 <vector114>:
80100884:	6a 00                	push   $0x0
80100886:	6a 72                	push   $0x72
80100888:	e9 d3 fb ff ff       	jmp    80100460 <vector_handler>

8010088d <vector115>:
8010088d:	6a 00                	push   $0x0
8010088f:	6a 73                	push   $0x73
80100891:	e9 ca fb ff ff       	jmp    80100460 <vector_handler>

80100896 <vector116>:
80100896:	6a 00                	push   $0x0
80100898:	6a 74                	push   $0x74
8010089a:	e9 c1 fb ff ff       	jmp    80100460 <vector_handler>

8010089f <vector117>:
8010089f:	6a 00                	push   $0x0
801008a1:	6a 75                	push   $0x75
801008a3:	e9 b8 fb ff ff       	jmp    80100460 <vector_handler>

801008a8 <vector118>:
801008a8:	6a 00                	push   $0x0
801008aa:	6a 76                	push   $0x76
801008ac:	e9 af fb ff ff       	jmp    80100460 <vector_handler>

801008b1 <vector119>:
801008b1:	6a 00                	push   $0x0
801008b3:	6a 77                	push   $0x77
801008b5:	e9 a6 fb ff ff       	jmp    80100460 <vector_handler>

801008ba <vector120>:
801008ba:	6a 00                	push   $0x0
801008bc:	6a 78                	push   $0x78
801008be:	e9 9d fb ff ff       	jmp    80100460 <vector_handler>

801008c3 <vector121>:
801008c3:	6a 00                	push   $0x0
801008c5:	6a 79                	push   $0x79
801008c7:	e9 94 fb ff ff       	jmp    80100460 <vector_handler>

801008cc <vector122>:
801008cc:	6a 00                	push   $0x0
801008ce:	6a 7a                	push   $0x7a
801008d0:	e9 8b fb ff ff       	jmp    80100460 <vector_handler>

801008d5 <vector123>:
801008d5:	6a 00                	push   $0x0
801008d7:	6a 7b                	push   $0x7b
801008d9:	e9 82 fb ff ff       	jmp    80100460 <vector_handler>

801008de <vector124>:
801008de:	6a 00                	push   $0x0
801008e0:	6a 7c                	push   $0x7c
801008e2:	e9 79 fb ff ff       	jmp    80100460 <vector_handler>

801008e7 <vector125>:
801008e7:	6a 00                	push   $0x0
801008e9:	6a 7d                	push   $0x7d
801008eb:	e9 70 fb ff ff       	jmp    80100460 <vector_handler>

801008f0 <vector126>:
801008f0:	6a 00                	push   $0x0
801008f2:	6a 7e                	push   $0x7e
801008f4:	e9 67 fb ff ff       	jmp    80100460 <vector_handler>

801008f9 <vector127>:
801008f9:	6a 00                	push   $0x0
801008fb:	6a 7f                	push   $0x7f
801008fd:	e9 5e fb ff ff       	jmp    80100460 <vector_handler>

80100902 <vector128>:
80100902:	6a 00                	push   $0x0
80100904:	68 80 00 00 00       	push   $0x80
80100909:	e9 52 fb ff ff       	jmp    80100460 <vector_handler>

8010090e <vector129>:
8010090e:	6a 00                	push   $0x0
80100910:	68 81 00 00 00       	push   $0x81
80100915:	e9 46 fb ff ff       	jmp    80100460 <vector_handler>

8010091a <vector130>:
8010091a:	6a 00                	push   $0x0
8010091c:	68 82 00 00 00       	push   $0x82
80100921:	e9 3a fb ff ff       	jmp    80100460 <vector_handler>

80100926 <vector131>:
80100926:	6a 00                	push   $0x0
80100928:	68 83 00 00 00       	push   $0x83
8010092d:	e9 2e fb ff ff       	jmp    80100460 <vector_handler>

80100932 <vector132>:
80100932:	6a 00                	push   $0x0
80100934:	68 84 00 00 00       	push   $0x84
80100939:	e9 22 fb ff ff       	jmp    80100460 <vector_handler>

8010093e <vector133>:
8010093e:	6a 00                	push   $0x0
80100940:	68 85 00 00 00       	push   $0x85
80100945:	e9 16 fb ff ff       	jmp    80100460 <vector_handler>

8010094a <vector134>:
8010094a:	6a 00                	push   $0x0
8010094c:	68 86 00 00 00       	push   $0x86
80100951:	e9 0a fb ff ff       	jmp    80100460 <vector_handler>

80100956 <vector135>:
80100956:	6a 00                	push   $0x0
80100958:	68 87 00 00 00       	push   $0x87
8010095d:	e9 fe fa ff ff       	jmp    80100460 <vector_handler>

80100962 <vector136>:
80100962:	6a 00                	push   $0x0
80100964:	68 88 00 00 00       	push   $0x88
80100969:	e9 f2 fa ff ff       	jmp    80100460 <vector_handler>

8010096e <vector137>:
8010096e:	6a 00                	push   $0x0
80100970:	68 89 00 00 00       	push   $0x89
80100975:	e9 e6 fa ff ff       	jmp    80100460 <vector_handler>

8010097a <vector138>:
8010097a:	6a 00                	push   $0x0
8010097c:	68 8a 00 00 00       	push   $0x8a
80100981:	e9 da fa ff ff       	jmp    80100460 <vector_handler>

80100986 <vector139>:
80100986:	6a 00                	push   $0x0
80100988:	68 8b 00 00 00       	push   $0x8b
8010098d:	e9 ce fa ff ff       	jmp    80100460 <vector_handler>

80100992 <vector140>:
80100992:	6a 00                	push   $0x0
80100994:	68 8c 00 00 00       	push   $0x8c
80100999:	e9 c2 fa ff ff       	jmp    80100460 <vector_handler>

8010099e <vector141>:
8010099e:	6a 00                	push   $0x0
801009a0:	68 8d 00 00 00       	push   $0x8d
801009a5:	e9 b6 fa ff ff       	jmp    80100460 <vector_handler>

801009aa <vector142>:
801009aa:	6a 00                	push   $0x0
801009ac:	68 8e 00 00 00       	push   $0x8e
801009b1:	e9 aa fa ff ff       	jmp    80100460 <vector_handler>

801009b6 <vector143>:
801009b6:	6a 00                	push   $0x0
801009b8:	68 8f 00 00 00       	push   $0x8f
801009bd:	e9 9e fa ff ff       	jmp    80100460 <vector_handler>

801009c2 <vector144>:
801009c2:	6a 00                	push   $0x0
801009c4:	68 90 00 00 00       	push   $0x90
801009c9:	e9 92 fa ff ff       	jmp    80100460 <vector_handler>

801009ce <vector145>:
801009ce:	6a 00                	push   $0x0
801009d0:	68 91 00 00 00       	push   $0x91
801009d5:	e9 86 fa ff ff       	jmp    80100460 <vector_handler>

801009da <vector146>:
801009da:	6a 00                	push   $0x0
801009dc:	68 92 00 00 00       	push   $0x92
801009e1:	e9 7a fa ff ff       	jmp    80100460 <vector_handler>

801009e6 <vector147>:
801009e6:	6a 00                	push   $0x0
801009e8:	68 93 00 00 00       	push   $0x93
801009ed:	e9 6e fa ff ff       	jmp    80100460 <vector_handler>

801009f2 <vector148>:
801009f2:	6a 00                	push   $0x0
801009f4:	68 94 00 00 00       	push   $0x94
801009f9:	e9 62 fa ff ff       	jmp    80100460 <vector_handler>

801009fe <vector149>:
801009fe:	6a 00                	push   $0x0
80100a00:	68 95 00 00 00       	push   $0x95
80100a05:	e9 56 fa ff ff       	jmp    80100460 <vector_handler>

80100a0a <vector150>:
80100a0a:	6a 00                	push   $0x0
80100a0c:	68 96 00 00 00       	push   $0x96
80100a11:	e9 4a fa ff ff       	jmp    80100460 <vector_handler>

80100a16 <vector151>:
80100a16:	6a 00                	push   $0x0
80100a18:	68 97 00 00 00       	push   $0x97
80100a1d:	e9 3e fa ff ff       	jmp    80100460 <vector_handler>

80100a22 <vector152>:
80100a22:	6a 00                	push   $0x0
80100a24:	68 98 00 00 00       	push   $0x98
80100a29:	e9 32 fa ff ff       	jmp    80100460 <vector_handler>

80100a2e <vector153>:
80100a2e:	6a 00                	push   $0x0
80100a30:	68 99 00 00 00       	push   $0x99
80100a35:	e9 26 fa ff ff       	jmp    80100460 <vector_handler>

80100a3a <vector154>:
80100a3a:	6a 00                	push   $0x0
80100a3c:	68 9a 00 00 00       	push   $0x9a
80100a41:	e9 1a fa ff ff       	jmp    80100460 <vector_handler>

80100a46 <vector155>:
80100a46:	6a 00                	push   $0x0
80100a48:	68 9b 00 00 00       	push   $0x9b
80100a4d:	e9 0e fa ff ff       	jmp    80100460 <vector_handler>

80100a52 <vector156>:
80100a52:	6a 00                	push   $0x0
80100a54:	68 9c 00 00 00       	push   $0x9c
80100a59:	e9 02 fa ff ff       	jmp    80100460 <vector_handler>

80100a5e <vector157>:
80100a5e:	6a 00                	push   $0x0
80100a60:	68 9d 00 00 00       	push   $0x9d
80100a65:	e9 f6 f9 ff ff       	jmp    80100460 <vector_handler>

80100a6a <vector158>:
80100a6a:	6a 00                	push   $0x0
80100a6c:	68 9e 00 00 00       	push   $0x9e
80100a71:	e9 ea f9 ff ff       	jmp    80100460 <vector_handler>

80100a76 <vector159>:
80100a76:	6a 00                	push   $0x0
80100a78:	68 9f 00 00 00       	push   $0x9f
80100a7d:	e9 de f9 ff ff       	jmp    80100460 <vector_handler>

80100a82 <vector160>:
80100a82:	6a 00                	push   $0x0
80100a84:	68 a0 00 00 00       	push   $0xa0
80100a89:	e9 d2 f9 ff ff       	jmp    80100460 <vector_handler>

80100a8e <vector161>:
80100a8e:	6a 00                	push   $0x0
80100a90:	68 a1 00 00 00       	push   $0xa1
80100a95:	e9 c6 f9 ff ff       	jmp    80100460 <vector_handler>

80100a9a <vector162>:
80100a9a:	6a 00                	push   $0x0
80100a9c:	68 a2 00 00 00       	push   $0xa2
80100aa1:	e9 ba f9 ff ff       	jmp    80100460 <vector_handler>

80100aa6 <vector163>:
80100aa6:	6a 00                	push   $0x0
80100aa8:	68 a3 00 00 00       	push   $0xa3
80100aad:	e9 ae f9 ff ff       	jmp    80100460 <vector_handler>

80100ab2 <vector164>:
80100ab2:	6a 00                	push   $0x0
80100ab4:	68 a4 00 00 00       	push   $0xa4
80100ab9:	e9 a2 f9 ff ff       	jmp    80100460 <vector_handler>

80100abe <vector165>:
80100abe:	6a 00                	push   $0x0
80100ac0:	68 a5 00 00 00       	push   $0xa5
80100ac5:	e9 96 f9 ff ff       	jmp    80100460 <vector_handler>

80100aca <vector166>:
80100aca:	6a 00                	push   $0x0
80100acc:	68 a6 00 00 00       	push   $0xa6
80100ad1:	e9 8a f9 ff ff       	jmp    80100460 <vector_handler>

80100ad6 <vector167>:
80100ad6:	6a 00                	push   $0x0
80100ad8:	68 a7 00 00 00       	push   $0xa7
80100add:	e9 7e f9 ff ff       	jmp    80100460 <vector_handler>

80100ae2 <vector168>:
80100ae2:	6a 00                	push   $0x0
80100ae4:	68 a8 00 00 00       	push   $0xa8
80100ae9:	e9 72 f9 ff ff       	jmp    80100460 <vector_handler>

80100aee <vector169>:
80100aee:	6a 00                	push   $0x0
80100af0:	68 a9 00 00 00       	push   $0xa9
80100af5:	e9 66 f9 ff ff       	jmp    80100460 <vector_handler>

80100afa <vector170>:
80100afa:	6a 00                	push   $0x0
80100afc:	68 aa 00 00 00       	push   $0xaa
80100b01:	e9 5a f9 ff ff       	jmp    80100460 <vector_handler>

80100b06 <vector171>:
80100b06:	6a 00                	push   $0x0
80100b08:	68 ab 00 00 00       	push   $0xab
80100b0d:	e9 4e f9 ff ff       	jmp    80100460 <vector_handler>

80100b12 <vector172>:
80100b12:	6a 00                	push   $0x0
80100b14:	68 ac 00 00 00       	push   $0xac
80100b19:	e9 42 f9 ff ff       	jmp    80100460 <vector_handler>

80100b1e <vector173>:
80100b1e:	6a 00                	push   $0x0
80100b20:	68 ad 00 00 00       	push   $0xad
80100b25:	e9 36 f9 ff ff       	jmp    80100460 <vector_handler>

80100b2a <vector174>:
80100b2a:	6a 00                	push   $0x0
80100b2c:	68 ae 00 00 00       	push   $0xae
80100b31:	e9 2a f9 ff ff       	jmp    80100460 <vector_handler>

80100b36 <vector175>:
80100b36:	6a 00                	push   $0x0
80100b38:	68 af 00 00 00       	push   $0xaf
80100b3d:	e9 1e f9 ff ff       	jmp    80100460 <vector_handler>

80100b42 <vector176>:
80100b42:	6a 00                	push   $0x0
80100b44:	68 b0 00 00 00       	push   $0xb0
80100b49:	e9 12 f9 ff ff       	jmp    80100460 <vector_handler>

80100b4e <vector177>:
80100b4e:	6a 00                	push   $0x0
80100b50:	68 b1 00 00 00       	push   $0xb1
80100b55:	e9 06 f9 ff ff       	jmp    80100460 <vector_handler>

80100b5a <vector178>:
80100b5a:	6a 00                	push   $0x0
80100b5c:	68 b2 00 00 00       	push   $0xb2
80100b61:	e9 fa f8 ff ff       	jmp    80100460 <vector_handler>

80100b66 <vector179>:
80100b66:	6a 00                	push   $0x0
80100b68:	68 b3 00 00 00       	push   $0xb3
80100b6d:	e9 ee f8 ff ff       	jmp    80100460 <vector_handler>

80100b72 <vector180>:
80100b72:	6a 00                	push   $0x0
80100b74:	68 b4 00 00 00       	push   $0xb4
80100b79:	e9 e2 f8 ff ff       	jmp    80100460 <vector_handler>

80100b7e <vector181>:
80100b7e:	6a 00                	push   $0x0
80100b80:	68 b5 00 00 00       	push   $0xb5
80100b85:	e9 d6 f8 ff ff       	jmp    80100460 <vector_handler>

80100b8a <vector182>:
80100b8a:	6a 00                	push   $0x0
80100b8c:	68 b6 00 00 00       	push   $0xb6
80100b91:	e9 ca f8 ff ff       	jmp    80100460 <vector_handler>

80100b96 <vector183>:
80100b96:	6a 00                	push   $0x0
80100b98:	68 b7 00 00 00       	push   $0xb7
80100b9d:	e9 be f8 ff ff       	jmp    80100460 <vector_handler>

80100ba2 <vector184>:
80100ba2:	6a 00                	push   $0x0
80100ba4:	68 b8 00 00 00       	push   $0xb8
80100ba9:	e9 b2 f8 ff ff       	jmp    80100460 <vector_handler>

80100bae <vector185>:
80100bae:	6a 00                	push   $0x0
80100bb0:	68 b9 00 00 00       	push   $0xb9
80100bb5:	e9 a6 f8 ff ff       	jmp    80100460 <vector_handler>

80100bba <vector186>:
80100bba:	6a 00                	push   $0x0
80100bbc:	68 ba 00 00 00       	push   $0xba
80100bc1:	e9 9a f8 ff ff       	jmp    80100460 <vector_handler>

80100bc6 <vector187>:
80100bc6:	6a 00                	push   $0x0
80100bc8:	68 bb 00 00 00       	push   $0xbb
80100bcd:	e9 8e f8 ff ff       	jmp    80100460 <vector_handler>

80100bd2 <vector188>:
80100bd2:	6a 00                	push   $0x0
80100bd4:	68 bc 00 00 00       	push   $0xbc
80100bd9:	e9 82 f8 ff ff       	jmp    80100460 <vector_handler>

80100bde <vector189>:
80100bde:	6a 00                	push   $0x0
80100be0:	68 bd 00 00 00       	push   $0xbd
80100be5:	e9 76 f8 ff ff       	jmp    80100460 <vector_handler>

80100bea <vector190>:
80100bea:	6a 00                	push   $0x0
80100bec:	68 be 00 00 00       	push   $0xbe
80100bf1:	e9 6a f8 ff ff       	jmp    80100460 <vector_handler>

80100bf6 <vector191>:
80100bf6:	6a 00                	push   $0x0
80100bf8:	68 bf 00 00 00       	push   $0xbf
80100bfd:	e9 5e f8 ff ff       	jmp    80100460 <vector_handler>

80100c02 <vector192>:
80100c02:	6a 00                	push   $0x0
80100c04:	68 c0 00 00 00       	push   $0xc0
80100c09:	e9 52 f8 ff ff       	jmp    80100460 <vector_handler>

80100c0e <vector193>:
80100c0e:	6a 00                	push   $0x0
80100c10:	68 c1 00 00 00       	push   $0xc1
80100c15:	e9 46 f8 ff ff       	jmp    80100460 <vector_handler>

80100c1a <vector194>:
80100c1a:	6a 00                	push   $0x0
80100c1c:	68 c2 00 00 00       	push   $0xc2
80100c21:	e9 3a f8 ff ff       	jmp    80100460 <vector_handler>

80100c26 <vector195>:
80100c26:	6a 00                	push   $0x0
80100c28:	68 c3 00 00 00       	push   $0xc3
80100c2d:	e9 2e f8 ff ff       	jmp    80100460 <vector_handler>

80100c32 <vector196>:
80100c32:	6a 00                	push   $0x0
80100c34:	68 c4 00 00 00       	push   $0xc4
80100c39:	e9 22 f8 ff ff       	jmp    80100460 <vector_handler>

80100c3e <vector197>:
80100c3e:	6a 00                	push   $0x0
80100c40:	68 c5 00 00 00       	push   $0xc5
80100c45:	e9 16 f8 ff ff       	jmp    80100460 <vector_handler>

80100c4a <vector198>:
80100c4a:	6a 00                	push   $0x0
80100c4c:	68 c6 00 00 00       	push   $0xc6
80100c51:	e9 0a f8 ff ff       	jmp    80100460 <vector_handler>

80100c56 <vector199>:
80100c56:	6a 00                	push   $0x0
80100c58:	68 c7 00 00 00       	push   $0xc7
80100c5d:	e9 fe f7 ff ff       	jmp    80100460 <vector_handler>

80100c62 <vector200>:
80100c62:	6a 00                	push   $0x0
80100c64:	68 c8 00 00 00       	push   $0xc8
80100c69:	e9 f2 f7 ff ff       	jmp    80100460 <vector_handler>

80100c6e <vector201>:
80100c6e:	6a 00                	push   $0x0
80100c70:	68 c9 00 00 00       	push   $0xc9
80100c75:	e9 e6 f7 ff ff       	jmp    80100460 <vector_handler>

80100c7a <vector202>:
80100c7a:	6a 00                	push   $0x0
80100c7c:	68 ca 00 00 00       	push   $0xca
80100c81:	e9 da f7 ff ff       	jmp    80100460 <vector_handler>

80100c86 <vector203>:
80100c86:	6a 00                	push   $0x0
80100c88:	68 cb 00 00 00       	push   $0xcb
80100c8d:	e9 ce f7 ff ff       	jmp    80100460 <vector_handler>

80100c92 <vector204>:
80100c92:	6a 00                	push   $0x0
80100c94:	68 cc 00 00 00       	push   $0xcc
80100c99:	e9 c2 f7 ff ff       	jmp    80100460 <vector_handler>

80100c9e <vector205>:
80100c9e:	6a 00                	push   $0x0
80100ca0:	68 cd 00 00 00       	push   $0xcd
80100ca5:	e9 b6 f7 ff ff       	jmp    80100460 <vector_handler>

80100caa <vector206>:
80100caa:	6a 00                	push   $0x0
80100cac:	68 ce 00 00 00       	push   $0xce
80100cb1:	e9 aa f7 ff ff       	jmp    80100460 <vector_handler>

80100cb6 <vector207>:
80100cb6:	6a 00                	push   $0x0
80100cb8:	68 cf 00 00 00       	push   $0xcf
80100cbd:	e9 9e f7 ff ff       	jmp    80100460 <vector_handler>

80100cc2 <vector208>:
80100cc2:	6a 00                	push   $0x0
80100cc4:	68 d0 00 00 00       	push   $0xd0
80100cc9:	e9 92 f7 ff ff       	jmp    80100460 <vector_handler>

80100cce <vector209>:
80100cce:	6a 00                	push   $0x0
80100cd0:	68 d1 00 00 00       	push   $0xd1
80100cd5:	e9 86 f7 ff ff       	jmp    80100460 <vector_handler>

80100cda <vector210>:
80100cda:	6a 00                	push   $0x0
80100cdc:	68 d2 00 00 00       	push   $0xd2
80100ce1:	e9 7a f7 ff ff       	jmp    80100460 <vector_handler>

80100ce6 <vector211>:
80100ce6:	6a 00                	push   $0x0
80100ce8:	68 d3 00 00 00       	push   $0xd3
80100ced:	e9 6e f7 ff ff       	jmp    80100460 <vector_handler>

80100cf2 <vector212>:
80100cf2:	6a 00                	push   $0x0
80100cf4:	68 d4 00 00 00       	push   $0xd4
80100cf9:	e9 62 f7 ff ff       	jmp    80100460 <vector_handler>

80100cfe <vector213>:
80100cfe:	6a 00                	push   $0x0
80100d00:	68 d5 00 00 00       	push   $0xd5
80100d05:	e9 56 f7 ff ff       	jmp    80100460 <vector_handler>

80100d0a <vector214>:
80100d0a:	6a 00                	push   $0x0
80100d0c:	68 d6 00 00 00       	push   $0xd6
80100d11:	e9 4a f7 ff ff       	jmp    80100460 <vector_handler>

80100d16 <vector215>:
80100d16:	6a 00                	push   $0x0
80100d18:	68 d7 00 00 00       	push   $0xd7
80100d1d:	e9 3e f7 ff ff       	jmp    80100460 <vector_handler>

80100d22 <vector216>:
80100d22:	6a 00                	push   $0x0
80100d24:	68 d8 00 00 00       	push   $0xd8
80100d29:	e9 32 f7 ff ff       	jmp    80100460 <vector_handler>

80100d2e <vector217>:
80100d2e:	6a 00                	push   $0x0
80100d30:	68 d9 00 00 00       	push   $0xd9
80100d35:	e9 26 f7 ff ff       	jmp    80100460 <vector_handler>

80100d3a <vector218>:
80100d3a:	6a 00                	push   $0x0
80100d3c:	68 da 00 00 00       	push   $0xda
80100d41:	e9 1a f7 ff ff       	jmp    80100460 <vector_handler>

80100d46 <vector219>:
80100d46:	6a 00                	push   $0x0
80100d48:	68 db 00 00 00       	push   $0xdb
80100d4d:	e9 0e f7 ff ff       	jmp    80100460 <vector_handler>

80100d52 <vector220>:
80100d52:	6a 00                	push   $0x0
80100d54:	68 dc 00 00 00       	push   $0xdc
80100d59:	e9 02 f7 ff ff       	jmp    80100460 <vector_handler>

80100d5e <vector221>:
80100d5e:	6a 00                	push   $0x0
80100d60:	68 dd 00 00 00       	push   $0xdd
80100d65:	e9 f6 f6 ff ff       	jmp    80100460 <vector_handler>

80100d6a <vector222>:
80100d6a:	6a 00                	push   $0x0
80100d6c:	68 de 00 00 00       	push   $0xde
80100d71:	e9 ea f6 ff ff       	jmp    80100460 <vector_handler>

80100d76 <vector223>:
80100d76:	6a 00                	push   $0x0
80100d78:	68 df 00 00 00       	push   $0xdf
80100d7d:	e9 de f6 ff ff       	jmp    80100460 <vector_handler>

80100d82 <vector224>:
80100d82:	6a 00                	push   $0x0
80100d84:	68 e0 00 00 00       	push   $0xe0
80100d89:	e9 d2 f6 ff ff       	jmp    80100460 <vector_handler>

80100d8e <vector225>:
80100d8e:	6a 00                	push   $0x0
80100d90:	68 e1 00 00 00       	push   $0xe1
80100d95:	e9 c6 f6 ff ff       	jmp    80100460 <vector_handler>

80100d9a <vector226>:
80100d9a:	6a 00                	push   $0x0
80100d9c:	68 e2 00 00 00       	push   $0xe2
80100da1:	e9 ba f6 ff ff       	jmp    80100460 <vector_handler>

80100da6 <vector227>:
80100da6:	6a 00                	push   $0x0
80100da8:	68 e3 00 00 00       	push   $0xe3
80100dad:	e9 ae f6 ff ff       	jmp    80100460 <vector_handler>

80100db2 <vector228>:
80100db2:	6a 00                	push   $0x0
80100db4:	68 e4 00 00 00       	push   $0xe4
80100db9:	e9 a2 f6 ff ff       	jmp    80100460 <vector_handler>

80100dbe <vector229>:
80100dbe:	6a 00                	push   $0x0
80100dc0:	68 e5 00 00 00       	push   $0xe5
80100dc5:	e9 96 f6 ff ff       	jmp    80100460 <vector_handler>

80100dca <vector230>:
80100dca:	6a 00                	push   $0x0
80100dcc:	68 e6 00 00 00       	push   $0xe6
80100dd1:	e9 8a f6 ff ff       	jmp    80100460 <vector_handler>

80100dd6 <vector231>:
80100dd6:	6a 00                	push   $0x0
80100dd8:	68 e7 00 00 00       	push   $0xe7
80100ddd:	e9 7e f6 ff ff       	jmp    80100460 <vector_handler>

80100de2 <vector232>:
80100de2:	6a 00                	push   $0x0
80100de4:	68 e8 00 00 00       	push   $0xe8
80100de9:	e9 72 f6 ff ff       	jmp    80100460 <vector_handler>

80100dee <vector233>:
80100dee:	6a 00                	push   $0x0
80100df0:	68 e9 00 00 00       	push   $0xe9
80100df5:	e9 66 f6 ff ff       	jmp    80100460 <vector_handler>

80100dfa <vector234>:
80100dfa:	6a 00                	push   $0x0
80100dfc:	68 ea 00 00 00       	push   $0xea
80100e01:	e9 5a f6 ff ff       	jmp    80100460 <vector_handler>

80100e06 <vector235>:
80100e06:	6a 00                	push   $0x0
80100e08:	68 eb 00 00 00       	push   $0xeb
80100e0d:	e9 4e f6 ff ff       	jmp    80100460 <vector_handler>

80100e12 <vector236>:
80100e12:	6a 00                	push   $0x0
80100e14:	68 ec 00 00 00       	push   $0xec
80100e19:	e9 42 f6 ff ff       	jmp    80100460 <vector_handler>

80100e1e <vector237>:
80100e1e:	6a 00                	push   $0x0
80100e20:	68 ed 00 00 00       	push   $0xed
80100e25:	e9 36 f6 ff ff       	jmp    80100460 <vector_handler>

80100e2a <vector238>:
80100e2a:	6a 00                	push   $0x0
80100e2c:	68 ee 00 00 00       	push   $0xee
80100e31:	e9 2a f6 ff ff       	jmp    80100460 <vector_handler>

80100e36 <vector239>:
80100e36:	6a 00                	push   $0x0
80100e38:	68 ef 00 00 00       	push   $0xef
80100e3d:	e9 1e f6 ff ff       	jmp    80100460 <vector_handler>

80100e42 <vector240>:
80100e42:	6a 00                	push   $0x0
80100e44:	68 f0 00 00 00       	push   $0xf0
80100e49:	e9 12 f6 ff ff       	jmp    80100460 <vector_handler>

80100e4e <vector241>:
80100e4e:	6a 00                	push   $0x0
80100e50:	68 f1 00 00 00       	push   $0xf1
80100e55:	e9 06 f6 ff ff       	jmp    80100460 <vector_handler>

80100e5a <vector242>:
80100e5a:	6a 00                	push   $0x0
80100e5c:	68 f2 00 00 00       	push   $0xf2
80100e61:	e9 fa f5 ff ff       	jmp    80100460 <vector_handler>

80100e66 <vector243>:
80100e66:	6a 00                	push   $0x0
80100e68:	68 f3 00 00 00       	push   $0xf3
80100e6d:	e9 ee f5 ff ff       	jmp    80100460 <vector_handler>

80100e72 <vector244>:
80100e72:	6a 00                	push   $0x0
80100e74:	68 f4 00 00 00       	push   $0xf4
80100e79:	e9 e2 f5 ff ff       	jmp    80100460 <vector_handler>

80100e7e <vector245>:
80100e7e:	6a 00                	push   $0x0
80100e80:	68 f5 00 00 00       	push   $0xf5
80100e85:	e9 d6 f5 ff ff       	jmp    80100460 <vector_handler>

80100e8a <vector246>:
80100e8a:	6a 00                	push   $0x0
80100e8c:	68 f6 00 00 00       	push   $0xf6
80100e91:	e9 ca f5 ff ff       	jmp    80100460 <vector_handler>

80100e96 <vector247>:
80100e96:	6a 00                	push   $0x0
80100e98:	68 f7 00 00 00       	push   $0xf7
80100e9d:	e9 be f5 ff ff       	jmp    80100460 <vector_handler>

80100ea2 <vector248>:
80100ea2:	6a 00                	push   $0x0
80100ea4:	68 f8 00 00 00       	push   $0xf8
80100ea9:	e9 b2 f5 ff ff       	jmp    80100460 <vector_handler>

80100eae <vector249>:
80100eae:	6a 00                	push   $0x0
80100eb0:	68 f9 00 00 00       	push   $0xf9
80100eb5:	e9 a6 f5 ff ff       	jmp    80100460 <vector_handler>

80100eba <vector250>:
80100eba:	6a 00                	push   $0x0
80100ebc:	68 fa 00 00 00       	push   $0xfa
80100ec1:	e9 9a f5 ff ff       	jmp    80100460 <vector_handler>

80100ec6 <vector251>:
80100ec6:	6a 00                	push   $0x0
80100ec8:	68 fb 00 00 00       	push   $0xfb
80100ecd:	e9 8e f5 ff ff       	jmp    80100460 <vector_handler>

80100ed2 <vector252>:
80100ed2:	6a 00                	push   $0x0
80100ed4:	68 fc 00 00 00       	push   $0xfc
80100ed9:	e9 82 f5 ff ff       	jmp    80100460 <vector_handler>

80100ede <vector253>:
80100ede:	6a 00                	push   $0x0
80100ee0:	68 fd 00 00 00       	push   $0xfd
80100ee5:	e9 76 f5 ff ff       	jmp    80100460 <vector_handler>

80100eea <vector254>:
80100eea:	6a 00                	push   $0x0
80100eec:	68 fe 00 00 00       	push   $0xfe
80100ef1:	e9 6a f5 ff ff       	jmp    80100460 <vector_handler>

80100ef6 <vector255>:
80100ef6:	6a 00                	push   $0x0
80100ef8:	68 ff 00 00 00       	push   $0xff
80100efd:	e9 5e f5 ff ff       	jmp    80100460 <vector_handler>

80100f02 <itoa>:
80100f02:	57                   	push   %edi
80100f03:	56                   	push   %esi
80100f04:	53                   	push   %ebx
80100f05:	8b 54 24 10          	mov    0x10(%esp),%edx
80100f09:	8b 74 24 14          	mov    0x14(%esp),%esi
80100f0d:	8b 44 24 18          	mov    0x18(%esp),%eax
80100f11:	89 c7                	mov    %eax,%edi
80100f13:	83 e8 02             	sub    $0x2,%eax
80100f16:	83 f8 22             	cmp    $0x22,%eax
80100f19:	77 04                	ja     80100f1f <itoa+0x1d>
80100f1b:	89 f1                	mov    %esi,%ecx
80100f1d:	eb 0b                	jmp    80100f2a <itoa+0x28>
80100f1f:	c6 06 00             	movb   $0x0,(%esi)
80100f22:	89 f0                	mov    %esi,%eax
80100f24:	5b                   	pop    %ebx
80100f25:	5e                   	pop    %esi
80100f26:	5f                   	pop    %edi
80100f27:	c3                   	ret    
80100f28:	89 d9                	mov    %ebx,%ecx
80100f2a:	8d 59 01             	lea    0x1(%ecx),%ebx
80100f2d:	89 d0                	mov    %edx,%eax
80100f2f:	ba 00 00 00 00       	mov    $0x0,%edx
80100f34:	f7 f7                	div    %edi
80100f36:	8a 92 53 11 10 80    	mov    -0x7fefeead(%edx),%dl
80100f3c:	88 53 ff             	mov    %dl,-0x1(%ebx)
80100f3f:	89 c2                	mov    %eax,%edx
80100f41:	85 c0                	test   %eax,%eax
80100f43:	75 e3                	jne    80100f28 <itoa+0x26>
80100f45:	c6 41 01 00          	movb   $0x0,0x1(%ecx)
80100f49:	39 f1                	cmp    %esi,%ecx
80100f4b:	76 d5                	jbe    80100f22 <itoa+0x20>
80100f4d:	89 f0                	mov    %esi,%eax
80100f4f:	8a 10                	mov    (%eax),%dl
80100f51:	40                   	inc    %eax
80100f52:	8a 19                	mov    (%ecx),%bl
80100f54:	88 58 ff             	mov    %bl,-0x1(%eax)
80100f57:	49                   	dec    %ecx
80100f58:	88 51 01             	mov    %dl,0x1(%ecx)
80100f5b:	39 c8                	cmp    %ecx,%eax
80100f5d:	72 f0                	jb     80100f4f <itoa+0x4d>
80100f5f:	eb c1                	jmp    80100f22 <itoa+0x20>

80100f61 <write_string>:
80100f61:	55                   	push   %ebp
80100f62:	57                   	push   %edi
80100f63:	56                   	push   %esi
80100f64:	53                   	push   %ebx
80100f65:	8b 5c 24 14          	mov    0x14(%esp),%ebx
80100f69:	8b 4c 24 18          	mov    0x18(%esp),%ecx
80100f6d:	8a 01                	mov    (%ecx),%al
80100f6f:	41                   	inc    %ecx
80100f70:	be 67 66 66 66       	mov    $0x66666667,%esi
80100f75:	eb 2d                	jmp    80100fa4 <write_string+0x43>
80100f77:	a1 00 34 10 80       	mov    0x80103400,%eax
80100f7c:	8d a8 00 80 f4 ff    	lea    -0xb8000(%eax),%ebp
80100f82:	89 e8                	mov    %ebp,%eax
80100f84:	f7 ee                	imul   %esi
80100f86:	89 d0                	mov    %edx,%eax
80100f88:	c1 f8 06             	sar    $0x6,%eax
80100f8b:	c1 fd 1f             	sar    $0x1f,%ebp
80100f8e:	29 e8                	sub    %ebp,%eax
80100f90:	8d 44 80 05          	lea    0x5(%eax,%eax,4),%eax
80100f94:	c1 e0 05             	shl    $0x5,%eax
80100f97:	05 00 80 0b 00       	add    $0xb8000,%eax
80100f9c:	a3 00 34 10 80       	mov    %eax,0x80103400
80100fa1:	8a 07                	mov    (%edi),%al
80100fa3:	41                   	inc    %ecx
80100fa4:	89 cf                	mov    %ecx,%edi
80100fa6:	80 79 ff 00          	cmpb   $0x0,-0x1(%ecx)
80100faa:	74 2a                	je     80100fd6 <write_string+0x75>
80100fac:	3c 0a                	cmp    $0xa,%al
80100fae:	74 c7                	je     80100f77 <write_string+0x16>
80100fb0:	8b 15 00 34 10 80    	mov    0x80103400,%edx
80100fb6:	8d 6a 01             	lea    0x1(%edx),%ebp
80100fb9:	89 2d 00 34 10 80    	mov    %ebp,0x80103400
80100fbf:	88 02                	mov    %al,(%edx)
80100fc1:	8a 07                	mov    (%edi),%al
80100fc3:	8b 15 00 34 10 80    	mov    0x80103400,%edx
80100fc9:	8d 7a 01             	lea    0x1(%edx),%edi
80100fcc:	89 3d 00 34 10 80    	mov    %edi,0x80103400
80100fd2:	88 1a                	mov    %bl,(%edx)
80100fd4:	eb cd                	jmp    80100fa3 <write_string+0x42>
80100fd6:	5b                   	pop    %ebx
80100fd7:	5e                   	pop    %esi
80100fd8:	5f                   	pop    %edi
80100fd9:	5d                   	pop    %ebp
80100fda:	c3                   	ret    

80100fdb <clr_scr>:
80100fdb:	c7 05 00 34 10 80 00 	movl   $0xb8000,0x80103400
80100fe2:	80 0b 00 
80100fe5:	b8 00 80 0b 00       	mov    $0xb8000,%eax
80100fea:	eb 02                	jmp    80100fee <clr_scr+0x13>
80100fec:	89 d0                	mov    %edx,%eax
80100fee:	c6 00 00             	movb   $0x0,(%eax)
80100ff1:	8d 50 02             	lea    0x2(%eax),%edx
80100ff4:	c6 40 01 00          	movb   $0x0,0x1(%eax)
80100ff8:	3d 9e 8f 0b 00       	cmp    $0xb8f9e,%eax
80100ffd:	75 ed                	jne    80100fec <clr_scr+0x11>
80100fff:	c3                   	ret    

80101000 <memcpy>:
80101000:	56                   	push   %esi
80101001:	53                   	push   %ebx
80101002:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101006:	8b 5c 24 10          	mov    0x10(%esp),%ebx
8010100a:	8b 4c 24 14          	mov    0x14(%esp),%ecx
8010100e:	39 c3                	cmp    %eax,%ebx
80101010:	72 19                	jb     8010102b <memcpy+0x2b>
80101012:	85 c9                	test   %ecx,%ecx
80101014:	7e 12                	jle    80101028 <memcpy+0x28>
80101016:	89 ce                	mov    %ecx,%esi
80101018:	ba 00 00 00 00       	mov    $0x0,%edx
8010101d:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
80101020:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101023:	42                   	inc    %edx
80101024:	39 d6                	cmp    %edx,%esi
80101026:	75 f5                	jne    8010101d <memcpy+0x1d>
80101028:	5b                   	pop    %ebx
80101029:	5e                   	pop    %esi
8010102a:	c3                   	ret    
8010102b:	8d 14 0b             	lea    (%ebx,%ecx,1),%edx
8010102e:	39 d0                	cmp    %edx,%eax
80101030:	73 e0                	jae    80101012 <memcpy+0x12>
80101032:	8d 51 ff             	lea    -0x1(%ecx),%edx
80101035:	85 c9                	test   %ecx,%ecx
80101037:	7e ef                	jle    80101028 <memcpy+0x28>
80101039:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
8010103c:	88 0c 10             	mov    %cl,(%eax,%edx,1)
8010103f:	4a                   	dec    %edx
80101040:	83 fa ff             	cmp    $0xffffffff,%edx
80101043:	75 f4                	jne    80101039 <memcpy+0x39>
80101045:	eb e1                	jmp    80101028 <memcpy+0x28>

80101047 <memset>:
80101047:	57                   	push   %edi
80101048:	8b 54 24 08          	mov    0x8(%esp),%edx
8010104c:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101050:	8b 4c 24 10          	mov    0x10(%esp),%ecx
80101054:	89 d7                	mov    %edx,%edi
80101056:	09 cf                	or     %ecx,%edi
80101058:	f7 c7 03 00 00 00    	test   $0x3,%edi
8010105e:	74 09                	je     80101069 <memset+0x22>
80101060:	89 d7                	mov    %edx,%edi
80101062:	fc                   	cld    
80101063:	f3 aa                	rep stos %al,%es:(%edi)
80101065:	89 d0                	mov    %edx,%eax
80101067:	5f                   	pop    %edi
80101068:	c3                   	ret    
80101069:	89 cf                	mov    %ecx,%edi
8010106b:	85 c9                	test   %ecx,%ecx
8010106d:	78 0c                	js     8010107b <memset+0x34>
8010106f:	89 f9                	mov    %edi,%ecx
80101071:	c1 f9 02             	sar    $0x2,%ecx
80101074:	89 d7                	mov    %edx,%edi
80101076:	fc                   	cld    
80101077:	f3 ab                	rep stos %eax,%es:(%edi)
80101079:	eb ea                	jmp    80101065 <memset+0x1e>
8010107b:	8d 79 03             	lea    0x3(%ecx),%edi
8010107e:	eb ef                	jmp    8010106f <memset+0x28>

80101080 <strlen>:
80101080:	8b 54 24 04          	mov    0x4(%esp),%edx
80101084:	80 3a 00             	cmpb   $0x0,(%edx)
80101087:	74 0d                	je     80101096 <strlen+0x16>
80101089:	b8 00 00 00 00       	mov    $0x0,%eax
8010108e:	40                   	inc    %eax
8010108f:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
80101093:	75 f9                	jne    8010108e <strlen+0xe>
80101095:	c3                   	ret    
80101096:	b8 00 00 00 00       	mov    $0x0,%eax
8010109b:	c3                   	ret    
