
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
8010001c:	bc b0 53 10 80       	mov    $0x801053b0,%esp
80100021:	b8 be 00 10 80       	mov    $0x801000be,%eax
80100026:	ff e0                	jmp    *%eax

80100028 <interrupt_handler>:
80100028:	eb fe                	jmp    80100028 <interrupt_handler>

8010002a <init_8259A>:
8010002a:	53                   	push   %ebx
8010002b:	b0 11                	mov    $0x11,%al
8010002d:	ba 20 00 00 00       	mov    $0x20,%edx
80100032:	ee                   	out    %al,(%dx)
80100033:	ba a0 00 00 00       	mov    $0xa0,%edx
80100038:	ee                   	out    %al,(%dx)
80100039:	bb 21 00 00 00       	mov    $0x21,%ebx
8010003e:	b0 20                	mov    $0x20,%al
80100040:	89 da                	mov    %ebx,%edx
80100042:	ee                   	out    %al,(%dx)
80100043:	b9 a1 00 00 00       	mov    $0xa1,%ecx
80100048:	b0 28                	mov    $0x28,%al
8010004a:	89 ca                	mov    %ecx,%edx
8010004c:	ee                   	out    %al,(%dx)
8010004d:	b0 04                	mov    $0x4,%al
8010004f:	89 da                	mov    %ebx,%edx
80100051:	ee                   	out    %al,(%dx)
80100052:	b0 02                	mov    $0x2,%al
80100054:	89 ca                	mov    %ecx,%edx
80100056:	ee                   	out    %al,(%dx)
80100057:	b0 01                	mov    $0x1,%al
80100059:	89 da                	mov    %ebx,%edx
8010005b:	ee                   	out    %al,(%dx)
8010005c:	89 ca                	mov    %ecx,%edx
8010005e:	ee                   	out    %al,(%dx)
8010005f:	b0 ff                	mov    $0xff,%al
80100061:	89 da                	mov    %ebx,%edx
80100063:	ee                   	out    %al,(%dx)
80100064:	89 ca                	mov    %ecx,%edx
80100066:	ee                   	out    %al,(%dx)
80100067:	5b                   	pop    %ebx
80100068:	c3                   	ret    

80100069 <make_desc>:
80100069:	57                   	push   %edi
8010006a:	56                   	push   %esi
8010006b:	53                   	push   %ebx
8010006c:	8b 5c 24 14          	mov    0x14(%esp),%ebx
80100070:	8b 7c 24 18          	mov    0x18(%esp),%edi
80100074:	8b 54 24 1c          	mov    0x1c(%esp),%edx
80100078:	8b 74 24 10          	mov    0x10(%esp),%esi
8010007c:	c1 e6 06             	shl    $0x6,%esi
8010007f:	8d 8e 00 00 20 80    	lea    -0x7fe00000(%esi),%ecx
80100085:	66 89 be 00 00 20 80 	mov    %di,-0x7fe00000(%esi)
8010008c:	66 89 9e 02 00 20 80 	mov    %bx,-0x7fdffffe(%esi)
80100093:	89 de                	mov    %ebx,%esi
80100095:	c1 ee 10             	shr    $0x10,%esi
80100098:	89 f0                	mov    %esi,%eax
8010009a:	88 41 04             	mov    %al,0x4(%ecx)
8010009d:	c1 eb 18             	shr    $0x18,%ebx
801000a0:	88 59 07             	mov    %bl,0x7(%ecx)
801000a3:	88 51 05             	mov    %dl,0x5(%ecx)
801000a6:	89 f8                	mov    %edi,%eax
801000a8:	c1 e8 10             	shr    $0x10,%eax
801000ab:	83 e0 0f             	and    $0xf,%eax
801000ae:	66 c1 ea 08          	shr    $0x8,%dx
801000b2:	83 e2 f0             	and    $0xfffffff0,%edx
801000b5:	09 d0                	or     %edx,%eax
801000b7:	88 41 06             	mov    %al,0x6(%ecx)
801000ba:	5b                   	pop    %ebx
801000bb:	5e                   	pop    %esi
801000bc:	5f                   	pop    %edi
801000bd:	c3                   	ret    

801000be <main>:
801000be:	8d 4c 24 04          	lea    0x4(%esp),%ecx
801000c2:	83 e4 f0             	and    $0xfffffff0,%esp
801000c5:	ff 71 fc             	pushl  -0x4(%ecx)
801000c8:	55                   	push   %ebp
801000c9:	89 e5                	mov    %esp,%ebp
801000cb:	56                   	push   %esi
801000cc:	53                   	push   %ebx
801000cd:	51                   	push   %ecx
801000ce:	83 ec 1c             	sub    $0x1c,%esp
801000d1:	e8 d8 0f 00 00       	call   801010ae <clr_scr>
801000d6:	6a 00                	push   $0x0
801000d8:	6a 00                	push   $0x0
801000da:	6a 00                	push   $0x0
801000dc:	6a 00                	push   $0x0
801000de:	e8 86 ff ff ff       	call   80100069 <make_desc>
801000e3:	68 98 c0 00 00       	push   $0xc098
801000e8:	68 00 00 f0 80       	push   $0x80f00000
801000ed:	6a 00                	push   $0x0
801000ef:	6a 01                	push   $0x1
801000f1:	e8 73 ff ff ff       	call   80100069 <make_desc>
801000f6:	83 c4 20             	add    $0x20,%esp
801000f9:	68 92 c0 00 00       	push   $0xc092
801000fe:	68 00 00 f0 80       	push   $0x80f00000
80100103:	6a 00                	push   $0x0
80100105:	6a 02                	push   $0x2
80100107:	e8 5d ff ff ff       	call   80100069 <make_desc>
8010010c:	68 f2 00 00 00       	push   $0xf2
80100111:	68 ff ff 00 00       	push   $0xffff
80100116:	68 00 80 0b 00       	push   $0xb8000
8010011b:	6a 03                	push   $0x3
8010011d:	e8 47 ff ff ff       	call   80100069 <make_desc>
80100122:	66 c7 05 b8 53 10 80 	movw   $0x1f,0x801053b8
80100129:	1f 00 
8010012b:	c7 05 ba 53 10 80 00 	movl   $0x80200000,0x801053ba
80100132:	00 20 80 
80100135:	0f 01 15 b8 53 10 80 	lgdtl  0x801053b8
8010013c:	b9 00 30 10 80       	mov    $0x80103000,%ecx
80100141:	83 c4 20             	add    $0x20,%esp
80100144:	b8 00 04 20 80       	mov    $0x80200400,%eax
80100149:	8b 11                	mov    (%ecx),%edx
8010014b:	66 89 10             	mov    %dx,(%eax)
8010014e:	c1 ea 10             	shr    $0x10,%edx
80100151:	66 89 50 06          	mov    %dx,0x6(%eax)
80100155:	66 c7 40 02 08 00    	movw   $0x8,0x2(%eax)
8010015b:	c6 40 04 00          	movb   $0x0,0x4(%eax)
8010015f:	c6 40 05 ee          	movb   $0xee,0x5(%eax)
80100163:	83 c1 04             	add    $0x4,%ecx
80100166:	83 c0 08             	add    $0x8,%eax
80100169:	3d 00 0c 20 80       	cmp    $0x80200c00,%eax
8010016e:	75 d9                	jne    80100149 <main+0x8b>
80100170:	66 c7 05 b0 53 10 80 	movw   $0x1f,0x801053b0
80100177:	1f 00 
80100179:	c7 05 b2 53 10 80 00 	movl   $0x80200400,0x801053b2
80100180:	04 20 80 
80100183:	0f 01 1d b0 53 10 80 	lidtl  0x801053b0
8010018a:	e8 9b fe ff ff       	call   8010002a <init_8259A>
8010018f:	83 ec 08             	sub    $0x8,%esp
80100192:	68 70 11 10 80       	push   $0x80101170
80100197:	6a 0f                	push   $0xf
80100199:	e8 96 0e 00 00       	call   80101034 <write_string>
8010019e:	83 c4 08             	add    $0x8,%esp
801001a1:	68 7e 11 10 80       	push   $0x8010117e
801001a6:	6a 0f                	push   $0xf
801001a8:	e8 87 0e 00 00       	call   80101034 <write_string>
801001ad:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
801001b3:	8d 04 12             	lea    (%edx,%edx,1),%eax
801001b6:	01 d0                	add    %edx,%eax
801001b8:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
801001bf:	83 c4 10             	add    $0x10,%esp
801001c2:	3d 00 80 00 00       	cmp    $0x8000,%eax
801001c7:	0f 86 18 01 00 00    	jbe    801002e5 <main+0x227>
801001cd:	bb 00 80 00 00       	mov    $0x8000,%ebx
801001d2:	8d 75 d8             	lea    -0x28(%ebp),%esi
801001d5:	eb 2f                	jmp    80100206 <main+0x148>
801001d7:	83 ec 08             	sub    $0x8,%esp
801001da:	68 b9 11 10 80       	push   $0x801011b9
801001df:	6a 0f                	push   $0xf
801001e1:	e8 4e 0e 00 00       	call   80101034 <write_string>
801001e6:	83 c4 10             	add    $0x10,%esp
801001e9:	83 c3 18             	add    $0x18,%ebx
801001ec:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
801001f2:	8d 04 12             	lea    (%edx,%edx,1),%eax
801001f5:	01 d0                	add    %edx,%eax
801001f7:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
801001fe:	39 c3                	cmp    %eax,%ebx
80100200:	0f 83 df 00 00 00    	jae    801002e5 <main+0x227>
80100206:	83 ec 04             	sub    $0x4,%esp
80100209:	6a 10                	push   $0x10
8010020b:	56                   	push   %esi
8010020c:	8b 43 04             	mov    0x4(%ebx),%eax
8010020f:	c1 e0 10             	shl    $0x10,%eax
80100212:	0b 03                	or     (%ebx),%eax
80100214:	50                   	push   %eax
80100215:	e8 a8 0d 00 00       	call   80100fc2 <itoa>
8010021a:	83 c4 08             	add    $0x8,%esp
8010021d:	68 99 11 10 80       	push   $0x80101199
80100222:	6a 0f                	push   $0xf
80100224:	e8 0b 0e 00 00       	call   80101034 <write_string>
80100229:	83 c4 08             	add    $0x8,%esp
8010022c:	56                   	push   %esi
8010022d:	6a 0f                	push   $0xf
8010022f:	e8 00 0e 00 00       	call   80101034 <write_string>
80100234:	83 c4 08             	add    $0x8,%esp
80100237:	68 a5 11 10 80       	push   $0x801011a5
8010023c:	6a 0f                	push   $0xf
8010023e:	e8 f1 0d 00 00       	call   80101034 <write_string>
80100243:	83 c4 0c             	add    $0xc,%esp
80100246:	6a 10                	push   $0x10
80100248:	56                   	push   %esi
80100249:	8b 43 0c             	mov    0xc(%ebx),%eax
8010024c:	c1 e0 10             	shl    $0x10,%eax
8010024f:	0b 43 08             	or     0x8(%ebx),%eax
80100252:	50                   	push   %eax
80100253:	e8 6a 0d 00 00       	call   80100fc2 <itoa>
80100258:	83 c4 08             	add    $0x8,%esp
8010025b:	56                   	push   %esi
8010025c:	6a 0f                	push   $0xf
8010025e:	e8 d1 0d 00 00       	call   80101034 <write_string>
80100263:	83 c4 08             	add    $0x8,%esp
80100266:	68 b0 11 10 80       	push   $0x801011b0
8010026b:	6a 0f                	push   $0xf
8010026d:	e8 c2 0d 00 00       	call   80101034 <write_string>
80100272:	83 c4 10             	add    $0x10,%esp
80100275:	83 7b 10 05          	cmpl   $0x5,0x10(%ebx)
80100279:	0f 87 6a ff ff ff    	ja     801001e9 <main+0x12b>
8010027f:	8b 43 10             	mov    0x10(%ebx),%eax
80100282:	ff 24 85 e8 11 10 80 	jmp    *-0x7fefee18(,%eax,4)
80100289:	83 ec 08             	sub    $0x8,%esp
8010028c:	68 c1 11 10 80       	push   $0x801011c1
80100291:	6a 0f                	push   $0xf
80100293:	e8 9c 0d 00 00       	call   80101034 <write_string>
80100298:	83 c4 10             	add    $0x10,%esp
8010029b:	e9 49 ff ff ff       	jmp    801001e9 <main+0x12b>
801002a0:	83 ec 08             	sub    $0x8,%esp
801002a3:	68 cb 11 10 80       	push   $0x801011cb
801002a8:	6a 0f                	push   $0xf
801002aa:	e8 85 0d 00 00       	call   80101034 <write_string>
801002af:	83 c4 10             	add    $0x10,%esp
801002b2:	e9 32 ff ff ff       	jmp    801001e9 <main+0x12b>
801002b7:	83 ec 08             	sub    $0x8,%esp
801002ba:	68 d8 11 10 80       	push   $0x801011d8
801002bf:	6a 0f                	push   $0xf
801002c1:	e8 6e 0d 00 00       	call   80101034 <write_string>
801002c6:	83 c4 10             	add    $0x10,%esp
801002c9:	e9 1b ff ff ff       	jmp    801001e9 <main+0x12b>
801002ce:	83 ec 08             	sub    $0x8,%esp
801002d1:	68 e2 11 10 80       	push   $0x801011e2
801002d6:	6a 0f                	push   $0xf
801002d8:	e8 57 0d 00 00       	call   80101034 <write_string>
801002dd:	83 c4 10             	add    $0x10,%esp
801002e0:	e9 04 ff ff ff       	jmp    801001e9 <main+0x12b>
801002e5:	eb fe                	jmp    801002e5 <main+0x227>

801002e7 <found_tbl_by_va>:
801002e7:	8b 44 24 04          	mov    0x4(%esp),%eax
801002eb:	c1 e8 16             	shr    $0x16,%eax
801002ee:	c1 e0 02             	shl    $0x2,%eax
801002f1:	f6 80 00 0c 20 80 01 	testb  $0x1,-0x7fdff400(%eax)
801002f8:	75 0a                	jne    80100304 <found_tbl_by_va+0x1d>
801002fa:	c7 80 00 0c 20 80 00 	movl   $0x80201c00,-0x7fdff400(%eax)
80100301:	1c 20 80 
80100304:	8b 80 00 0c 20 80    	mov    -0x7fdff400(%eax),%eax
8010030a:	c3                   	ret    

8010030b <vm_map>:
8010030b:	55                   	push   %ebp
8010030c:	57                   	push   %edi
8010030d:	56                   	push   %esi
8010030e:	53                   	push   %ebx
8010030f:	83 ec 1c             	sub    $0x1c,%esp
80100312:	8b 44 24 30          	mov    0x30(%esp),%eax
80100316:	89 c5                	mov    %eax,%ebp
80100318:	81 e5 00 f0 ff ff    	and    $0xfffff000,%ebp
8010031e:	8b 54 24 34          	mov    0x34(%esp),%edx
80100322:	8d 74 10 ff          	lea    -0x1(%eax,%edx,1),%esi
80100326:	81 e6 00 f0 ff ff    	and    $0xfffff000,%esi
8010032c:	55                   	push   %ebp
8010032d:	e8 b5 ff ff ff       	call   801002e7 <found_tbl_by_va>
80100332:	6a 10                	push   $0x10
80100334:	8d 7c 24 08          	lea    0x8(%esp),%edi
80100338:	57                   	push   %edi
80100339:	50                   	push   %eax
8010033a:	e8 83 0c 00 00       	call   80100fc2 <itoa>
8010033f:	83 c4 08             	add    $0x8,%esp
80100342:	57                   	push   %edi
80100343:	6a 0f                	push   $0xf
80100345:	e8 ea 0c 00 00       	call   80101034 <write_string>
8010034a:	83 c4 08             	add    $0x8,%esp
8010034d:	68 97 11 10 80       	push   $0x80101197
80100352:	6a 0f                	push   $0xf
80100354:	e8 db 0c 00 00       	call   80101034 <write_string>
80100359:	83 c4 10             	add    $0x10,%esp
8010035c:	39 f5                	cmp    %esi,%ebp
8010035e:	77 25                	ja     80100385 <vm_map+0x7a>
80100360:	89 eb                	mov    %ebp,%ebx
80100362:	8b 7c 24 38          	mov    0x38(%esp),%edi
80100366:	83 cf 07             	or     $0x7,%edi
80100369:	29 ee                	sub    %ebp,%esi
8010036b:	83 ec 0c             	sub    $0xc,%esp
8010036e:	53                   	push   %ebx
8010036f:	e8 73 ff ff ff       	call   801002e7 <found_tbl_by_va>
80100374:	89 38                	mov    %edi,(%eax)
80100376:	81 c3 00 40 00 00    	add    $0x4000,%ebx
8010037c:	83 c4 10             	add    $0x10,%esp
8010037f:	89 d8                	mov    %ebx,%eax
80100381:	01 f0                	add    %esi,%eax
80100383:	73 e6                	jae    8010036b <vm_map+0x60>
80100385:	83 c4 1c             	add    $0x1c,%esp
80100388:	5b                   	pop    %ebx
80100389:	5e                   	pop    %esi
8010038a:	5f                   	pop    %edi
8010038b:	5d                   	pop    %ebp
8010038c:	c3                   	ret    

8010038d <kinit_paging>:
8010038d:	57                   	push   %edi
8010038e:	56                   	push   %esi
8010038f:	53                   	push   %ebx
80100390:	83 ec 10             	sub    $0x10,%esp
80100393:	8b 54 24 20          	mov    0x20(%esp),%edx
80100397:	8b 4c 24 24          	mov    0x24(%esp),%ecx
8010039b:	8d 04 12             	lea    (%edx,%edx,1),%eax
8010039e:	01 d0                	add    %edx,%eax
801003a0:	8d 34 c1             	lea    (%ecx,%eax,8),%esi
801003a3:	39 f1                	cmp    %esi,%ecx
801003a5:	73 2b                	jae    801003d2 <kinit_paging+0x45>
801003a7:	bb 00 00 00 00       	mov    $0x0,%ebx
801003ac:	eb 07                	jmp    801003b5 <kinit_paging+0x28>
801003ae:	83 c1 18             	add    $0x18,%ecx
801003b1:	39 f1                	cmp    %esi,%ecx
801003b3:	73 22                	jae    801003d7 <kinit_paging+0x4a>
801003b5:	8b 41 04             	mov    0x4(%ecx),%eax
801003b8:	c1 e0 10             	shl    $0x10,%eax
801003bb:	0b 01                	or     (%ecx),%eax
801003bd:	89 c2                	mov    %eax,%edx
801003bf:	8b 41 0c             	mov    0xc(%ecx),%eax
801003c2:	c1 e0 10             	shl    $0x10,%eax
801003c5:	0b 41 08             	or     0x8(%ecx),%eax
801003c8:	01 d0                	add    %edx,%eax
801003ca:	39 c3                	cmp    %eax,%ebx
801003cc:	73 e0                	jae    801003ae <kinit_paging+0x21>
801003ce:	89 c3                	mov    %eax,%ebx
801003d0:	eb dc                	jmp    801003ae <kinit_paging+0x21>
801003d2:	bb 00 00 00 00       	mov    $0x0,%ebx
801003d7:	83 ec 08             	sub    $0x8,%esp
801003da:	68 00 12 10 80       	push   $0x80101200
801003df:	6a 0f                	push   $0xf
801003e1:	e8 4e 0c 00 00       	call   80101034 <write_string>
801003e6:	83 c4 0c             	add    $0xc,%esp
801003e9:	6a 0a                	push   $0xa
801003eb:	8d 7c 24 08          	lea    0x8(%esp),%edi
801003ef:	57                   	push   %edi
801003f0:	89 d8                	mov    %ebx,%eax
801003f2:	c1 e8 14             	shr    $0x14,%eax
801003f5:	50                   	push   %eax
801003f6:	e8 c7 0b 00 00       	call   80100fc2 <itoa>
801003fb:	83 c4 08             	add    $0x8,%esp
801003fe:	57                   	push   %edi
801003ff:	6a 0f                	push   $0xf
80100401:	e8 2e 0c 00 00       	call   80101034 <write_string>
80100406:	83 c4 08             	add    $0x8,%esp
80100409:	68 10 12 10 80       	push   $0x80101210
8010040e:	6a 0f                	push   $0xf
80100410:	e8 1f 0c 00 00       	call   80101034 <write_string>
80100415:	83 c4 0c             	add    $0xc,%esp
80100418:	f7 c3 ff ff 3f 00    	test   $0x3fffff,%ebx
8010041e:	0f 95 c0             	setne  %al
80100421:	0f b6 c0             	movzbl %al,%eax
80100424:	c1 eb 16             	shr    $0x16,%ebx
80100427:	01 c3                	add    %eax,%ebx
80100429:	89 de                	mov    %ebx,%esi
8010042b:	6a 0a                	push   $0xa
8010042d:	57                   	push   %edi
8010042e:	53                   	push   %ebx
8010042f:	e8 8e 0b 00 00       	call   80100fc2 <itoa>
80100434:	83 c4 08             	add    $0x8,%esp
80100437:	68 15 12 10 80       	push   $0x80101215
8010043c:	6a 0f                	push   $0xf
8010043e:	e8 f1 0b 00 00       	call   80101034 <write_string>
80100443:	83 c4 08             	add    $0x8,%esp
80100446:	57                   	push   %edi
80100447:	6a 0f                	push   $0xf
80100449:	e8 e6 0b 00 00       	call   80101034 <write_string>
8010044e:	83 c4 08             	add    $0x8,%esp
80100451:	68 1e 12 10 80       	push   $0x8010121e
80100456:	6a 0f                	push   $0xf
80100458:	e8 d7 0b 00 00       	call   80101034 <write_string>
8010045d:	83 c4 10             	add    $0x10,%esp
80100460:	85 db                	test   %ebx,%ebx
80100462:	74 43                	je     801004a7 <kinit_paging+0x11a>
80100464:	b9 00 00 00 00       	mov    $0x0,%ecx
80100469:	ba 07 1c 20 00       	mov    $0x201c07,%edx
8010046e:	b8 00 0c 20 80       	mov    $0x80200c00,%eax
80100473:	89 10                	mov    %edx,(%eax)
80100475:	81 c2 00 10 00 00    	add    $0x1000,%edx
8010047b:	41                   	inc    %ecx
8010047c:	8d 40 04             	lea    0x4(%eax),%eax
8010047f:	39 ce                	cmp    %ecx,%esi
80100481:	75 f0                	jne    80100473 <kinit_paging+0xe6>
80100483:	c1 e3 0a             	shl    $0xa,%ebx
80100486:	74 1f                	je     801004a7 <kinit_paging+0x11a>
80100488:	b9 00 00 00 00       	mov    $0x0,%ecx
8010048d:	ba 07 00 00 00       	mov    $0x7,%edx
80100492:	b8 00 1c 20 80       	mov    $0x80201c00,%eax
80100497:	89 10                	mov    %edx,(%eax)
80100499:	81 c2 00 10 00 00    	add    $0x1000,%edx
8010049f:	41                   	inc    %ecx
801004a0:	8d 40 04             	lea    0x4(%eax),%eax
801004a3:	39 cb                	cmp    %ecx,%ebx
801004a5:	75 f0                	jne    80100497 <kinit_paging+0x10a>
801004a7:	83 ec 08             	sub    $0x8,%esp
801004aa:	68 3c 12 10 80       	push   $0x8010123c
801004af:	6a 0f                	push   $0xf
801004b1:	e8 7e 0b 00 00       	call   80101034 <write_string>
801004b6:	83 c4 0c             	add    $0xc,%esp
801004b9:	6a 00                	push   $0x0
801004bb:	68 00 00 00 0f       	push   $0xf000000
801004c0:	68 00 00 00 80       	push   $0x80000000
801004c5:	e8 41 fe ff ff       	call   8010030b <vm_map>
801004ca:	83 c4 08             	add    $0x8,%esp
801004cd:	68 51 12 10 80       	push   $0x80101251
801004d2:	6a 0f                	push   $0xf
801004d4:	e8 5b 0b 00 00       	call   80101034 <write_string>
801004d9:	83 c4 0c             	add    $0xc,%esp
801004dc:	6a 10                	push   $0x10
801004de:	8d 5c 24 08          	lea    0x8(%esp),%ebx
801004e2:	53                   	push   %ebx
801004e3:	68 00 0c 20 00       	push   $0x200c00
801004e8:	e8 d5 0a 00 00       	call   80100fc2 <itoa>
801004ed:	83 c4 08             	add    $0x8,%esp
801004f0:	53                   	push   %ebx
801004f1:	6a 0f                	push   $0xf
801004f3:	e8 3c 0b 00 00       	call   80101034 <write_string>
801004f8:	83 c4 08             	add    $0x8,%esp
801004fb:	68 97 11 10 80       	push   $0x80101197
80100500:	6a 0f                	push   $0xf
80100502:	e8 2d 0b 00 00       	call   80101034 <write_string>
80100507:	b8 00 0c 20 00       	mov    $0x200c00,%eax
8010050c:	0f 22 d8             	mov    %eax,%cr3
8010050f:	83 c4 20             	add    $0x20,%esp
80100512:	5b                   	pop    %ebx
80100513:	5e                   	pop    %esi
80100514:	5f                   	pop    %edi
80100515:	c3                   	ret    

80100516 <kinit_gdt>:
80100516:	c3                   	ret    
80100517:	66 90                	xchg   %ax,%ax
80100519:	66 90                	xchg   %ax,%ax
8010051b:	66 90                	xchg   %ax,%ax
8010051d:	66 90                	xchg   %ax,%ax
8010051f:	90                   	nop

80100520 <vector_handler>:
80100520:	60                   	pusha  
80100521:	1e                   	push   %ds
80100522:	06                   	push   %es
80100523:	0f a0                	push   %fs
80100525:	0f a8                	push   %gs
80100527:	89 d6                	mov    %edx,%esi
80100529:	66 b8 10 00          	mov    $0x10,%ax
8010052d:	8e d8                	mov    %eax,%ds
8010052f:	8e c0                	mov    %eax,%es
80100531:	89 f2                	mov    %esi,%edx
80100533:	54                   	push   %esp
80100534:	e8 ef fa ff ff       	call   80100028 <interrupt_handler>
80100539:	83 c4 04             	add    $0x4,%esp

8010053c <vector_handler.vector_handler_ret>:
8010053c:	0f a9                	pop    %gs
8010053e:	0f a1                	pop    %fs
80100540:	07                   	pop    %es
80100541:	1f                   	pop    %ds
80100542:	61                   	popa   
80100543:	83 c4 08             	add    $0x8,%esp
80100546:	cf                   	iret   
80100547:	66 90                	xchg   %ax,%ax
80100549:	66 90                	xchg   %ax,%ax
8010054b:	66 90                	xchg   %ax,%ax
8010054d:	66 90                	xchg   %ax,%ax
8010054f:	90                   	nop

80100550 <vector0>:
80100550:	6a 00                	push   $0x0
80100552:	6a 00                	push   $0x0
80100554:	e9 c7 ff ff ff       	jmp    80100520 <vector_handler>

80100559 <vector1>:
80100559:	6a 00                	push   $0x0
8010055b:	6a 01                	push   $0x1
8010055d:	e9 be ff ff ff       	jmp    80100520 <vector_handler>

80100562 <vector2>:
80100562:	6a 00                	push   $0x0
80100564:	6a 02                	push   $0x2
80100566:	e9 b5 ff ff ff       	jmp    80100520 <vector_handler>

8010056b <vector3>:
8010056b:	6a 00                	push   $0x0
8010056d:	6a 03                	push   $0x3
8010056f:	e9 ac ff ff ff       	jmp    80100520 <vector_handler>

80100574 <vector4>:
80100574:	6a 00                	push   $0x0
80100576:	6a 04                	push   $0x4
80100578:	e9 a3 ff ff ff       	jmp    80100520 <vector_handler>

8010057d <vector5>:
8010057d:	6a 00                	push   $0x0
8010057f:	6a 05                	push   $0x5
80100581:	e9 9a ff ff ff       	jmp    80100520 <vector_handler>

80100586 <vector6>:
80100586:	6a 00                	push   $0x0
80100588:	6a 06                	push   $0x6
8010058a:	e9 91 ff ff ff       	jmp    80100520 <vector_handler>

8010058f <vector7>:
8010058f:	6a 00                	push   $0x0
80100591:	6a 07                	push   $0x7
80100593:	e9 88 ff ff ff       	jmp    80100520 <vector_handler>

80100598 <vector8>:
80100598:	6a 08                	push   $0x8
8010059a:	e9 81 ff ff ff       	jmp    80100520 <vector_handler>

8010059f <vector9>:
8010059f:	6a 00                	push   $0x0
801005a1:	6a 09                	push   $0x9
801005a3:	e9 78 ff ff ff       	jmp    80100520 <vector_handler>

801005a8 <vector10>:
801005a8:	6a 0a                	push   $0xa
801005aa:	e9 71 ff ff ff       	jmp    80100520 <vector_handler>

801005af <vector11>:
801005af:	6a 0b                	push   $0xb
801005b1:	e9 6a ff ff ff       	jmp    80100520 <vector_handler>

801005b6 <vector12>:
801005b6:	6a 0c                	push   $0xc
801005b8:	e9 63 ff ff ff       	jmp    80100520 <vector_handler>

801005bd <vector13>:
801005bd:	6a 0d                	push   $0xd
801005bf:	e9 5c ff ff ff       	jmp    80100520 <vector_handler>

801005c4 <vector14>:
801005c4:	6a 0e                	push   $0xe
801005c6:	e9 55 ff ff ff       	jmp    80100520 <vector_handler>

801005cb <vector15>:
801005cb:	6a 00                	push   $0x0
801005cd:	6a 0f                	push   $0xf
801005cf:	e9 4c ff ff ff       	jmp    80100520 <vector_handler>

801005d4 <vector16>:
801005d4:	6a 00                	push   $0x0
801005d6:	6a 10                	push   $0x10
801005d8:	e9 43 ff ff ff       	jmp    80100520 <vector_handler>

801005dd <vector17>:
801005dd:	6a 11                	push   $0x11
801005df:	e9 3c ff ff ff       	jmp    80100520 <vector_handler>

801005e4 <vector18>:
801005e4:	6a 00                	push   $0x0
801005e6:	6a 12                	push   $0x12
801005e8:	e9 33 ff ff ff       	jmp    80100520 <vector_handler>

801005ed <vector19>:
801005ed:	6a 00                	push   $0x0
801005ef:	6a 13                	push   $0x13
801005f1:	e9 2a ff ff ff       	jmp    80100520 <vector_handler>

801005f6 <vector20>:
801005f6:	6a 00                	push   $0x0
801005f8:	6a 14                	push   $0x14
801005fa:	e9 21 ff ff ff       	jmp    80100520 <vector_handler>

801005ff <vector21>:
801005ff:	6a 00                	push   $0x0
80100601:	6a 15                	push   $0x15
80100603:	e9 18 ff ff ff       	jmp    80100520 <vector_handler>

80100608 <vector22>:
80100608:	6a 00                	push   $0x0
8010060a:	6a 16                	push   $0x16
8010060c:	e9 0f ff ff ff       	jmp    80100520 <vector_handler>

80100611 <vector23>:
80100611:	6a 00                	push   $0x0
80100613:	6a 17                	push   $0x17
80100615:	e9 06 ff ff ff       	jmp    80100520 <vector_handler>

8010061a <vector24>:
8010061a:	6a 00                	push   $0x0
8010061c:	6a 18                	push   $0x18
8010061e:	e9 fd fe ff ff       	jmp    80100520 <vector_handler>

80100623 <vector25>:
80100623:	6a 00                	push   $0x0
80100625:	6a 19                	push   $0x19
80100627:	e9 f4 fe ff ff       	jmp    80100520 <vector_handler>

8010062c <vector26>:
8010062c:	6a 00                	push   $0x0
8010062e:	6a 1a                	push   $0x1a
80100630:	e9 eb fe ff ff       	jmp    80100520 <vector_handler>

80100635 <vector27>:
80100635:	6a 00                	push   $0x0
80100637:	6a 1b                	push   $0x1b
80100639:	e9 e2 fe ff ff       	jmp    80100520 <vector_handler>

8010063e <vector28>:
8010063e:	6a 00                	push   $0x0
80100640:	6a 1c                	push   $0x1c
80100642:	e9 d9 fe ff ff       	jmp    80100520 <vector_handler>

80100647 <vector29>:
80100647:	6a 00                	push   $0x0
80100649:	6a 1d                	push   $0x1d
8010064b:	e9 d0 fe ff ff       	jmp    80100520 <vector_handler>

80100650 <vector30>:
80100650:	6a 00                	push   $0x0
80100652:	6a 1e                	push   $0x1e
80100654:	e9 c7 fe ff ff       	jmp    80100520 <vector_handler>

80100659 <vector31>:
80100659:	6a 00                	push   $0x0
8010065b:	6a 1f                	push   $0x1f
8010065d:	e9 be fe ff ff       	jmp    80100520 <vector_handler>

80100662 <vector32>:
80100662:	6a 00                	push   $0x0
80100664:	6a 20                	push   $0x20
80100666:	e9 b5 fe ff ff       	jmp    80100520 <vector_handler>

8010066b <vector33>:
8010066b:	6a 00                	push   $0x0
8010066d:	6a 21                	push   $0x21
8010066f:	e9 ac fe ff ff       	jmp    80100520 <vector_handler>

80100674 <vector34>:
80100674:	6a 00                	push   $0x0
80100676:	6a 22                	push   $0x22
80100678:	e9 a3 fe ff ff       	jmp    80100520 <vector_handler>

8010067d <vector35>:
8010067d:	6a 00                	push   $0x0
8010067f:	6a 23                	push   $0x23
80100681:	e9 9a fe ff ff       	jmp    80100520 <vector_handler>

80100686 <vector36>:
80100686:	6a 00                	push   $0x0
80100688:	6a 24                	push   $0x24
8010068a:	e9 91 fe ff ff       	jmp    80100520 <vector_handler>

8010068f <vector37>:
8010068f:	6a 00                	push   $0x0
80100691:	6a 25                	push   $0x25
80100693:	e9 88 fe ff ff       	jmp    80100520 <vector_handler>

80100698 <vector38>:
80100698:	6a 00                	push   $0x0
8010069a:	6a 26                	push   $0x26
8010069c:	e9 7f fe ff ff       	jmp    80100520 <vector_handler>

801006a1 <vector39>:
801006a1:	6a 00                	push   $0x0
801006a3:	6a 27                	push   $0x27
801006a5:	e9 76 fe ff ff       	jmp    80100520 <vector_handler>

801006aa <vector40>:
801006aa:	6a 00                	push   $0x0
801006ac:	6a 28                	push   $0x28
801006ae:	e9 6d fe ff ff       	jmp    80100520 <vector_handler>

801006b3 <vector41>:
801006b3:	6a 00                	push   $0x0
801006b5:	6a 29                	push   $0x29
801006b7:	e9 64 fe ff ff       	jmp    80100520 <vector_handler>

801006bc <vector42>:
801006bc:	6a 00                	push   $0x0
801006be:	6a 2a                	push   $0x2a
801006c0:	e9 5b fe ff ff       	jmp    80100520 <vector_handler>

801006c5 <vector43>:
801006c5:	6a 00                	push   $0x0
801006c7:	6a 2b                	push   $0x2b
801006c9:	e9 52 fe ff ff       	jmp    80100520 <vector_handler>

801006ce <vector44>:
801006ce:	6a 00                	push   $0x0
801006d0:	6a 2c                	push   $0x2c
801006d2:	e9 49 fe ff ff       	jmp    80100520 <vector_handler>

801006d7 <vector45>:
801006d7:	6a 00                	push   $0x0
801006d9:	6a 2d                	push   $0x2d
801006db:	e9 40 fe ff ff       	jmp    80100520 <vector_handler>

801006e0 <vector46>:
801006e0:	6a 00                	push   $0x0
801006e2:	6a 2e                	push   $0x2e
801006e4:	e9 37 fe ff ff       	jmp    80100520 <vector_handler>

801006e9 <vector47>:
801006e9:	6a 00                	push   $0x0
801006eb:	6a 2f                	push   $0x2f
801006ed:	e9 2e fe ff ff       	jmp    80100520 <vector_handler>

801006f2 <vector48>:
801006f2:	6a 00                	push   $0x0
801006f4:	6a 30                	push   $0x30
801006f6:	e9 25 fe ff ff       	jmp    80100520 <vector_handler>

801006fb <vector49>:
801006fb:	6a 00                	push   $0x0
801006fd:	6a 31                	push   $0x31
801006ff:	e9 1c fe ff ff       	jmp    80100520 <vector_handler>

80100704 <vector50>:
80100704:	6a 00                	push   $0x0
80100706:	6a 32                	push   $0x32
80100708:	e9 13 fe ff ff       	jmp    80100520 <vector_handler>

8010070d <vector51>:
8010070d:	6a 00                	push   $0x0
8010070f:	6a 33                	push   $0x33
80100711:	e9 0a fe ff ff       	jmp    80100520 <vector_handler>

80100716 <vector52>:
80100716:	6a 00                	push   $0x0
80100718:	6a 34                	push   $0x34
8010071a:	e9 01 fe ff ff       	jmp    80100520 <vector_handler>

8010071f <vector53>:
8010071f:	6a 00                	push   $0x0
80100721:	6a 35                	push   $0x35
80100723:	e9 f8 fd ff ff       	jmp    80100520 <vector_handler>

80100728 <vector54>:
80100728:	6a 00                	push   $0x0
8010072a:	6a 36                	push   $0x36
8010072c:	e9 ef fd ff ff       	jmp    80100520 <vector_handler>

80100731 <vector55>:
80100731:	6a 00                	push   $0x0
80100733:	6a 37                	push   $0x37
80100735:	e9 e6 fd ff ff       	jmp    80100520 <vector_handler>

8010073a <vector56>:
8010073a:	6a 00                	push   $0x0
8010073c:	6a 38                	push   $0x38
8010073e:	e9 dd fd ff ff       	jmp    80100520 <vector_handler>

80100743 <vector57>:
80100743:	6a 00                	push   $0x0
80100745:	6a 39                	push   $0x39
80100747:	e9 d4 fd ff ff       	jmp    80100520 <vector_handler>

8010074c <vector58>:
8010074c:	6a 00                	push   $0x0
8010074e:	6a 3a                	push   $0x3a
80100750:	e9 cb fd ff ff       	jmp    80100520 <vector_handler>

80100755 <vector59>:
80100755:	6a 00                	push   $0x0
80100757:	6a 3b                	push   $0x3b
80100759:	e9 c2 fd ff ff       	jmp    80100520 <vector_handler>

8010075e <vector60>:
8010075e:	6a 00                	push   $0x0
80100760:	6a 3c                	push   $0x3c
80100762:	e9 b9 fd ff ff       	jmp    80100520 <vector_handler>

80100767 <vector61>:
80100767:	6a 00                	push   $0x0
80100769:	6a 3d                	push   $0x3d
8010076b:	e9 b0 fd ff ff       	jmp    80100520 <vector_handler>

80100770 <vector62>:
80100770:	6a 00                	push   $0x0
80100772:	6a 3e                	push   $0x3e
80100774:	e9 a7 fd ff ff       	jmp    80100520 <vector_handler>

80100779 <vector63>:
80100779:	6a 00                	push   $0x0
8010077b:	6a 3f                	push   $0x3f
8010077d:	e9 9e fd ff ff       	jmp    80100520 <vector_handler>

80100782 <vector64>:
80100782:	6a 00                	push   $0x0
80100784:	6a 40                	push   $0x40
80100786:	e9 95 fd ff ff       	jmp    80100520 <vector_handler>

8010078b <vector65>:
8010078b:	6a 00                	push   $0x0
8010078d:	6a 41                	push   $0x41
8010078f:	e9 8c fd ff ff       	jmp    80100520 <vector_handler>

80100794 <vector66>:
80100794:	6a 00                	push   $0x0
80100796:	6a 42                	push   $0x42
80100798:	e9 83 fd ff ff       	jmp    80100520 <vector_handler>

8010079d <vector67>:
8010079d:	6a 00                	push   $0x0
8010079f:	6a 43                	push   $0x43
801007a1:	e9 7a fd ff ff       	jmp    80100520 <vector_handler>

801007a6 <vector68>:
801007a6:	6a 00                	push   $0x0
801007a8:	6a 44                	push   $0x44
801007aa:	e9 71 fd ff ff       	jmp    80100520 <vector_handler>

801007af <vector69>:
801007af:	6a 00                	push   $0x0
801007b1:	6a 45                	push   $0x45
801007b3:	e9 68 fd ff ff       	jmp    80100520 <vector_handler>

801007b8 <vector70>:
801007b8:	6a 00                	push   $0x0
801007ba:	6a 46                	push   $0x46
801007bc:	e9 5f fd ff ff       	jmp    80100520 <vector_handler>

801007c1 <vector71>:
801007c1:	6a 00                	push   $0x0
801007c3:	6a 47                	push   $0x47
801007c5:	e9 56 fd ff ff       	jmp    80100520 <vector_handler>

801007ca <vector72>:
801007ca:	6a 00                	push   $0x0
801007cc:	6a 48                	push   $0x48
801007ce:	e9 4d fd ff ff       	jmp    80100520 <vector_handler>

801007d3 <vector73>:
801007d3:	6a 00                	push   $0x0
801007d5:	6a 49                	push   $0x49
801007d7:	e9 44 fd ff ff       	jmp    80100520 <vector_handler>

801007dc <vector74>:
801007dc:	6a 00                	push   $0x0
801007de:	6a 4a                	push   $0x4a
801007e0:	e9 3b fd ff ff       	jmp    80100520 <vector_handler>

801007e5 <vector75>:
801007e5:	6a 00                	push   $0x0
801007e7:	6a 4b                	push   $0x4b
801007e9:	e9 32 fd ff ff       	jmp    80100520 <vector_handler>

801007ee <vector76>:
801007ee:	6a 00                	push   $0x0
801007f0:	6a 4c                	push   $0x4c
801007f2:	e9 29 fd ff ff       	jmp    80100520 <vector_handler>

801007f7 <vector77>:
801007f7:	6a 00                	push   $0x0
801007f9:	6a 4d                	push   $0x4d
801007fb:	e9 20 fd ff ff       	jmp    80100520 <vector_handler>

80100800 <vector78>:
80100800:	6a 00                	push   $0x0
80100802:	6a 4e                	push   $0x4e
80100804:	e9 17 fd ff ff       	jmp    80100520 <vector_handler>

80100809 <vector79>:
80100809:	6a 00                	push   $0x0
8010080b:	6a 4f                	push   $0x4f
8010080d:	e9 0e fd ff ff       	jmp    80100520 <vector_handler>

80100812 <vector80>:
80100812:	6a 00                	push   $0x0
80100814:	6a 50                	push   $0x50
80100816:	e9 05 fd ff ff       	jmp    80100520 <vector_handler>

8010081b <vector81>:
8010081b:	6a 00                	push   $0x0
8010081d:	6a 51                	push   $0x51
8010081f:	e9 fc fc ff ff       	jmp    80100520 <vector_handler>

80100824 <vector82>:
80100824:	6a 00                	push   $0x0
80100826:	6a 52                	push   $0x52
80100828:	e9 f3 fc ff ff       	jmp    80100520 <vector_handler>

8010082d <vector83>:
8010082d:	6a 00                	push   $0x0
8010082f:	6a 53                	push   $0x53
80100831:	e9 ea fc ff ff       	jmp    80100520 <vector_handler>

80100836 <vector84>:
80100836:	6a 00                	push   $0x0
80100838:	6a 54                	push   $0x54
8010083a:	e9 e1 fc ff ff       	jmp    80100520 <vector_handler>

8010083f <vector85>:
8010083f:	6a 00                	push   $0x0
80100841:	6a 55                	push   $0x55
80100843:	e9 d8 fc ff ff       	jmp    80100520 <vector_handler>

80100848 <vector86>:
80100848:	6a 00                	push   $0x0
8010084a:	6a 56                	push   $0x56
8010084c:	e9 cf fc ff ff       	jmp    80100520 <vector_handler>

80100851 <vector87>:
80100851:	6a 00                	push   $0x0
80100853:	6a 57                	push   $0x57
80100855:	e9 c6 fc ff ff       	jmp    80100520 <vector_handler>

8010085a <vector88>:
8010085a:	6a 00                	push   $0x0
8010085c:	6a 58                	push   $0x58
8010085e:	e9 bd fc ff ff       	jmp    80100520 <vector_handler>

80100863 <vector89>:
80100863:	6a 00                	push   $0x0
80100865:	6a 59                	push   $0x59
80100867:	e9 b4 fc ff ff       	jmp    80100520 <vector_handler>

8010086c <vector90>:
8010086c:	6a 00                	push   $0x0
8010086e:	6a 5a                	push   $0x5a
80100870:	e9 ab fc ff ff       	jmp    80100520 <vector_handler>

80100875 <vector91>:
80100875:	6a 00                	push   $0x0
80100877:	6a 5b                	push   $0x5b
80100879:	e9 a2 fc ff ff       	jmp    80100520 <vector_handler>

8010087e <vector92>:
8010087e:	6a 00                	push   $0x0
80100880:	6a 5c                	push   $0x5c
80100882:	e9 99 fc ff ff       	jmp    80100520 <vector_handler>

80100887 <vector93>:
80100887:	6a 00                	push   $0x0
80100889:	6a 5d                	push   $0x5d
8010088b:	e9 90 fc ff ff       	jmp    80100520 <vector_handler>

80100890 <vector94>:
80100890:	6a 00                	push   $0x0
80100892:	6a 5e                	push   $0x5e
80100894:	e9 87 fc ff ff       	jmp    80100520 <vector_handler>

80100899 <vector95>:
80100899:	6a 00                	push   $0x0
8010089b:	6a 5f                	push   $0x5f
8010089d:	e9 7e fc ff ff       	jmp    80100520 <vector_handler>

801008a2 <vector96>:
801008a2:	6a 00                	push   $0x0
801008a4:	6a 60                	push   $0x60
801008a6:	e9 75 fc ff ff       	jmp    80100520 <vector_handler>

801008ab <vector97>:
801008ab:	6a 00                	push   $0x0
801008ad:	6a 61                	push   $0x61
801008af:	e9 6c fc ff ff       	jmp    80100520 <vector_handler>

801008b4 <vector98>:
801008b4:	6a 00                	push   $0x0
801008b6:	6a 62                	push   $0x62
801008b8:	e9 63 fc ff ff       	jmp    80100520 <vector_handler>

801008bd <vector99>:
801008bd:	6a 00                	push   $0x0
801008bf:	6a 63                	push   $0x63
801008c1:	e9 5a fc ff ff       	jmp    80100520 <vector_handler>

801008c6 <vector100>:
801008c6:	6a 00                	push   $0x0
801008c8:	6a 64                	push   $0x64
801008ca:	e9 51 fc ff ff       	jmp    80100520 <vector_handler>

801008cf <vector101>:
801008cf:	6a 00                	push   $0x0
801008d1:	6a 65                	push   $0x65
801008d3:	e9 48 fc ff ff       	jmp    80100520 <vector_handler>

801008d8 <vector102>:
801008d8:	6a 00                	push   $0x0
801008da:	6a 66                	push   $0x66
801008dc:	e9 3f fc ff ff       	jmp    80100520 <vector_handler>

801008e1 <vector103>:
801008e1:	6a 00                	push   $0x0
801008e3:	6a 67                	push   $0x67
801008e5:	e9 36 fc ff ff       	jmp    80100520 <vector_handler>

801008ea <vector104>:
801008ea:	6a 00                	push   $0x0
801008ec:	6a 68                	push   $0x68
801008ee:	e9 2d fc ff ff       	jmp    80100520 <vector_handler>

801008f3 <vector105>:
801008f3:	6a 00                	push   $0x0
801008f5:	6a 69                	push   $0x69
801008f7:	e9 24 fc ff ff       	jmp    80100520 <vector_handler>

801008fc <vector106>:
801008fc:	6a 00                	push   $0x0
801008fe:	6a 6a                	push   $0x6a
80100900:	e9 1b fc ff ff       	jmp    80100520 <vector_handler>

80100905 <vector107>:
80100905:	6a 00                	push   $0x0
80100907:	6a 6b                	push   $0x6b
80100909:	e9 12 fc ff ff       	jmp    80100520 <vector_handler>

8010090e <vector108>:
8010090e:	6a 00                	push   $0x0
80100910:	6a 6c                	push   $0x6c
80100912:	e9 09 fc ff ff       	jmp    80100520 <vector_handler>

80100917 <vector109>:
80100917:	6a 00                	push   $0x0
80100919:	6a 6d                	push   $0x6d
8010091b:	e9 00 fc ff ff       	jmp    80100520 <vector_handler>

80100920 <vector110>:
80100920:	6a 00                	push   $0x0
80100922:	6a 6e                	push   $0x6e
80100924:	e9 f7 fb ff ff       	jmp    80100520 <vector_handler>

80100929 <vector111>:
80100929:	6a 00                	push   $0x0
8010092b:	6a 6f                	push   $0x6f
8010092d:	e9 ee fb ff ff       	jmp    80100520 <vector_handler>

80100932 <vector112>:
80100932:	6a 00                	push   $0x0
80100934:	6a 70                	push   $0x70
80100936:	e9 e5 fb ff ff       	jmp    80100520 <vector_handler>

8010093b <vector113>:
8010093b:	6a 00                	push   $0x0
8010093d:	6a 71                	push   $0x71
8010093f:	e9 dc fb ff ff       	jmp    80100520 <vector_handler>

80100944 <vector114>:
80100944:	6a 00                	push   $0x0
80100946:	6a 72                	push   $0x72
80100948:	e9 d3 fb ff ff       	jmp    80100520 <vector_handler>

8010094d <vector115>:
8010094d:	6a 00                	push   $0x0
8010094f:	6a 73                	push   $0x73
80100951:	e9 ca fb ff ff       	jmp    80100520 <vector_handler>

80100956 <vector116>:
80100956:	6a 00                	push   $0x0
80100958:	6a 74                	push   $0x74
8010095a:	e9 c1 fb ff ff       	jmp    80100520 <vector_handler>

8010095f <vector117>:
8010095f:	6a 00                	push   $0x0
80100961:	6a 75                	push   $0x75
80100963:	e9 b8 fb ff ff       	jmp    80100520 <vector_handler>

80100968 <vector118>:
80100968:	6a 00                	push   $0x0
8010096a:	6a 76                	push   $0x76
8010096c:	e9 af fb ff ff       	jmp    80100520 <vector_handler>

80100971 <vector119>:
80100971:	6a 00                	push   $0x0
80100973:	6a 77                	push   $0x77
80100975:	e9 a6 fb ff ff       	jmp    80100520 <vector_handler>

8010097a <vector120>:
8010097a:	6a 00                	push   $0x0
8010097c:	6a 78                	push   $0x78
8010097e:	e9 9d fb ff ff       	jmp    80100520 <vector_handler>

80100983 <vector121>:
80100983:	6a 00                	push   $0x0
80100985:	6a 79                	push   $0x79
80100987:	e9 94 fb ff ff       	jmp    80100520 <vector_handler>

8010098c <vector122>:
8010098c:	6a 00                	push   $0x0
8010098e:	6a 7a                	push   $0x7a
80100990:	e9 8b fb ff ff       	jmp    80100520 <vector_handler>

80100995 <vector123>:
80100995:	6a 00                	push   $0x0
80100997:	6a 7b                	push   $0x7b
80100999:	e9 82 fb ff ff       	jmp    80100520 <vector_handler>

8010099e <vector124>:
8010099e:	6a 00                	push   $0x0
801009a0:	6a 7c                	push   $0x7c
801009a2:	e9 79 fb ff ff       	jmp    80100520 <vector_handler>

801009a7 <vector125>:
801009a7:	6a 00                	push   $0x0
801009a9:	6a 7d                	push   $0x7d
801009ab:	e9 70 fb ff ff       	jmp    80100520 <vector_handler>

801009b0 <vector126>:
801009b0:	6a 00                	push   $0x0
801009b2:	6a 7e                	push   $0x7e
801009b4:	e9 67 fb ff ff       	jmp    80100520 <vector_handler>

801009b9 <vector127>:
801009b9:	6a 00                	push   $0x0
801009bb:	6a 7f                	push   $0x7f
801009bd:	e9 5e fb ff ff       	jmp    80100520 <vector_handler>

801009c2 <vector128>:
801009c2:	6a 00                	push   $0x0
801009c4:	68 80 00 00 00       	push   $0x80
801009c9:	e9 52 fb ff ff       	jmp    80100520 <vector_handler>

801009ce <vector129>:
801009ce:	6a 00                	push   $0x0
801009d0:	68 81 00 00 00       	push   $0x81
801009d5:	e9 46 fb ff ff       	jmp    80100520 <vector_handler>

801009da <vector130>:
801009da:	6a 00                	push   $0x0
801009dc:	68 82 00 00 00       	push   $0x82
801009e1:	e9 3a fb ff ff       	jmp    80100520 <vector_handler>

801009e6 <vector131>:
801009e6:	6a 00                	push   $0x0
801009e8:	68 83 00 00 00       	push   $0x83
801009ed:	e9 2e fb ff ff       	jmp    80100520 <vector_handler>

801009f2 <vector132>:
801009f2:	6a 00                	push   $0x0
801009f4:	68 84 00 00 00       	push   $0x84
801009f9:	e9 22 fb ff ff       	jmp    80100520 <vector_handler>

801009fe <vector133>:
801009fe:	6a 00                	push   $0x0
80100a00:	68 85 00 00 00       	push   $0x85
80100a05:	e9 16 fb ff ff       	jmp    80100520 <vector_handler>

80100a0a <vector134>:
80100a0a:	6a 00                	push   $0x0
80100a0c:	68 86 00 00 00       	push   $0x86
80100a11:	e9 0a fb ff ff       	jmp    80100520 <vector_handler>

80100a16 <vector135>:
80100a16:	6a 00                	push   $0x0
80100a18:	68 87 00 00 00       	push   $0x87
80100a1d:	e9 fe fa ff ff       	jmp    80100520 <vector_handler>

80100a22 <vector136>:
80100a22:	6a 00                	push   $0x0
80100a24:	68 88 00 00 00       	push   $0x88
80100a29:	e9 f2 fa ff ff       	jmp    80100520 <vector_handler>

80100a2e <vector137>:
80100a2e:	6a 00                	push   $0x0
80100a30:	68 89 00 00 00       	push   $0x89
80100a35:	e9 e6 fa ff ff       	jmp    80100520 <vector_handler>

80100a3a <vector138>:
80100a3a:	6a 00                	push   $0x0
80100a3c:	68 8a 00 00 00       	push   $0x8a
80100a41:	e9 da fa ff ff       	jmp    80100520 <vector_handler>

80100a46 <vector139>:
80100a46:	6a 00                	push   $0x0
80100a48:	68 8b 00 00 00       	push   $0x8b
80100a4d:	e9 ce fa ff ff       	jmp    80100520 <vector_handler>

80100a52 <vector140>:
80100a52:	6a 00                	push   $0x0
80100a54:	68 8c 00 00 00       	push   $0x8c
80100a59:	e9 c2 fa ff ff       	jmp    80100520 <vector_handler>

80100a5e <vector141>:
80100a5e:	6a 00                	push   $0x0
80100a60:	68 8d 00 00 00       	push   $0x8d
80100a65:	e9 b6 fa ff ff       	jmp    80100520 <vector_handler>

80100a6a <vector142>:
80100a6a:	6a 00                	push   $0x0
80100a6c:	68 8e 00 00 00       	push   $0x8e
80100a71:	e9 aa fa ff ff       	jmp    80100520 <vector_handler>

80100a76 <vector143>:
80100a76:	6a 00                	push   $0x0
80100a78:	68 8f 00 00 00       	push   $0x8f
80100a7d:	e9 9e fa ff ff       	jmp    80100520 <vector_handler>

80100a82 <vector144>:
80100a82:	6a 00                	push   $0x0
80100a84:	68 90 00 00 00       	push   $0x90
80100a89:	e9 92 fa ff ff       	jmp    80100520 <vector_handler>

80100a8e <vector145>:
80100a8e:	6a 00                	push   $0x0
80100a90:	68 91 00 00 00       	push   $0x91
80100a95:	e9 86 fa ff ff       	jmp    80100520 <vector_handler>

80100a9a <vector146>:
80100a9a:	6a 00                	push   $0x0
80100a9c:	68 92 00 00 00       	push   $0x92
80100aa1:	e9 7a fa ff ff       	jmp    80100520 <vector_handler>

80100aa6 <vector147>:
80100aa6:	6a 00                	push   $0x0
80100aa8:	68 93 00 00 00       	push   $0x93
80100aad:	e9 6e fa ff ff       	jmp    80100520 <vector_handler>

80100ab2 <vector148>:
80100ab2:	6a 00                	push   $0x0
80100ab4:	68 94 00 00 00       	push   $0x94
80100ab9:	e9 62 fa ff ff       	jmp    80100520 <vector_handler>

80100abe <vector149>:
80100abe:	6a 00                	push   $0x0
80100ac0:	68 95 00 00 00       	push   $0x95
80100ac5:	e9 56 fa ff ff       	jmp    80100520 <vector_handler>

80100aca <vector150>:
80100aca:	6a 00                	push   $0x0
80100acc:	68 96 00 00 00       	push   $0x96
80100ad1:	e9 4a fa ff ff       	jmp    80100520 <vector_handler>

80100ad6 <vector151>:
80100ad6:	6a 00                	push   $0x0
80100ad8:	68 97 00 00 00       	push   $0x97
80100add:	e9 3e fa ff ff       	jmp    80100520 <vector_handler>

80100ae2 <vector152>:
80100ae2:	6a 00                	push   $0x0
80100ae4:	68 98 00 00 00       	push   $0x98
80100ae9:	e9 32 fa ff ff       	jmp    80100520 <vector_handler>

80100aee <vector153>:
80100aee:	6a 00                	push   $0x0
80100af0:	68 99 00 00 00       	push   $0x99
80100af5:	e9 26 fa ff ff       	jmp    80100520 <vector_handler>

80100afa <vector154>:
80100afa:	6a 00                	push   $0x0
80100afc:	68 9a 00 00 00       	push   $0x9a
80100b01:	e9 1a fa ff ff       	jmp    80100520 <vector_handler>

80100b06 <vector155>:
80100b06:	6a 00                	push   $0x0
80100b08:	68 9b 00 00 00       	push   $0x9b
80100b0d:	e9 0e fa ff ff       	jmp    80100520 <vector_handler>

80100b12 <vector156>:
80100b12:	6a 00                	push   $0x0
80100b14:	68 9c 00 00 00       	push   $0x9c
80100b19:	e9 02 fa ff ff       	jmp    80100520 <vector_handler>

80100b1e <vector157>:
80100b1e:	6a 00                	push   $0x0
80100b20:	68 9d 00 00 00       	push   $0x9d
80100b25:	e9 f6 f9 ff ff       	jmp    80100520 <vector_handler>

80100b2a <vector158>:
80100b2a:	6a 00                	push   $0x0
80100b2c:	68 9e 00 00 00       	push   $0x9e
80100b31:	e9 ea f9 ff ff       	jmp    80100520 <vector_handler>

80100b36 <vector159>:
80100b36:	6a 00                	push   $0x0
80100b38:	68 9f 00 00 00       	push   $0x9f
80100b3d:	e9 de f9 ff ff       	jmp    80100520 <vector_handler>

80100b42 <vector160>:
80100b42:	6a 00                	push   $0x0
80100b44:	68 a0 00 00 00       	push   $0xa0
80100b49:	e9 d2 f9 ff ff       	jmp    80100520 <vector_handler>

80100b4e <vector161>:
80100b4e:	6a 00                	push   $0x0
80100b50:	68 a1 00 00 00       	push   $0xa1
80100b55:	e9 c6 f9 ff ff       	jmp    80100520 <vector_handler>

80100b5a <vector162>:
80100b5a:	6a 00                	push   $0x0
80100b5c:	68 a2 00 00 00       	push   $0xa2
80100b61:	e9 ba f9 ff ff       	jmp    80100520 <vector_handler>

80100b66 <vector163>:
80100b66:	6a 00                	push   $0x0
80100b68:	68 a3 00 00 00       	push   $0xa3
80100b6d:	e9 ae f9 ff ff       	jmp    80100520 <vector_handler>

80100b72 <vector164>:
80100b72:	6a 00                	push   $0x0
80100b74:	68 a4 00 00 00       	push   $0xa4
80100b79:	e9 a2 f9 ff ff       	jmp    80100520 <vector_handler>

80100b7e <vector165>:
80100b7e:	6a 00                	push   $0x0
80100b80:	68 a5 00 00 00       	push   $0xa5
80100b85:	e9 96 f9 ff ff       	jmp    80100520 <vector_handler>

80100b8a <vector166>:
80100b8a:	6a 00                	push   $0x0
80100b8c:	68 a6 00 00 00       	push   $0xa6
80100b91:	e9 8a f9 ff ff       	jmp    80100520 <vector_handler>

80100b96 <vector167>:
80100b96:	6a 00                	push   $0x0
80100b98:	68 a7 00 00 00       	push   $0xa7
80100b9d:	e9 7e f9 ff ff       	jmp    80100520 <vector_handler>

80100ba2 <vector168>:
80100ba2:	6a 00                	push   $0x0
80100ba4:	68 a8 00 00 00       	push   $0xa8
80100ba9:	e9 72 f9 ff ff       	jmp    80100520 <vector_handler>

80100bae <vector169>:
80100bae:	6a 00                	push   $0x0
80100bb0:	68 a9 00 00 00       	push   $0xa9
80100bb5:	e9 66 f9 ff ff       	jmp    80100520 <vector_handler>

80100bba <vector170>:
80100bba:	6a 00                	push   $0x0
80100bbc:	68 aa 00 00 00       	push   $0xaa
80100bc1:	e9 5a f9 ff ff       	jmp    80100520 <vector_handler>

80100bc6 <vector171>:
80100bc6:	6a 00                	push   $0x0
80100bc8:	68 ab 00 00 00       	push   $0xab
80100bcd:	e9 4e f9 ff ff       	jmp    80100520 <vector_handler>

80100bd2 <vector172>:
80100bd2:	6a 00                	push   $0x0
80100bd4:	68 ac 00 00 00       	push   $0xac
80100bd9:	e9 42 f9 ff ff       	jmp    80100520 <vector_handler>

80100bde <vector173>:
80100bde:	6a 00                	push   $0x0
80100be0:	68 ad 00 00 00       	push   $0xad
80100be5:	e9 36 f9 ff ff       	jmp    80100520 <vector_handler>

80100bea <vector174>:
80100bea:	6a 00                	push   $0x0
80100bec:	68 ae 00 00 00       	push   $0xae
80100bf1:	e9 2a f9 ff ff       	jmp    80100520 <vector_handler>

80100bf6 <vector175>:
80100bf6:	6a 00                	push   $0x0
80100bf8:	68 af 00 00 00       	push   $0xaf
80100bfd:	e9 1e f9 ff ff       	jmp    80100520 <vector_handler>

80100c02 <vector176>:
80100c02:	6a 00                	push   $0x0
80100c04:	68 b0 00 00 00       	push   $0xb0
80100c09:	e9 12 f9 ff ff       	jmp    80100520 <vector_handler>

80100c0e <vector177>:
80100c0e:	6a 00                	push   $0x0
80100c10:	68 b1 00 00 00       	push   $0xb1
80100c15:	e9 06 f9 ff ff       	jmp    80100520 <vector_handler>

80100c1a <vector178>:
80100c1a:	6a 00                	push   $0x0
80100c1c:	68 b2 00 00 00       	push   $0xb2
80100c21:	e9 fa f8 ff ff       	jmp    80100520 <vector_handler>

80100c26 <vector179>:
80100c26:	6a 00                	push   $0x0
80100c28:	68 b3 00 00 00       	push   $0xb3
80100c2d:	e9 ee f8 ff ff       	jmp    80100520 <vector_handler>

80100c32 <vector180>:
80100c32:	6a 00                	push   $0x0
80100c34:	68 b4 00 00 00       	push   $0xb4
80100c39:	e9 e2 f8 ff ff       	jmp    80100520 <vector_handler>

80100c3e <vector181>:
80100c3e:	6a 00                	push   $0x0
80100c40:	68 b5 00 00 00       	push   $0xb5
80100c45:	e9 d6 f8 ff ff       	jmp    80100520 <vector_handler>

80100c4a <vector182>:
80100c4a:	6a 00                	push   $0x0
80100c4c:	68 b6 00 00 00       	push   $0xb6
80100c51:	e9 ca f8 ff ff       	jmp    80100520 <vector_handler>

80100c56 <vector183>:
80100c56:	6a 00                	push   $0x0
80100c58:	68 b7 00 00 00       	push   $0xb7
80100c5d:	e9 be f8 ff ff       	jmp    80100520 <vector_handler>

80100c62 <vector184>:
80100c62:	6a 00                	push   $0x0
80100c64:	68 b8 00 00 00       	push   $0xb8
80100c69:	e9 b2 f8 ff ff       	jmp    80100520 <vector_handler>

80100c6e <vector185>:
80100c6e:	6a 00                	push   $0x0
80100c70:	68 b9 00 00 00       	push   $0xb9
80100c75:	e9 a6 f8 ff ff       	jmp    80100520 <vector_handler>

80100c7a <vector186>:
80100c7a:	6a 00                	push   $0x0
80100c7c:	68 ba 00 00 00       	push   $0xba
80100c81:	e9 9a f8 ff ff       	jmp    80100520 <vector_handler>

80100c86 <vector187>:
80100c86:	6a 00                	push   $0x0
80100c88:	68 bb 00 00 00       	push   $0xbb
80100c8d:	e9 8e f8 ff ff       	jmp    80100520 <vector_handler>

80100c92 <vector188>:
80100c92:	6a 00                	push   $0x0
80100c94:	68 bc 00 00 00       	push   $0xbc
80100c99:	e9 82 f8 ff ff       	jmp    80100520 <vector_handler>

80100c9e <vector189>:
80100c9e:	6a 00                	push   $0x0
80100ca0:	68 bd 00 00 00       	push   $0xbd
80100ca5:	e9 76 f8 ff ff       	jmp    80100520 <vector_handler>

80100caa <vector190>:
80100caa:	6a 00                	push   $0x0
80100cac:	68 be 00 00 00       	push   $0xbe
80100cb1:	e9 6a f8 ff ff       	jmp    80100520 <vector_handler>

80100cb6 <vector191>:
80100cb6:	6a 00                	push   $0x0
80100cb8:	68 bf 00 00 00       	push   $0xbf
80100cbd:	e9 5e f8 ff ff       	jmp    80100520 <vector_handler>

80100cc2 <vector192>:
80100cc2:	6a 00                	push   $0x0
80100cc4:	68 c0 00 00 00       	push   $0xc0
80100cc9:	e9 52 f8 ff ff       	jmp    80100520 <vector_handler>

80100cce <vector193>:
80100cce:	6a 00                	push   $0x0
80100cd0:	68 c1 00 00 00       	push   $0xc1
80100cd5:	e9 46 f8 ff ff       	jmp    80100520 <vector_handler>

80100cda <vector194>:
80100cda:	6a 00                	push   $0x0
80100cdc:	68 c2 00 00 00       	push   $0xc2
80100ce1:	e9 3a f8 ff ff       	jmp    80100520 <vector_handler>

80100ce6 <vector195>:
80100ce6:	6a 00                	push   $0x0
80100ce8:	68 c3 00 00 00       	push   $0xc3
80100ced:	e9 2e f8 ff ff       	jmp    80100520 <vector_handler>

80100cf2 <vector196>:
80100cf2:	6a 00                	push   $0x0
80100cf4:	68 c4 00 00 00       	push   $0xc4
80100cf9:	e9 22 f8 ff ff       	jmp    80100520 <vector_handler>

80100cfe <vector197>:
80100cfe:	6a 00                	push   $0x0
80100d00:	68 c5 00 00 00       	push   $0xc5
80100d05:	e9 16 f8 ff ff       	jmp    80100520 <vector_handler>

80100d0a <vector198>:
80100d0a:	6a 00                	push   $0x0
80100d0c:	68 c6 00 00 00       	push   $0xc6
80100d11:	e9 0a f8 ff ff       	jmp    80100520 <vector_handler>

80100d16 <vector199>:
80100d16:	6a 00                	push   $0x0
80100d18:	68 c7 00 00 00       	push   $0xc7
80100d1d:	e9 fe f7 ff ff       	jmp    80100520 <vector_handler>

80100d22 <vector200>:
80100d22:	6a 00                	push   $0x0
80100d24:	68 c8 00 00 00       	push   $0xc8
80100d29:	e9 f2 f7 ff ff       	jmp    80100520 <vector_handler>

80100d2e <vector201>:
80100d2e:	6a 00                	push   $0x0
80100d30:	68 c9 00 00 00       	push   $0xc9
80100d35:	e9 e6 f7 ff ff       	jmp    80100520 <vector_handler>

80100d3a <vector202>:
80100d3a:	6a 00                	push   $0x0
80100d3c:	68 ca 00 00 00       	push   $0xca
80100d41:	e9 da f7 ff ff       	jmp    80100520 <vector_handler>

80100d46 <vector203>:
80100d46:	6a 00                	push   $0x0
80100d48:	68 cb 00 00 00       	push   $0xcb
80100d4d:	e9 ce f7 ff ff       	jmp    80100520 <vector_handler>

80100d52 <vector204>:
80100d52:	6a 00                	push   $0x0
80100d54:	68 cc 00 00 00       	push   $0xcc
80100d59:	e9 c2 f7 ff ff       	jmp    80100520 <vector_handler>

80100d5e <vector205>:
80100d5e:	6a 00                	push   $0x0
80100d60:	68 cd 00 00 00       	push   $0xcd
80100d65:	e9 b6 f7 ff ff       	jmp    80100520 <vector_handler>

80100d6a <vector206>:
80100d6a:	6a 00                	push   $0x0
80100d6c:	68 ce 00 00 00       	push   $0xce
80100d71:	e9 aa f7 ff ff       	jmp    80100520 <vector_handler>

80100d76 <vector207>:
80100d76:	6a 00                	push   $0x0
80100d78:	68 cf 00 00 00       	push   $0xcf
80100d7d:	e9 9e f7 ff ff       	jmp    80100520 <vector_handler>

80100d82 <vector208>:
80100d82:	6a 00                	push   $0x0
80100d84:	68 d0 00 00 00       	push   $0xd0
80100d89:	e9 92 f7 ff ff       	jmp    80100520 <vector_handler>

80100d8e <vector209>:
80100d8e:	6a 00                	push   $0x0
80100d90:	68 d1 00 00 00       	push   $0xd1
80100d95:	e9 86 f7 ff ff       	jmp    80100520 <vector_handler>

80100d9a <vector210>:
80100d9a:	6a 00                	push   $0x0
80100d9c:	68 d2 00 00 00       	push   $0xd2
80100da1:	e9 7a f7 ff ff       	jmp    80100520 <vector_handler>

80100da6 <vector211>:
80100da6:	6a 00                	push   $0x0
80100da8:	68 d3 00 00 00       	push   $0xd3
80100dad:	e9 6e f7 ff ff       	jmp    80100520 <vector_handler>

80100db2 <vector212>:
80100db2:	6a 00                	push   $0x0
80100db4:	68 d4 00 00 00       	push   $0xd4
80100db9:	e9 62 f7 ff ff       	jmp    80100520 <vector_handler>

80100dbe <vector213>:
80100dbe:	6a 00                	push   $0x0
80100dc0:	68 d5 00 00 00       	push   $0xd5
80100dc5:	e9 56 f7 ff ff       	jmp    80100520 <vector_handler>

80100dca <vector214>:
80100dca:	6a 00                	push   $0x0
80100dcc:	68 d6 00 00 00       	push   $0xd6
80100dd1:	e9 4a f7 ff ff       	jmp    80100520 <vector_handler>

80100dd6 <vector215>:
80100dd6:	6a 00                	push   $0x0
80100dd8:	68 d7 00 00 00       	push   $0xd7
80100ddd:	e9 3e f7 ff ff       	jmp    80100520 <vector_handler>

80100de2 <vector216>:
80100de2:	6a 00                	push   $0x0
80100de4:	68 d8 00 00 00       	push   $0xd8
80100de9:	e9 32 f7 ff ff       	jmp    80100520 <vector_handler>

80100dee <vector217>:
80100dee:	6a 00                	push   $0x0
80100df0:	68 d9 00 00 00       	push   $0xd9
80100df5:	e9 26 f7 ff ff       	jmp    80100520 <vector_handler>

80100dfa <vector218>:
80100dfa:	6a 00                	push   $0x0
80100dfc:	68 da 00 00 00       	push   $0xda
80100e01:	e9 1a f7 ff ff       	jmp    80100520 <vector_handler>

80100e06 <vector219>:
80100e06:	6a 00                	push   $0x0
80100e08:	68 db 00 00 00       	push   $0xdb
80100e0d:	e9 0e f7 ff ff       	jmp    80100520 <vector_handler>

80100e12 <vector220>:
80100e12:	6a 00                	push   $0x0
80100e14:	68 dc 00 00 00       	push   $0xdc
80100e19:	e9 02 f7 ff ff       	jmp    80100520 <vector_handler>

80100e1e <vector221>:
80100e1e:	6a 00                	push   $0x0
80100e20:	68 dd 00 00 00       	push   $0xdd
80100e25:	e9 f6 f6 ff ff       	jmp    80100520 <vector_handler>

80100e2a <vector222>:
80100e2a:	6a 00                	push   $0x0
80100e2c:	68 de 00 00 00       	push   $0xde
80100e31:	e9 ea f6 ff ff       	jmp    80100520 <vector_handler>

80100e36 <vector223>:
80100e36:	6a 00                	push   $0x0
80100e38:	68 df 00 00 00       	push   $0xdf
80100e3d:	e9 de f6 ff ff       	jmp    80100520 <vector_handler>

80100e42 <vector224>:
80100e42:	6a 00                	push   $0x0
80100e44:	68 e0 00 00 00       	push   $0xe0
80100e49:	e9 d2 f6 ff ff       	jmp    80100520 <vector_handler>

80100e4e <vector225>:
80100e4e:	6a 00                	push   $0x0
80100e50:	68 e1 00 00 00       	push   $0xe1
80100e55:	e9 c6 f6 ff ff       	jmp    80100520 <vector_handler>

80100e5a <vector226>:
80100e5a:	6a 00                	push   $0x0
80100e5c:	68 e2 00 00 00       	push   $0xe2
80100e61:	e9 ba f6 ff ff       	jmp    80100520 <vector_handler>

80100e66 <vector227>:
80100e66:	6a 00                	push   $0x0
80100e68:	68 e3 00 00 00       	push   $0xe3
80100e6d:	e9 ae f6 ff ff       	jmp    80100520 <vector_handler>

80100e72 <vector228>:
80100e72:	6a 00                	push   $0x0
80100e74:	68 e4 00 00 00       	push   $0xe4
80100e79:	e9 a2 f6 ff ff       	jmp    80100520 <vector_handler>

80100e7e <vector229>:
80100e7e:	6a 00                	push   $0x0
80100e80:	68 e5 00 00 00       	push   $0xe5
80100e85:	e9 96 f6 ff ff       	jmp    80100520 <vector_handler>

80100e8a <vector230>:
80100e8a:	6a 00                	push   $0x0
80100e8c:	68 e6 00 00 00       	push   $0xe6
80100e91:	e9 8a f6 ff ff       	jmp    80100520 <vector_handler>

80100e96 <vector231>:
80100e96:	6a 00                	push   $0x0
80100e98:	68 e7 00 00 00       	push   $0xe7
80100e9d:	e9 7e f6 ff ff       	jmp    80100520 <vector_handler>

80100ea2 <vector232>:
80100ea2:	6a 00                	push   $0x0
80100ea4:	68 e8 00 00 00       	push   $0xe8
80100ea9:	e9 72 f6 ff ff       	jmp    80100520 <vector_handler>

80100eae <vector233>:
80100eae:	6a 00                	push   $0x0
80100eb0:	68 e9 00 00 00       	push   $0xe9
80100eb5:	e9 66 f6 ff ff       	jmp    80100520 <vector_handler>

80100eba <vector234>:
80100eba:	6a 00                	push   $0x0
80100ebc:	68 ea 00 00 00       	push   $0xea
80100ec1:	e9 5a f6 ff ff       	jmp    80100520 <vector_handler>

80100ec6 <vector235>:
80100ec6:	6a 00                	push   $0x0
80100ec8:	68 eb 00 00 00       	push   $0xeb
80100ecd:	e9 4e f6 ff ff       	jmp    80100520 <vector_handler>

80100ed2 <vector236>:
80100ed2:	6a 00                	push   $0x0
80100ed4:	68 ec 00 00 00       	push   $0xec
80100ed9:	e9 42 f6 ff ff       	jmp    80100520 <vector_handler>

80100ede <vector237>:
80100ede:	6a 00                	push   $0x0
80100ee0:	68 ed 00 00 00       	push   $0xed
80100ee5:	e9 36 f6 ff ff       	jmp    80100520 <vector_handler>

80100eea <vector238>:
80100eea:	6a 00                	push   $0x0
80100eec:	68 ee 00 00 00       	push   $0xee
80100ef1:	e9 2a f6 ff ff       	jmp    80100520 <vector_handler>

80100ef6 <vector239>:
80100ef6:	6a 00                	push   $0x0
80100ef8:	68 ef 00 00 00       	push   $0xef
80100efd:	e9 1e f6 ff ff       	jmp    80100520 <vector_handler>

80100f02 <vector240>:
80100f02:	6a 00                	push   $0x0
80100f04:	68 f0 00 00 00       	push   $0xf0
80100f09:	e9 12 f6 ff ff       	jmp    80100520 <vector_handler>

80100f0e <vector241>:
80100f0e:	6a 00                	push   $0x0
80100f10:	68 f1 00 00 00       	push   $0xf1
80100f15:	e9 06 f6 ff ff       	jmp    80100520 <vector_handler>

80100f1a <vector242>:
80100f1a:	6a 00                	push   $0x0
80100f1c:	68 f2 00 00 00       	push   $0xf2
80100f21:	e9 fa f5 ff ff       	jmp    80100520 <vector_handler>

80100f26 <vector243>:
80100f26:	6a 00                	push   $0x0
80100f28:	68 f3 00 00 00       	push   $0xf3
80100f2d:	e9 ee f5 ff ff       	jmp    80100520 <vector_handler>

80100f32 <vector244>:
80100f32:	6a 00                	push   $0x0
80100f34:	68 f4 00 00 00       	push   $0xf4
80100f39:	e9 e2 f5 ff ff       	jmp    80100520 <vector_handler>

80100f3e <vector245>:
80100f3e:	6a 00                	push   $0x0
80100f40:	68 f5 00 00 00       	push   $0xf5
80100f45:	e9 d6 f5 ff ff       	jmp    80100520 <vector_handler>

80100f4a <vector246>:
80100f4a:	6a 00                	push   $0x0
80100f4c:	68 f6 00 00 00       	push   $0xf6
80100f51:	e9 ca f5 ff ff       	jmp    80100520 <vector_handler>

80100f56 <vector247>:
80100f56:	6a 00                	push   $0x0
80100f58:	68 f7 00 00 00       	push   $0xf7
80100f5d:	e9 be f5 ff ff       	jmp    80100520 <vector_handler>

80100f62 <vector248>:
80100f62:	6a 00                	push   $0x0
80100f64:	68 f8 00 00 00       	push   $0xf8
80100f69:	e9 b2 f5 ff ff       	jmp    80100520 <vector_handler>

80100f6e <vector249>:
80100f6e:	6a 00                	push   $0x0
80100f70:	68 f9 00 00 00       	push   $0xf9
80100f75:	e9 a6 f5 ff ff       	jmp    80100520 <vector_handler>

80100f7a <vector250>:
80100f7a:	6a 00                	push   $0x0
80100f7c:	68 fa 00 00 00       	push   $0xfa
80100f81:	e9 9a f5 ff ff       	jmp    80100520 <vector_handler>

80100f86 <vector251>:
80100f86:	6a 00                	push   $0x0
80100f88:	68 fb 00 00 00       	push   $0xfb
80100f8d:	e9 8e f5 ff ff       	jmp    80100520 <vector_handler>

80100f92 <vector252>:
80100f92:	6a 00                	push   $0x0
80100f94:	68 fc 00 00 00       	push   $0xfc
80100f99:	e9 82 f5 ff ff       	jmp    80100520 <vector_handler>

80100f9e <vector253>:
80100f9e:	6a 00                	push   $0x0
80100fa0:	68 fd 00 00 00       	push   $0xfd
80100fa5:	e9 76 f5 ff ff       	jmp    80100520 <vector_handler>

80100faa <vector254>:
80100faa:	6a 00                	push   $0x0
80100fac:	68 fe 00 00 00       	push   $0xfe
80100fb1:	e9 6a f5 ff ff       	jmp    80100520 <vector_handler>

80100fb6 <vector255>:
80100fb6:	6a 00                	push   $0x0
80100fb8:	68 ff 00 00 00       	push   $0xff
80100fbd:	e9 5e f5 ff ff       	jmp    80100520 <vector_handler>

80100fc2 <itoa>:
80100fc2:	55                   	push   %ebp
80100fc3:	57                   	push   %edi
80100fc4:	56                   	push   %esi
80100fc5:	53                   	push   %ebx
80100fc6:	8b 54 24 14          	mov    0x14(%esp),%edx
80100fca:	8b 6c 24 18          	mov    0x18(%esp),%ebp
80100fce:	8b 7c 24 1c          	mov    0x1c(%esp),%edi
80100fd2:	8d 47 fe             	lea    -0x2(%edi),%eax
80100fd5:	83 f8 22             	cmp    $0x22,%eax
80100fd8:	77 0a                	ja     80100fe4 <itoa+0x22>
80100fda:	85 d2                	test   %edx,%edx
80100fdc:	78 11                	js     80100fef <itoa+0x2d>
80100fde:	89 eb                	mov    %ebp,%ebx
80100fe0:	89 d9                	mov    %ebx,%ecx
80100fe2:	eb 1f                	jmp    80101003 <itoa+0x41>
80100fe4:	c6 45 00 00          	movb   $0x0,0x0(%ebp)
80100fe8:	89 e8                	mov    %ebp,%eax
80100fea:	5b                   	pop    %ebx
80100feb:	5e                   	pop    %esi
80100fec:	5f                   	pop    %edi
80100fed:	5d                   	pop    %ebp
80100fee:	c3                   	ret    
80100fef:	83 ff 0a             	cmp    $0xa,%edi
80100ff2:	74 04                	je     80100ff8 <itoa+0x36>
80100ff4:	89 eb                	mov    %ebp,%ebx
80100ff6:	eb e8                	jmp    80100fe0 <itoa+0x1e>
80100ff8:	8d 5d 01             	lea    0x1(%ebp),%ebx
80100ffb:	c6 45 00 2d          	movb   $0x2d,0x0(%ebp)
80100fff:	eb df                	jmp    80100fe0 <itoa+0x1e>
80101001:	89 f1                	mov    %esi,%ecx
80101003:	8d 71 01             	lea    0x1(%ecx),%esi
80101006:	89 d0                	mov    %edx,%eax
80101008:	99                   	cltd   
80101009:	f7 ff                	idiv   %edi
8010100b:	8a 92 83 12 10 80    	mov    -0x7fefed7d(%edx),%dl
80101011:	88 56 ff             	mov    %dl,-0x1(%esi)
80101014:	89 c2                	mov    %eax,%edx
80101016:	85 c0                	test   %eax,%eax
80101018:	75 e7                	jne    80101001 <itoa+0x3f>
8010101a:	c6 41 01 00          	movb   $0x0,0x1(%ecx)
8010101e:	39 cb                	cmp    %ecx,%ebx
80101020:	73 c6                	jae    80100fe8 <itoa+0x26>
80101022:	8a 03                	mov    (%ebx),%al
80101024:	43                   	inc    %ebx
80101025:	8a 11                	mov    (%ecx),%dl
80101027:	88 53 ff             	mov    %dl,-0x1(%ebx)
8010102a:	49                   	dec    %ecx
8010102b:	88 41 01             	mov    %al,0x1(%ecx)
8010102e:	39 cb                	cmp    %ecx,%ebx
80101030:	72 f0                	jb     80101022 <itoa+0x60>
80101032:	eb b4                	jmp    80100fe8 <itoa+0x26>

80101034 <write_string>:
80101034:	55                   	push   %ebp
80101035:	57                   	push   %edi
80101036:	56                   	push   %esi
80101037:	53                   	push   %ebx
80101038:	8b 5c 24 14          	mov    0x14(%esp),%ebx
8010103c:	8b 4c 24 18          	mov    0x18(%esp),%ecx
80101040:	8a 01                	mov    (%ecx),%al
80101042:	41                   	inc    %ecx
80101043:	be 67 66 66 66       	mov    $0x66666667,%esi
80101048:	eb 2d                	jmp    80101077 <write_string+0x43>
8010104a:	a1 00 34 10 80       	mov    0x80103400,%eax
8010104f:	8d a8 00 80 f4 ff    	lea    -0xb8000(%eax),%ebp
80101055:	89 e8                	mov    %ebp,%eax
80101057:	f7 ee                	imul   %esi
80101059:	89 d0                	mov    %edx,%eax
8010105b:	c1 f8 06             	sar    $0x6,%eax
8010105e:	c1 fd 1f             	sar    $0x1f,%ebp
80101061:	29 e8                	sub    %ebp,%eax
80101063:	8d 44 80 05          	lea    0x5(%eax,%eax,4),%eax
80101067:	c1 e0 05             	shl    $0x5,%eax
8010106a:	05 00 80 0b 00       	add    $0xb8000,%eax
8010106f:	a3 00 34 10 80       	mov    %eax,0x80103400
80101074:	8a 07                	mov    (%edi),%al
80101076:	41                   	inc    %ecx
80101077:	89 cf                	mov    %ecx,%edi
80101079:	80 79 ff 00          	cmpb   $0x0,-0x1(%ecx)
8010107d:	74 2a                	je     801010a9 <write_string+0x75>
8010107f:	3c 0a                	cmp    $0xa,%al
80101081:	74 c7                	je     8010104a <write_string+0x16>
80101083:	8b 15 00 34 10 80    	mov    0x80103400,%edx
80101089:	8d 6a 01             	lea    0x1(%edx),%ebp
8010108c:	89 2d 00 34 10 80    	mov    %ebp,0x80103400
80101092:	88 02                	mov    %al,(%edx)
80101094:	8a 07                	mov    (%edi),%al
80101096:	8b 15 00 34 10 80    	mov    0x80103400,%edx
8010109c:	8d 7a 01             	lea    0x1(%edx),%edi
8010109f:	89 3d 00 34 10 80    	mov    %edi,0x80103400
801010a5:	88 1a                	mov    %bl,(%edx)
801010a7:	eb cd                	jmp    80101076 <write_string+0x42>
801010a9:	5b                   	pop    %ebx
801010aa:	5e                   	pop    %esi
801010ab:	5f                   	pop    %edi
801010ac:	5d                   	pop    %ebp
801010ad:	c3                   	ret    

801010ae <clr_scr>:
801010ae:	c7 05 00 34 10 80 00 	movl   $0xb8000,0x80103400
801010b5:	80 0b 00 
801010b8:	b8 00 80 0b 00       	mov    $0xb8000,%eax
801010bd:	eb 02                	jmp    801010c1 <clr_scr+0x13>
801010bf:	89 d0                	mov    %edx,%eax
801010c1:	c6 00 00             	movb   $0x0,(%eax)
801010c4:	8d 50 02             	lea    0x2(%eax),%edx
801010c7:	c6 40 01 00          	movb   $0x0,0x1(%eax)
801010cb:	3d 9e 8f 0b 00       	cmp    $0xb8f9e,%eax
801010d0:	75 ed                	jne    801010bf <clr_scr+0x11>
801010d2:	c3                   	ret    

801010d3 <memcpy>:
801010d3:	56                   	push   %esi
801010d4:	53                   	push   %ebx
801010d5:	8b 44 24 0c          	mov    0xc(%esp),%eax
801010d9:	8b 5c 24 10          	mov    0x10(%esp),%ebx
801010dd:	8b 4c 24 14          	mov    0x14(%esp),%ecx
801010e1:	39 c3                	cmp    %eax,%ebx
801010e3:	72 19                	jb     801010fe <memcpy+0x2b>
801010e5:	85 c9                	test   %ecx,%ecx
801010e7:	7e 12                	jle    801010fb <memcpy+0x28>
801010e9:	89 ce                	mov    %ecx,%esi
801010eb:	ba 00 00 00 00       	mov    $0x0,%edx
801010f0:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
801010f3:	88 0c 10             	mov    %cl,(%eax,%edx,1)
801010f6:	42                   	inc    %edx
801010f7:	39 d6                	cmp    %edx,%esi
801010f9:	75 f5                	jne    801010f0 <memcpy+0x1d>
801010fb:	5b                   	pop    %ebx
801010fc:	5e                   	pop    %esi
801010fd:	c3                   	ret    
801010fe:	8d 14 0b             	lea    (%ebx,%ecx,1),%edx
80101101:	39 d0                	cmp    %edx,%eax
80101103:	73 e0                	jae    801010e5 <memcpy+0x12>
80101105:	8d 51 ff             	lea    -0x1(%ecx),%edx
80101108:	85 c9                	test   %ecx,%ecx
8010110a:	7e ef                	jle    801010fb <memcpy+0x28>
8010110c:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
8010110f:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101112:	4a                   	dec    %edx
80101113:	83 fa ff             	cmp    $0xffffffff,%edx
80101116:	75 f4                	jne    8010110c <memcpy+0x39>
80101118:	eb e1                	jmp    801010fb <memcpy+0x28>

8010111a <memset>:
8010111a:	57                   	push   %edi
8010111b:	8b 54 24 08          	mov    0x8(%esp),%edx
8010111f:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101123:	8b 4c 24 10          	mov    0x10(%esp),%ecx
80101127:	89 d7                	mov    %edx,%edi
80101129:	09 cf                	or     %ecx,%edi
8010112b:	f7 c7 03 00 00 00    	test   $0x3,%edi
80101131:	74 09                	je     8010113c <memset+0x22>
80101133:	89 d7                	mov    %edx,%edi
80101135:	fc                   	cld    
80101136:	f3 aa                	rep stos %al,%es:(%edi)
80101138:	89 d0                	mov    %edx,%eax
8010113a:	5f                   	pop    %edi
8010113b:	c3                   	ret    
8010113c:	89 cf                	mov    %ecx,%edi
8010113e:	85 c9                	test   %ecx,%ecx
80101140:	78 0c                	js     8010114e <memset+0x34>
80101142:	89 f9                	mov    %edi,%ecx
80101144:	c1 f9 02             	sar    $0x2,%ecx
80101147:	89 d7                	mov    %edx,%edi
80101149:	fc                   	cld    
8010114a:	f3 ab                	rep stos %eax,%es:(%edi)
8010114c:	eb ea                	jmp    80101138 <memset+0x1e>
8010114e:	8d 79 03             	lea    0x3(%ecx),%edi
80101151:	eb ef                	jmp    80101142 <memset+0x28>

80101153 <strlen>:
80101153:	8b 54 24 04          	mov    0x4(%esp),%edx
80101157:	80 3a 00             	cmpb   $0x0,(%edx)
8010115a:	74 0d                	je     80101169 <strlen+0x16>
8010115c:	b8 00 00 00 00       	mov    $0x0,%eax
80101161:	40                   	inc    %eax
80101162:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
80101166:	75 f9                	jne    80101161 <strlen+0xe>
80101168:	c3                   	ret    
80101169:	b8 00 00 00 00       	mov    $0x0,%eax
8010116e:	c3                   	ret    

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
8010001c:	bc b0 53 10 80       	mov    $0x801053b0,%esp
80100021:	b8 be 00 10 80       	mov    $0x801000be,%eax
80100026:	ff e0                	jmp    *%eax

80100028 <interrupt_handler>:
80100028:	eb fe                	jmp    80100028 <interrupt_handler>

8010002a <init_8259A>:
8010002a:	53                   	push   %ebx
8010002b:	b0 11                	mov    $0x11,%al
8010002d:	ba 20 00 00 00       	mov    $0x20,%edx
80100032:	ee                   	out    %al,(%dx)
80100033:	ba a0 00 00 00       	mov    $0xa0,%edx
80100038:	ee                   	out    %al,(%dx)
80100039:	bb 21 00 00 00       	mov    $0x21,%ebx
8010003e:	b0 20                	mov    $0x20,%al
80100040:	89 da                	mov    %ebx,%edx
80100042:	ee                   	out    %al,(%dx)
80100043:	b9 a1 00 00 00       	mov    $0xa1,%ecx
80100048:	b0 28                	mov    $0x28,%al
8010004a:	89 ca                	mov    %ecx,%edx
8010004c:	ee                   	out    %al,(%dx)
8010004d:	b0 04                	mov    $0x4,%al
8010004f:	89 da                	mov    %ebx,%edx
80100051:	ee                   	out    %al,(%dx)
80100052:	b0 02                	mov    $0x2,%al
80100054:	89 ca                	mov    %ecx,%edx
80100056:	ee                   	out    %al,(%dx)
80100057:	b0 01                	mov    $0x1,%al
80100059:	89 da                	mov    %ebx,%edx
8010005b:	ee                   	out    %al,(%dx)
8010005c:	89 ca                	mov    %ecx,%edx
8010005e:	ee                   	out    %al,(%dx)
8010005f:	b0 ff                	mov    $0xff,%al
80100061:	89 da                	mov    %ebx,%edx
80100063:	ee                   	out    %al,(%dx)
80100064:	89 ca                	mov    %ecx,%edx
80100066:	ee                   	out    %al,(%dx)
80100067:	5b                   	pop    %ebx
80100068:	c3                   	ret    

80100069 <make_desc>:
80100069:	57                   	push   %edi
8010006a:	56                   	push   %esi
8010006b:	53                   	push   %ebx
8010006c:	8b 5c 24 14          	mov    0x14(%esp),%ebx
80100070:	8b 7c 24 18          	mov    0x18(%esp),%edi
80100074:	8b 54 24 1c          	mov    0x1c(%esp),%edx
80100078:	8b 74 24 10          	mov    0x10(%esp),%esi
8010007c:	c1 e6 06             	shl    $0x6,%esi
8010007f:	8d 8e 00 00 20 80    	lea    -0x7fe00000(%esi),%ecx
80100085:	66 89 be 00 00 20 80 	mov    %di,-0x7fe00000(%esi)
8010008c:	66 89 9e 02 00 20 80 	mov    %bx,-0x7fdffffe(%esi)
80100093:	89 de                	mov    %ebx,%esi
80100095:	c1 ee 10             	shr    $0x10,%esi
80100098:	89 f0                	mov    %esi,%eax
8010009a:	88 41 04             	mov    %al,0x4(%ecx)
8010009d:	c1 eb 18             	shr    $0x18,%ebx
801000a0:	88 59 07             	mov    %bl,0x7(%ecx)
801000a3:	88 51 05             	mov    %dl,0x5(%ecx)
801000a6:	89 f8                	mov    %edi,%eax
801000a8:	c1 e8 10             	shr    $0x10,%eax
801000ab:	83 e0 0f             	and    $0xf,%eax
801000ae:	66 c1 ea 08          	shr    $0x8,%dx
801000b2:	83 e2 f0             	and    $0xfffffff0,%edx
801000b5:	09 d0                	or     %edx,%eax
801000b7:	88 41 06             	mov    %al,0x6(%ecx)
801000ba:	5b                   	pop    %ebx
801000bb:	5e                   	pop    %esi
801000bc:	5f                   	pop    %edi
801000bd:	c3                   	ret    

801000be <main>:
801000be:	8d 4c 24 04          	lea    0x4(%esp),%ecx
801000c2:	83 e4 f0             	and    $0xfffffff0,%esp
801000c5:	ff 71 fc             	pushl  -0x4(%ecx)
801000c8:	55                   	push   %ebp
801000c9:	89 e5                	mov    %esp,%ebp
801000cb:	56                   	push   %esi
801000cc:	53                   	push   %ebx
801000cd:	51                   	push   %ecx
801000ce:	83 ec 3c             	sub    $0x3c,%esp
801000d1:	e8 18 10 00 00       	call   801010ee <clr_scr>
801000d6:	6a 00                	push   $0x0
801000d8:	6a 00                	push   $0x0
801000da:	6a 00                	push   $0x0
801000dc:	6a 00                	push   $0x0
801000de:	e8 86 ff ff ff       	call   80100069 <make_desc>
801000e3:	68 98 c0 00 00       	push   $0xc098
801000e8:	68 00 00 f0 80       	push   $0x80f00000
801000ed:	6a 00                	push   $0x0
801000ef:	6a 01                	push   $0x1
801000f1:	e8 73 ff ff ff       	call   80100069 <make_desc>
801000f6:	83 c4 20             	add    $0x20,%esp
801000f9:	68 92 c0 00 00       	push   $0xc092
801000fe:	68 00 00 f0 80       	push   $0x80f00000
80100103:	6a 00                	push   $0x0
80100105:	6a 02                	push   $0x2
80100107:	e8 5d ff ff ff       	call   80100069 <make_desc>
8010010c:	68 f2 00 00 00       	push   $0xf2
80100111:	68 ff ff 00 00       	push   $0xffff
80100116:	68 00 80 0b 00       	push   $0xb8000
8010011b:	6a 03                	push   $0x3
8010011d:	e8 47 ff ff ff       	call   80100069 <make_desc>
80100122:	66 c7 05 b8 53 10 80 	movw   $0x1f,0x801053b8
80100129:	1f 00 
8010012b:	c7 05 ba 53 10 80 00 	movl   $0x80200000,0x801053ba
80100132:	00 20 80 
80100135:	0f 01 15 b8 53 10 80 	lgdtl  0x801053b8
8010013c:	83 c4 1c             	add    $0x1c,%esp
8010013f:	6a 10                	push   $0x10
80100141:	8d 5d c8             	lea    -0x38(%ebp),%ebx
80100144:	53                   	push   %ebx
80100145:	ff 35 28 30 10 80    	pushl  0x80103028
8010014b:	e8 b2 0e 00 00       	call   80101002 <itoa>
80100150:	83 c4 08             	add    $0x8,%esp
80100153:	68 b0 11 10 80       	push   $0x801011b0
80100158:	6a 0f                	push   $0xf
8010015a:	e8 15 0f 00 00       	call   80101074 <write_string>
8010015f:	83 c4 08             	add    $0x8,%esp
80100162:	53                   	push   %ebx
80100163:	6a 0f                	push   $0xf
80100165:	e8 0a 0f 00 00       	call   80101074 <write_string>
8010016a:	83 c4 08             	add    $0x8,%esp
8010016d:	68 f1 11 10 80       	push   $0x801011f1
80100172:	6a 0f                	push   $0xf
80100174:	e8 fb 0e 00 00       	call   80101074 <write_string>
80100179:	b9 00 30 10 80       	mov    $0x80103000,%ecx
8010017e:	83 c4 10             	add    $0x10,%esp
80100181:	b8 00 04 20 80       	mov    $0x80200400,%eax
80100186:	8b 11                	mov    (%ecx),%edx
80100188:	66 89 10             	mov    %dx,(%eax)
8010018b:	c1 ea 10             	shr    $0x10,%edx
8010018e:	66 89 50 06          	mov    %dx,0x6(%eax)
80100192:	66 c7 40 02 08 00    	movw   $0x8,0x2(%eax)
80100198:	c6 40 04 00          	movb   $0x0,0x4(%eax)
8010019c:	c6 40 05 8e          	movb   $0x8e,0x5(%eax)
801001a0:	83 c1 04             	add    $0x4,%ecx
801001a3:	83 c0 08             	add    $0x8,%eax
801001a6:	3d 00 0c 20 80       	cmp    $0x80200c00,%eax
801001ab:	75 d9                	jne    80100186 <main+0xc8>
801001ad:	66 c7 05 b0 53 10 80 	movw   $0x7ff,0x801053b0
801001b4:	ff 07 
801001b6:	c7 05 b2 53 10 80 00 	movl   $0x80200400,0x801053b2
801001bd:	04 20 80 
801001c0:	0f 01 1d b0 53 10 80 	lidtl  0x801053b0
801001c7:	e8 5e fe ff ff       	call   8010002a <init_8259A>
801001cc:	83 ec 08             	sub    $0x8,%esp
801001cf:	68 ca 11 10 80       	push   $0x801011ca
801001d4:	6a 0f                	push   $0xf
801001d6:	e8 99 0e 00 00       	call   80101074 <write_string>
801001db:	83 c4 08             	add    $0x8,%esp
801001de:	68 d8 11 10 80       	push   $0x801011d8
801001e3:	6a 0f                	push   $0xf
801001e5:	e8 8a 0e 00 00       	call   80101074 <write_string>
801001ea:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
801001f0:	8d 04 12             	lea    (%edx,%edx,1),%eax
801001f3:	01 d0                	add    %edx,%eax
801001f5:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
801001fc:	83 c4 10             	add    $0x10,%esp
801001ff:	3d 00 80 00 00       	cmp    $0x8000,%eax
80100204:	0f 86 18 01 00 00    	jbe    80100322 <main+0x264>
8010020a:	bb 00 80 00 00       	mov    $0x8000,%ebx
8010020f:	8d 75 b8             	lea    -0x48(%ebp),%esi
80100212:	eb 2f                	jmp    80100243 <main+0x185>
80100214:	83 ec 08             	sub    $0x8,%esp
80100217:	68 13 12 10 80       	push   $0x80101213
8010021c:	6a 0f                	push   $0xf
8010021e:	e8 51 0e 00 00       	call   80101074 <write_string>
80100223:	83 c4 10             	add    $0x10,%esp
80100226:	83 c3 18             	add    $0x18,%ebx
80100229:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
8010022f:	8d 04 12             	lea    (%edx,%edx,1),%eax
80100232:	01 d0                	add    %edx,%eax
80100234:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
8010023b:	39 c3                	cmp    %eax,%ebx
8010023d:	0f 83 df 00 00 00    	jae    80100322 <main+0x264>
80100243:	83 ec 04             	sub    $0x4,%esp
80100246:	6a 10                	push   $0x10
80100248:	56                   	push   %esi
80100249:	8b 43 04             	mov    0x4(%ebx),%eax
8010024c:	c1 e0 10             	shl    $0x10,%eax
8010024f:	0b 03                	or     (%ebx),%eax
80100251:	50                   	push   %eax
80100252:	e8 ab 0d 00 00       	call   80101002 <itoa>
80100257:	83 c4 08             	add    $0x8,%esp
8010025a:	68 f3 11 10 80       	push   $0x801011f3
8010025f:	6a 0f                	push   $0xf
80100261:	e8 0e 0e 00 00       	call   80101074 <write_string>
80100266:	83 c4 08             	add    $0x8,%esp
80100269:	56                   	push   %esi
8010026a:	6a 0f                	push   $0xf
8010026c:	e8 03 0e 00 00       	call   80101074 <write_string>
80100271:	83 c4 08             	add    $0x8,%esp
80100274:	68 ff 11 10 80       	push   $0x801011ff
80100279:	6a 0f                	push   $0xf
8010027b:	e8 f4 0d 00 00       	call   80101074 <write_string>
80100280:	83 c4 0c             	add    $0xc,%esp
80100283:	6a 10                	push   $0x10
80100285:	56                   	push   %esi
80100286:	8b 43 0c             	mov    0xc(%ebx),%eax
80100289:	c1 e0 10             	shl    $0x10,%eax
8010028c:	0b 43 08             	or     0x8(%ebx),%eax
8010028f:	50                   	push   %eax
80100290:	e8 6d 0d 00 00       	call   80101002 <itoa>
80100295:	83 c4 08             	add    $0x8,%esp
80100298:	56                   	push   %esi
80100299:	6a 0f                	push   $0xf
8010029b:	e8 d4 0d 00 00       	call   80101074 <write_string>
801002a0:	83 c4 08             	add    $0x8,%esp
801002a3:	68 0a 12 10 80       	push   $0x8010120a
801002a8:	6a 0f                	push   $0xf
801002aa:	e8 c5 0d 00 00       	call   80101074 <write_string>
801002af:	83 c4 10             	add    $0x10,%esp
801002b2:	83 7b 10 05          	cmpl   $0x5,0x10(%ebx)
801002b6:	0f 87 6a ff ff ff    	ja     80100226 <main+0x168>
801002bc:	8b 43 10             	mov    0x10(%ebx),%eax
801002bf:	ff 24 85 44 12 10 80 	jmp    *-0x7fefedbc(,%eax,4)
801002c6:	83 ec 08             	sub    $0x8,%esp
801002c9:	68 1b 12 10 80       	push   $0x8010121b
801002ce:	6a 0f                	push   $0xf
801002d0:	e8 9f 0d 00 00       	call   80101074 <write_string>
801002d5:	83 c4 10             	add    $0x10,%esp
801002d8:	e9 49 ff ff ff       	jmp    80100226 <main+0x168>
801002dd:	83 ec 08             	sub    $0x8,%esp
801002e0:	68 25 12 10 80       	push   $0x80101225
801002e5:	6a 0f                	push   $0xf
801002e7:	e8 88 0d 00 00       	call   80101074 <write_string>
801002ec:	83 c4 10             	add    $0x10,%esp
801002ef:	e9 32 ff ff ff       	jmp    80100226 <main+0x168>
801002f4:	83 ec 08             	sub    $0x8,%esp
801002f7:	68 32 12 10 80       	push   $0x80101232
801002fc:	6a 0f                	push   $0xf
801002fe:	e8 71 0d 00 00       	call   80101074 <write_string>
80100303:	83 c4 10             	add    $0x10,%esp
80100306:	e9 1b ff ff ff       	jmp    80100226 <main+0x168>
8010030b:	83 ec 08             	sub    $0x8,%esp
8010030e:	68 3c 12 10 80       	push   $0x8010123c
80100313:	6a 0f                	push   $0xf
80100315:	e8 5a 0d 00 00       	call   80101074 <write_string>
8010031a:	83 c4 10             	add    $0x10,%esp
8010031d:	e9 04 ff ff ff       	jmp    80100226 <main+0x168>
80100322:	cd 10                	int    $0x10
80100324:	eb fe                	jmp    80100324 <main+0x266>

80100326 <found_tbl_by_va>:
80100326:	8b 44 24 04          	mov    0x4(%esp),%eax
8010032a:	c1 e8 16             	shr    $0x16,%eax
8010032d:	c1 e0 02             	shl    $0x2,%eax
80100330:	f6 80 00 0c 20 80 01 	testb  $0x1,-0x7fdff400(%eax)
80100337:	75 0a                	jne    80100343 <found_tbl_by_va+0x1d>
80100339:	c7 80 00 0c 20 80 00 	movl   $0x80201c00,-0x7fdff400(%eax)
80100340:	1c 20 80 
80100343:	8b 80 00 0c 20 80    	mov    -0x7fdff400(%eax),%eax
80100349:	c3                   	ret    

8010034a <vm_map>:
8010034a:	55                   	push   %ebp
8010034b:	57                   	push   %edi
8010034c:	56                   	push   %esi
8010034d:	53                   	push   %ebx
8010034e:	83 ec 1c             	sub    $0x1c,%esp
80100351:	8b 44 24 30          	mov    0x30(%esp),%eax
80100355:	89 c5                	mov    %eax,%ebp
80100357:	81 e5 00 f0 ff ff    	and    $0xfffff000,%ebp
8010035d:	8b 54 24 34          	mov    0x34(%esp),%edx
80100361:	8d 74 10 ff          	lea    -0x1(%eax,%edx,1),%esi
80100365:	81 e6 00 f0 ff ff    	and    $0xfffff000,%esi
8010036b:	55                   	push   %ebp
8010036c:	e8 b5 ff ff ff       	call   80100326 <found_tbl_by_va>
80100371:	6a 10                	push   $0x10
80100373:	8d 7c 24 08          	lea    0x8(%esp),%edi
80100377:	57                   	push   %edi
80100378:	50                   	push   %eax
80100379:	e8 84 0c 00 00       	call   80101002 <itoa>
8010037e:	83 c4 08             	add    $0x8,%esp
80100381:	57                   	push   %edi
80100382:	6a 0f                	push   $0xf
80100384:	e8 eb 0c 00 00       	call   80101074 <write_string>
80100389:	83 c4 08             	add    $0x8,%esp
8010038c:	68 f1 11 10 80       	push   $0x801011f1
80100391:	6a 0f                	push   $0xf
80100393:	e8 dc 0c 00 00       	call   80101074 <write_string>
80100398:	83 c4 10             	add    $0x10,%esp
8010039b:	39 f5                	cmp    %esi,%ebp
8010039d:	77 25                	ja     801003c4 <vm_map+0x7a>
8010039f:	89 eb                	mov    %ebp,%ebx
801003a1:	8b 7c 24 38          	mov    0x38(%esp),%edi
801003a5:	83 cf 07             	or     $0x7,%edi
801003a8:	29 ee                	sub    %ebp,%esi
801003aa:	83 ec 0c             	sub    $0xc,%esp
801003ad:	53                   	push   %ebx
801003ae:	e8 73 ff ff ff       	call   80100326 <found_tbl_by_va>
801003b3:	89 38                	mov    %edi,(%eax)
801003b5:	81 c3 00 40 00 00    	add    $0x4000,%ebx
801003bb:	83 c4 10             	add    $0x10,%esp
801003be:	89 d8                	mov    %ebx,%eax
801003c0:	01 f0                	add    %esi,%eax
801003c2:	73 e6                	jae    801003aa <vm_map+0x60>
801003c4:	83 c4 1c             	add    $0x1c,%esp
801003c7:	5b                   	pop    %ebx
801003c8:	5e                   	pop    %esi
801003c9:	5f                   	pop    %edi
801003ca:	5d                   	pop    %ebp
801003cb:	c3                   	ret    

801003cc <kinit_paging>:
801003cc:	57                   	push   %edi
801003cd:	56                   	push   %esi
801003ce:	53                   	push   %ebx
801003cf:	83 ec 10             	sub    $0x10,%esp
801003d2:	8b 54 24 20          	mov    0x20(%esp),%edx
801003d6:	8b 4c 24 24          	mov    0x24(%esp),%ecx
801003da:	8d 04 12             	lea    (%edx,%edx,1),%eax
801003dd:	01 d0                	add    %edx,%eax
801003df:	8d 34 c1             	lea    (%ecx,%eax,8),%esi
801003e2:	39 f1                	cmp    %esi,%ecx
801003e4:	73 2b                	jae    80100411 <kinit_paging+0x45>
801003e6:	bb 00 00 00 00       	mov    $0x0,%ebx
801003eb:	eb 07                	jmp    801003f4 <kinit_paging+0x28>
801003ed:	83 c1 18             	add    $0x18,%ecx
801003f0:	39 f1                	cmp    %esi,%ecx
801003f2:	73 22                	jae    80100416 <kinit_paging+0x4a>
801003f4:	8b 41 04             	mov    0x4(%ecx),%eax
801003f7:	c1 e0 10             	shl    $0x10,%eax
801003fa:	0b 01                	or     (%ecx),%eax
801003fc:	89 c2                	mov    %eax,%edx
801003fe:	8b 41 0c             	mov    0xc(%ecx),%eax
80100401:	c1 e0 10             	shl    $0x10,%eax
80100404:	0b 41 08             	or     0x8(%ecx),%eax
80100407:	01 d0                	add    %edx,%eax
80100409:	39 c3                	cmp    %eax,%ebx
8010040b:	73 e0                	jae    801003ed <kinit_paging+0x21>
8010040d:	89 c3                	mov    %eax,%ebx
8010040f:	eb dc                	jmp    801003ed <kinit_paging+0x21>
80100411:	bb 00 00 00 00       	mov    $0x0,%ebx
80100416:	83 ec 08             	sub    $0x8,%esp
80100419:	68 5c 12 10 80       	push   $0x8010125c
8010041e:	6a 0f                	push   $0xf
80100420:	e8 4f 0c 00 00       	call   80101074 <write_string>
80100425:	83 c4 0c             	add    $0xc,%esp
80100428:	6a 0a                	push   $0xa
8010042a:	8d 7c 24 08          	lea    0x8(%esp),%edi
8010042e:	57                   	push   %edi
8010042f:	89 d8                	mov    %ebx,%eax
80100431:	c1 e8 14             	shr    $0x14,%eax
80100434:	50                   	push   %eax
80100435:	e8 c8 0b 00 00       	call   80101002 <itoa>
8010043a:	83 c4 08             	add    $0x8,%esp
8010043d:	57                   	push   %edi
8010043e:	6a 0f                	push   $0xf
80100440:	e8 2f 0c 00 00       	call   80101074 <write_string>
80100445:	83 c4 08             	add    $0x8,%esp
80100448:	68 6c 12 10 80       	push   $0x8010126c
8010044d:	6a 0f                	push   $0xf
8010044f:	e8 20 0c 00 00       	call   80101074 <write_string>
80100454:	83 c4 0c             	add    $0xc,%esp
80100457:	f7 c3 ff ff 3f 00    	test   $0x3fffff,%ebx
8010045d:	0f 95 c0             	setne  %al
80100460:	0f b6 c0             	movzbl %al,%eax
80100463:	c1 eb 16             	shr    $0x16,%ebx
80100466:	01 c3                	add    %eax,%ebx
80100468:	89 de                	mov    %ebx,%esi
8010046a:	6a 0a                	push   $0xa
8010046c:	57                   	push   %edi
8010046d:	53                   	push   %ebx
8010046e:	e8 8f 0b 00 00       	call   80101002 <itoa>
80100473:	83 c4 08             	add    $0x8,%esp
80100476:	68 71 12 10 80       	push   $0x80101271
8010047b:	6a 0f                	push   $0xf
8010047d:	e8 f2 0b 00 00       	call   80101074 <write_string>
80100482:	83 c4 08             	add    $0x8,%esp
80100485:	57                   	push   %edi
80100486:	6a 0f                	push   $0xf
80100488:	e8 e7 0b 00 00       	call   80101074 <write_string>
8010048d:	83 c4 08             	add    $0x8,%esp
80100490:	68 7a 12 10 80       	push   $0x8010127a
80100495:	6a 0f                	push   $0xf
80100497:	e8 d8 0b 00 00       	call   80101074 <write_string>
8010049c:	83 c4 10             	add    $0x10,%esp
8010049f:	85 db                	test   %ebx,%ebx
801004a1:	74 43                	je     801004e6 <kinit_paging+0x11a>
801004a3:	b9 00 00 00 00       	mov    $0x0,%ecx
801004a8:	ba 07 1c 20 00       	mov    $0x201c07,%edx
801004ad:	b8 00 0c 20 80       	mov    $0x80200c00,%eax
801004b2:	89 10                	mov    %edx,(%eax)
801004b4:	81 c2 00 10 00 00    	add    $0x1000,%edx
801004ba:	41                   	inc    %ecx
801004bb:	8d 40 04             	lea    0x4(%eax),%eax
801004be:	39 ce                	cmp    %ecx,%esi
801004c0:	75 f0                	jne    801004b2 <kinit_paging+0xe6>
801004c2:	c1 e3 0a             	shl    $0xa,%ebx
801004c5:	74 1f                	je     801004e6 <kinit_paging+0x11a>
801004c7:	b9 00 00 00 00       	mov    $0x0,%ecx
801004cc:	ba 07 00 00 00       	mov    $0x7,%edx
801004d1:	b8 00 1c 20 80       	mov    $0x80201c00,%eax
801004d6:	89 10                	mov    %edx,(%eax)
801004d8:	81 c2 00 10 00 00    	add    $0x1000,%edx
801004de:	41                   	inc    %ecx
801004df:	8d 40 04             	lea    0x4(%eax),%eax
801004e2:	39 cb                	cmp    %ecx,%ebx
801004e4:	75 f0                	jne    801004d6 <kinit_paging+0x10a>
801004e6:	83 ec 08             	sub    $0x8,%esp
801004e9:	68 98 12 10 80       	push   $0x80101298
801004ee:	6a 0f                	push   $0xf
801004f0:	e8 7f 0b 00 00       	call   80101074 <write_string>
801004f5:	83 c4 0c             	add    $0xc,%esp
801004f8:	6a 00                	push   $0x0
801004fa:	68 00 00 00 0f       	push   $0xf000000
801004ff:	68 00 00 00 80       	push   $0x80000000
80100504:	e8 41 fe ff ff       	call   8010034a <vm_map>
80100509:	83 c4 08             	add    $0x8,%esp
8010050c:	68 ad 12 10 80       	push   $0x801012ad
80100511:	6a 0f                	push   $0xf
80100513:	e8 5c 0b 00 00       	call   80101074 <write_string>
80100518:	83 c4 0c             	add    $0xc,%esp
8010051b:	6a 10                	push   $0x10
8010051d:	8d 5c 24 08          	lea    0x8(%esp),%ebx
80100521:	53                   	push   %ebx
80100522:	68 00 0c 20 00       	push   $0x200c00
80100527:	e8 d6 0a 00 00       	call   80101002 <itoa>
8010052c:	83 c4 08             	add    $0x8,%esp
8010052f:	53                   	push   %ebx
80100530:	6a 0f                	push   $0xf
80100532:	e8 3d 0b 00 00       	call   80101074 <write_string>
80100537:	83 c4 08             	add    $0x8,%esp
8010053a:	68 f1 11 10 80       	push   $0x801011f1
8010053f:	6a 0f                	push   $0xf
80100541:	e8 2e 0b 00 00       	call   80101074 <write_string>
80100546:	b8 00 0c 20 00       	mov    $0x200c00,%eax
8010054b:	0f 22 d8             	mov    %eax,%cr3
8010054e:	83 c4 20             	add    $0x20,%esp
80100551:	5b                   	pop    %ebx
80100552:	5e                   	pop    %esi
80100553:	5f                   	pop    %edi
80100554:	c3                   	ret    

80100555 <kinit_gdt>:
80100555:	c3                   	ret    
80100556:	66 90                	xchg   %ax,%ax
80100558:	66 90                	xchg   %ax,%ax
8010055a:	66 90                	xchg   %ax,%ax
8010055c:	66 90                	xchg   %ax,%ax
8010055e:	66 90                	xchg   %ax,%ax

80100560 <vector_handler>:
80100560:	60                   	pusha  
80100561:	1e                   	push   %ds
80100562:	06                   	push   %es
80100563:	0f a0                	push   %fs
80100565:	0f a8                	push   %gs
80100567:	89 d6                	mov    %edx,%esi
80100569:	66 b8 10 00          	mov    $0x10,%ax
8010056d:	8e d8                	mov    %eax,%ds
8010056f:	8e c0                	mov    %eax,%es
80100571:	89 f2                	mov    %esi,%edx
80100573:	54                   	push   %esp
80100574:	e8 af fa ff ff       	call   80100028 <interrupt_handler>
80100579:	83 c4 04             	add    $0x4,%esp

8010057c <vector_handler.vector_handler_ret>:
8010057c:	0f a9                	pop    %gs
8010057e:	0f a1                	pop    %fs
80100580:	07                   	pop    %es
80100581:	1f                   	pop    %ds
80100582:	61                   	popa   
80100583:	83 c4 08             	add    $0x8,%esp
80100586:	cf                   	iret   
80100587:	66 90                	xchg   %ax,%ax
80100589:	66 90                	xchg   %ax,%ax
8010058b:	66 90                	xchg   %ax,%ax
8010058d:	66 90                	xchg   %ax,%ax
8010058f:	90                   	nop

80100590 <vector0>:
80100590:	6a 00                	push   $0x0
80100592:	6a 00                	push   $0x0
80100594:	e9 c7 ff ff ff       	jmp    80100560 <vector_handler>

80100599 <vector1>:
80100599:	6a 00                	push   $0x0
8010059b:	6a 01                	push   $0x1
8010059d:	e9 be ff ff ff       	jmp    80100560 <vector_handler>

801005a2 <vector2>:
801005a2:	6a 00                	push   $0x0
801005a4:	6a 02                	push   $0x2
801005a6:	e9 b5 ff ff ff       	jmp    80100560 <vector_handler>

801005ab <vector3>:
801005ab:	6a 00                	push   $0x0
801005ad:	6a 03                	push   $0x3
801005af:	e9 ac ff ff ff       	jmp    80100560 <vector_handler>

801005b4 <vector4>:
801005b4:	6a 00                	push   $0x0
801005b6:	6a 04                	push   $0x4
801005b8:	e9 a3 ff ff ff       	jmp    80100560 <vector_handler>

801005bd <vector5>:
801005bd:	6a 00                	push   $0x0
801005bf:	6a 05                	push   $0x5
801005c1:	e9 9a ff ff ff       	jmp    80100560 <vector_handler>

801005c6 <vector6>:
801005c6:	6a 00                	push   $0x0
801005c8:	6a 06                	push   $0x6
801005ca:	e9 91 ff ff ff       	jmp    80100560 <vector_handler>

801005cf <vector7>:
801005cf:	6a 00                	push   $0x0
801005d1:	6a 07                	push   $0x7
801005d3:	e9 88 ff ff ff       	jmp    80100560 <vector_handler>

801005d8 <vector8>:
801005d8:	6a 08                	push   $0x8
801005da:	e9 81 ff ff ff       	jmp    80100560 <vector_handler>

801005df <vector9>:
801005df:	6a 00                	push   $0x0
801005e1:	6a 09                	push   $0x9
801005e3:	e9 78 ff ff ff       	jmp    80100560 <vector_handler>

801005e8 <vector10>:
801005e8:	6a 0a                	push   $0xa
801005ea:	e9 71 ff ff ff       	jmp    80100560 <vector_handler>

801005ef <vector11>:
801005ef:	6a 0b                	push   $0xb
801005f1:	e9 6a ff ff ff       	jmp    80100560 <vector_handler>

801005f6 <vector12>:
801005f6:	6a 0c                	push   $0xc
801005f8:	e9 63 ff ff ff       	jmp    80100560 <vector_handler>

801005fd <vector13>:
801005fd:	6a 0d                	push   $0xd
801005ff:	e9 5c ff ff ff       	jmp    80100560 <vector_handler>

80100604 <vector14>:
80100604:	6a 0e                	push   $0xe
80100606:	e9 55 ff ff ff       	jmp    80100560 <vector_handler>

8010060b <vector15>:
8010060b:	6a 00                	push   $0x0
8010060d:	6a 0f                	push   $0xf
8010060f:	e9 4c ff ff ff       	jmp    80100560 <vector_handler>

80100614 <vector16>:
80100614:	6a 00                	push   $0x0
80100616:	6a 10                	push   $0x10
80100618:	e9 43 ff ff ff       	jmp    80100560 <vector_handler>

8010061d <vector17>:
8010061d:	6a 11                	push   $0x11
8010061f:	e9 3c ff ff ff       	jmp    80100560 <vector_handler>

80100624 <vector18>:
80100624:	6a 00                	push   $0x0
80100626:	6a 12                	push   $0x12
80100628:	e9 33 ff ff ff       	jmp    80100560 <vector_handler>

8010062d <vector19>:
8010062d:	6a 00                	push   $0x0
8010062f:	6a 13                	push   $0x13
80100631:	e9 2a ff ff ff       	jmp    80100560 <vector_handler>

80100636 <vector20>:
80100636:	6a 00                	push   $0x0
80100638:	6a 14                	push   $0x14
8010063a:	e9 21 ff ff ff       	jmp    80100560 <vector_handler>

8010063f <vector21>:
8010063f:	6a 00                	push   $0x0
80100641:	6a 15                	push   $0x15
80100643:	e9 18 ff ff ff       	jmp    80100560 <vector_handler>

80100648 <vector22>:
80100648:	6a 00                	push   $0x0
8010064a:	6a 16                	push   $0x16
8010064c:	e9 0f ff ff ff       	jmp    80100560 <vector_handler>

80100651 <vector23>:
80100651:	6a 00                	push   $0x0
80100653:	6a 17                	push   $0x17
80100655:	e9 06 ff ff ff       	jmp    80100560 <vector_handler>

8010065a <vector24>:
8010065a:	6a 00                	push   $0x0
8010065c:	6a 18                	push   $0x18
8010065e:	e9 fd fe ff ff       	jmp    80100560 <vector_handler>

80100663 <vector25>:
80100663:	6a 00                	push   $0x0
80100665:	6a 19                	push   $0x19
80100667:	e9 f4 fe ff ff       	jmp    80100560 <vector_handler>

8010066c <vector26>:
8010066c:	6a 00                	push   $0x0
8010066e:	6a 1a                	push   $0x1a
80100670:	e9 eb fe ff ff       	jmp    80100560 <vector_handler>

80100675 <vector27>:
80100675:	6a 00                	push   $0x0
80100677:	6a 1b                	push   $0x1b
80100679:	e9 e2 fe ff ff       	jmp    80100560 <vector_handler>

8010067e <vector28>:
8010067e:	6a 00                	push   $0x0
80100680:	6a 1c                	push   $0x1c
80100682:	e9 d9 fe ff ff       	jmp    80100560 <vector_handler>

80100687 <vector29>:
80100687:	6a 00                	push   $0x0
80100689:	6a 1d                	push   $0x1d
8010068b:	e9 d0 fe ff ff       	jmp    80100560 <vector_handler>

80100690 <vector30>:
80100690:	6a 00                	push   $0x0
80100692:	6a 1e                	push   $0x1e
80100694:	e9 c7 fe ff ff       	jmp    80100560 <vector_handler>

80100699 <vector31>:
80100699:	6a 00                	push   $0x0
8010069b:	6a 1f                	push   $0x1f
8010069d:	e9 be fe ff ff       	jmp    80100560 <vector_handler>

801006a2 <vector32>:
801006a2:	6a 00                	push   $0x0
801006a4:	6a 20                	push   $0x20
801006a6:	e9 b5 fe ff ff       	jmp    80100560 <vector_handler>

801006ab <vector33>:
801006ab:	6a 00                	push   $0x0
801006ad:	6a 21                	push   $0x21
801006af:	e9 ac fe ff ff       	jmp    80100560 <vector_handler>

801006b4 <vector34>:
801006b4:	6a 00                	push   $0x0
801006b6:	6a 22                	push   $0x22
801006b8:	e9 a3 fe ff ff       	jmp    80100560 <vector_handler>

801006bd <vector35>:
801006bd:	6a 00                	push   $0x0
801006bf:	6a 23                	push   $0x23
801006c1:	e9 9a fe ff ff       	jmp    80100560 <vector_handler>

801006c6 <vector36>:
801006c6:	6a 00                	push   $0x0
801006c8:	6a 24                	push   $0x24
801006ca:	e9 91 fe ff ff       	jmp    80100560 <vector_handler>

801006cf <vector37>:
801006cf:	6a 00                	push   $0x0
801006d1:	6a 25                	push   $0x25
801006d3:	e9 88 fe ff ff       	jmp    80100560 <vector_handler>

801006d8 <vector38>:
801006d8:	6a 00                	push   $0x0
801006da:	6a 26                	push   $0x26
801006dc:	e9 7f fe ff ff       	jmp    80100560 <vector_handler>

801006e1 <vector39>:
801006e1:	6a 00                	push   $0x0
801006e3:	6a 27                	push   $0x27
801006e5:	e9 76 fe ff ff       	jmp    80100560 <vector_handler>

801006ea <vector40>:
801006ea:	6a 00                	push   $0x0
801006ec:	6a 28                	push   $0x28
801006ee:	e9 6d fe ff ff       	jmp    80100560 <vector_handler>

801006f3 <vector41>:
801006f3:	6a 00                	push   $0x0
801006f5:	6a 29                	push   $0x29
801006f7:	e9 64 fe ff ff       	jmp    80100560 <vector_handler>

801006fc <vector42>:
801006fc:	6a 00                	push   $0x0
801006fe:	6a 2a                	push   $0x2a
80100700:	e9 5b fe ff ff       	jmp    80100560 <vector_handler>

80100705 <vector43>:
80100705:	6a 00                	push   $0x0
80100707:	6a 2b                	push   $0x2b
80100709:	e9 52 fe ff ff       	jmp    80100560 <vector_handler>

8010070e <vector44>:
8010070e:	6a 00                	push   $0x0
80100710:	6a 2c                	push   $0x2c
80100712:	e9 49 fe ff ff       	jmp    80100560 <vector_handler>

80100717 <vector45>:
80100717:	6a 00                	push   $0x0
80100719:	6a 2d                	push   $0x2d
8010071b:	e9 40 fe ff ff       	jmp    80100560 <vector_handler>

80100720 <vector46>:
80100720:	6a 00                	push   $0x0
80100722:	6a 2e                	push   $0x2e
80100724:	e9 37 fe ff ff       	jmp    80100560 <vector_handler>

80100729 <vector47>:
80100729:	6a 00                	push   $0x0
8010072b:	6a 2f                	push   $0x2f
8010072d:	e9 2e fe ff ff       	jmp    80100560 <vector_handler>

80100732 <vector48>:
80100732:	6a 00                	push   $0x0
80100734:	6a 30                	push   $0x30
80100736:	e9 25 fe ff ff       	jmp    80100560 <vector_handler>

8010073b <vector49>:
8010073b:	6a 00                	push   $0x0
8010073d:	6a 31                	push   $0x31
8010073f:	e9 1c fe ff ff       	jmp    80100560 <vector_handler>

80100744 <vector50>:
80100744:	6a 00                	push   $0x0
80100746:	6a 32                	push   $0x32
80100748:	e9 13 fe ff ff       	jmp    80100560 <vector_handler>

8010074d <vector51>:
8010074d:	6a 00                	push   $0x0
8010074f:	6a 33                	push   $0x33
80100751:	e9 0a fe ff ff       	jmp    80100560 <vector_handler>

80100756 <vector52>:
80100756:	6a 00                	push   $0x0
80100758:	6a 34                	push   $0x34
8010075a:	e9 01 fe ff ff       	jmp    80100560 <vector_handler>

8010075f <vector53>:
8010075f:	6a 00                	push   $0x0
80100761:	6a 35                	push   $0x35
80100763:	e9 f8 fd ff ff       	jmp    80100560 <vector_handler>

80100768 <vector54>:
80100768:	6a 00                	push   $0x0
8010076a:	6a 36                	push   $0x36
8010076c:	e9 ef fd ff ff       	jmp    80100560 <vector_handler>

80100771 <vector55>:
80100771:	6a 00                	push   $0x0
80100773:	6a 37                	push   $0x37
80100775:	e9 e6 fd ff ff       	jmp    80100560 <vector_handler>

8010077a <vector56>:
8010077a:	6a 00                	push   $0x0
8010077c:	6a 38                	push   $0x38
8010077e:	e9 dd fd ff ff       	jmp    80100560 <vector_handler>

80100783 <vector57>:
80100783:	6a 00                	push   $0x0
80100785:	6a 39                	push   $0x39
80100787:	e9 d4 fd ff ff       	jmp    80100560 <vector_handler>

8010078c <vector58>:
8010078c:	6a 00                	push   $0x0
8010078e:	6a 3a                	push   $0x3a
80100790:	e9 cb fd ff ff       	jmp    80100560 <vector_handler>

80100795 <vector59>:
80100795:	6a 00                	push   $0x0
80100797:	6a 3b                	push   $0x3b
80100799:	e9 c2 fd ff ff       	jmp    80100560 <vector_handler>

8010079e <vector60>:
8010079e:	6a 00                	push   $0x0
801007a0:	6a 3c                	push   $0x3c
801007a2:	e9 b9 fd ff ff       	jmp    80100560 <vector_handler>

801007a7 <vector61>:
801007a7:	6a 00                	push   $0x0
801007a9:	6a 3d                	push   $0x3d
801007ab:	e9 b0 fd ff ff       	jmp    80100560 <vector_handler>

801007b0 <vector62>:
801007b0:	6a 00                	push   $0x0
801007b2:	6a 3e                	push   $0x3e
801007b4:	e9 a7 fd ff ff       	jmp    80100560 <vector_handler>

801007b9 <vector63>:
801007b9:	6a 00                	push   $0x0
801007bb:	6a 3f                	push   $0x3f
801007bd:	e9 9e fd ff ff       	jmp    80100560 <vector_handler>

801007c2 <vector64>:
801007c2:	6a 00                	push   $0x0
801007c4:	6a 40                	push   $0x40
801007c6:	e9 95 fd ff ff       	jmp    80100560 <vector_handler>

801007cb <vector65>:
801007cb:	6a 00                	push   $0x0
801007cd:	6a 41                	push   $0x41
801007cf:	e9 8c fd ff ff       	jmp    80100560 <vector_handler>

801007d4 <vector66>:
801007d4:	6a 00                	push   $0x0
801007d6:	6a 42                	push   $0x42
801007d8:	e9 83 fd ff ff       	jmp    80100560 <vector_handler>

801007dd <vector67>:
801007dd:	6a 00                	push   $0x0
801007df:	6a 43                	push   $0x43
801007e1:	e9 7a fd ff ff       	jmp    80100560 <vector_handler>

801007e6 <vector68>:
801007e6:	6a 00                	push   $0x0
801007e8:	6a 44                	push   $0x44
801007ea:	e9 71 fd ff ff       	jmp    80100560 <vector_handler>

801007ef <vector69>:
801007ef:	6a 00                	push   $0x0
801007f1:	6a 45                	push   $0x45
801007f3:	e9 68 fd ff ff       	jmp    80100560 <vector_handler>

801007f8 <vector70>:
801007f8:	6a 00                	push   $0x0
801007fa:	6a 46                	push   $0x46
801007fc:	e9 5f fd ff ff       	jmp    80100560 <vector_handler>

80100801 <vector71>:
80100801:	6a 00                	push   $0x0
80100803:	6a 47                	push   $0x47
80100805:	e9 56 fd ff ff       	jmp    80100560 <vector_handler>

8010080a <vector72>:
8010080a:	6a 00                	push   $0x0
8010080c:	6a 48                	push   $0x48
8010080e:	e9 4d fd ff ff       	jmp    80100560 <vector_handler>

80100813 <vector73>:
80100813:	6a 00                	push   $0x0
80100815:	6a 49                	push   $0x49
80100817:	e9 44 fd ff ff       	jmp    80100560 <vector_handler>

8010081c <vector74>:
8010081c:	6a 00                	push   $0x0
8010081e:	6a 4a                	push   $0x4a
80100820:	e9 3b fd ff ff       	jmp    80100560 <vector_handler>

80100825 <vector75>:
80100825:	6a 00                	push   $0x0
80100827:	6a 4b                	push   $0x4b
80100829:	e9 32 fd ff ff       	jmp    80100560 <vector_handler>

8010082e <vector76>:
8010082e:	6a 00                	push   $0x0
80100830:	6a 4c                	push   $0x4c
80100832:	e9 29 fd ff ff       	jmp    80100560 <vector_handler>

80100837 <vector77>:
80100837:	6a 00                	push   $0x0
80100839:	6a 4d                	push   $0x4d
8010083b:	e9 20 fd ff ff       	jmp    80100560 <vector_handler>

80100840 <vector78>:
80100840:	6a 00                	push   $0x0
80100842:	6a 4e                	push   $0x4e
80100844:	e9 17 fd ff ff       	jmp    80100560 <vector_handler>

80100849 <vector79>:
80100849:	6a 00                	push   $0x0
8010084b:	6a 4f                	push   $0x4f
8010084d:	e9 0e fd ff ff       	jmp    80100560 <vector_handler>

80100852 <vector80>:
80100852:	6a 00                	push   $0x0
80100854:	6a 50                	push   $0x50
80100856:	e9 05 fd ff ff       	jmp    80100560 <vector_handler>

8010085b <vector81>:
8010085b:	6a 00                	push   $0x0
8010085d:	6a 51                	push   $0x51
8010085f:	e9 fc fc ff ff       	jmp    80100560 <vector_handler>

80100864 <vector82>:
80100864:	6a 00                	push   $0x0
80100866:	6a 52                	push   $0x52
80100868:	e9 f3 fc ff ff       	jmp    80100560 <vector_handler>

8010086d <vector83>:
8010086d:	6a 00                	push   $0x0
8010086f:	6a 53                	push   $0x53
80100871:	e9 ea fc ff ff       	jmp    80100560 <vector_handler>

80100876 <vector84>:
80100876:	6a 00                	push   $0x0
80100878:	6a 54                	push   $0x54
8010087a:	e9 e1 fc ff ff       	jmp    80100560 <vector_handler>

8010087f <vector85>:
8010087f:	6a 00                	push   $0x0
80100881:	6a 55                	push   $0x55
80100883:	e9 d8 fc ff ff       	jmp    80100560 <vector_handler>

80100888 <vector86>:
80100888:	6a 00                	push   $0x0
8010088a:	6a 56                	push   $0x56
8010088c:	e9 cf fc ff ff       	jmp    80100560 <vector_handler>

80100891 <vector87>:
80100891:	6a 00                	push   $0x0
80100893:	6a 57                	push   $0x57
80100895:	e9 c6 fc ff ff       	jmp    80100560 <vector_handler>

8010089a <vector88>:
8010089a:	6a 00                	push   $0x0
8010089c:	6a 58                	push   $0x58
8010089e:	e9 bd fc ff ff       	jmp    80100560 <vector_handler>

801008a3 <vector89>:
801008a3:	6a 00                	push   $0x0
801008a5:	6a 59                	push   $0x59
801008a7:	e9 b4 fc ff ff       	jmp    80100560 <vector_handler>

801008ac <vector90>:
801008ac:	6a 00                	push   $0x0
801008ae:	6a 5a                	push   $0x5a
801008b0:	e9 ab fc ff ff       	jmp    80100560 <vector_handler>

801008b5 <vector91>:
801008b5:	6a 00                	push   $0x0
801008b7:	6a 5b                	push   $0x5b
801008b9:	e9 a2 fc ff ff       	jmp    80100560 <vector_handler>

801008be <vector92>:
801008be:	6a 00                	push   $0x0
801008c0:	6a 5c                	push   $0x5c
801008c2:	e9 99 fc ff ff       	jmp    80100560 <vector_handler>

801008c7 <vector93>:
801008c7:	6a 00                	push   $0x0
801008c9:	6a 5d                	push   $0x5d
801008cb:	e9 90 fc ff ff       	jmp    80100560 <vector_handler>

801008d0 <vector94>:
801008d0:	6a 00                	push   $0x0
801008d2:	6a 5e                	push   $0x5e
801008d4:	e9 87 fc ff ff       	jmp    80100560 <vector_handler>

801008d9 <vector95>:
801008d9:	6a 00                	push   $0x0
801008db:	6a 5f                	push   $0x5f
801008dd:	e9 7e fc ff ff       	jmp    80100560 <vector_handler>

801008e2 <vector96>:
801008e2:	6a 00                	push   $0x0
801008e4:	6a 60                	push   $0x60
801008e6:	e9 75 fc ff ff       	jmp    80100560 <vector_handler>

801008eb <vector97>:
801008eb:	6a 00                	push   $0x0
801008ed:	6a 61                	push   $0x61
801008ef:	e9 6c fc ff ff       	jmp    80100560 <vector_handler>

801008f4 <vector98>:
801008f4:	6a 00                	push   $0x0
801008f6:	6a 62                	push   $0x62
801008f8:	e9 63 fc ff ff       	jmp    80100560 <vector_handler>

801008fd <vector99>:
801008fd:	6a 00                	push   $0x0
801008ff:	6a 63                	push   $0x63
80100901:	e9 5a fc ff ff       	jmp    80100560 <vector_handler>

80100906 <vector100>:
80100906:	6a 00                	push   $0x0
80100908:	6a 64                	push   $0x64
8010090a:	e9 51 fc ff ff       	jmp    80100560 <vector_handler>

8010090f <vector101>:
8010090f:	6a 00                	push   $0x0
80100911:	6a 65                	push   $0x65
80100913:	e9 48 fc ff ff       	jmp    80100560 <vector_handler>

80100918 <vector102>:
80100918:	6a 00                	push   $0x0
8010091a:	6a 66                	push   $0x66
8010091c:	e9 3f fc ff ff       	jmp    80100560 <vector_handler>

80100921 <vector103>:
80100921:	6a 00                	push   $0x0
80100923:	6a 67                	push   $0x67
80100925:	e9 36 fc ff ff       	jmp    80100560 <vector_handler>

8010092a <vector104>:
8010092a:	6a 00                	push   $0x0
8010092c:	6a 68                	push   $0x68
8010092e:	e9 2d fc ff ff       	jmp    80100560 <vector_handler>

80100933 <vector105>:
80100933:	6a 00                	push   $0x0
80100935:	6a 69                	push   $0x69
80100937:	e9 24 fc ff ff       	jmp    80100560 <vector_handler>

8010093c <vector106>:
8010093c:	6a 00                	push   $0x0
8010093e:	6a 6a                	push   $0x6a
80100940:	e9 1b fc ff ff       	jmp    80100560 <vector_handler>

80100945 <vector107>:
80100945:	6a 00                	push   $0x0
80100947:	6a 6b                	push   $0x6b
80100949:	e9 12 fc ff ff       	jmp    80100560 <vector_handler>

8010094e <vector108>:
8010094e:	6a 00                	push   $0x0
80100950:	6a 6c                	push   $0x6c
80100952:	e9 09 fc ff ff       	jmp    80100560 <vector_handler>

80100957 <vector109>:
80100957:	6a 00                	push   $0x0
80100959:	6a 6d                	push   $0x6d
8010095b:	e9 00 fc ff ff       	jmp    80100560 <vector_handler>

80100960 <vector110>:
80100960:	6a 00                	push   $0x0
80100962:	6a 6e                	push   $0x6e
80100964:	e9 f7 fb ff ff       	jmp    80100560 <vector_handler>

80100969 <vector111>:
80100969:	6a 00                	push   $0x0
8010096b:	6a 6f                	push   $0x6f
8010096d:	e9 ee fb ff ff       	jmp    80100560 <vector_handler>

80100972 <vector112>:
80100972:	6a 00                	push   $0x0
80100974:	6a 70                	push   $0x70
80100976:	e9 e5 fb ff ff       	jmp    80100560 <vector_handler>

8010097b <vector113>:
8010097b:	6a 00                	push   $0x0
8010097d:	6a 71                	push   $0x71
8010097f:	e9 dc fb ff ff       	jmp    80100560 <vector_handler>

80100984 <vector114>:
80100984:	6a 00                	push   $0x0
80100986:	6a 72                	push   $0x72
80100988:	e9 d3 fb ff ff       	jmp    80100560 <vector_handler>

8010098d <vector115>:
8010098d:	6a 00                	push   $0x0
8010098f:	6a 73                	push   $0x73
80100991:	e9 ca fb ff ff       	jmp    80100560 <vector_handler>

80100996 <vector116>:
80100996:	6a 00                	push   $0x0
80100998:	6a 74                	push   $0x74
8010099a:	e9 c1 fb ff ff       	jmp    80100560 <vector_handler>

8010099f <vector117>:
8010099f:	6a 00                	push   $0x0
801009a1:	6a 75                	push   $0x75
801009a3:	e9 b8 fb ff ff       	jmp    80100560 <vector_handler>

801009a8 <vector118>:
801009a8:	6a 00                	push   $0x0
801009aa:	6a 76                	push   $0x76
801009ac:	e9 af fb ff ff       	jmp    80100560 <vector_handler>

801009b1 <vector119>:
801009b1:	6a 00                	push   $0x0
801009b3:	6a 77                	push   $0x77
801009b5:	e9 a6 fb ff ff       	jmp    80100560 <vector_handler>

801009ba <vector120>:
801009ba:	6a 00                	push   $0x0
801009bc:	6a 78                	push   $0x78
801009be:	e9 9d fb ff ff       	jmp    80100560 <vector_handler>

801009c3 <vector121>:
801009c3:	6a 00                	push   $0x0
801009c5:	6a 79                	push   $0x79
801009c7:	e9 94 fb ff ff       	jmp    80100560 <vector_handler>

801009cc <vector122>:
801009cc:	6a 00                	push   $0x0
801009ce:	6a 7a                	push   $0x7a
801009d0:	e9 8b fb ff ff       	jmp    80100560 <vector_handler>

801009d5 <vector123>:
801009d5:	6a 00                	push   $0x0
801009d7:	6a 7b                	push   $0x7b
801009d9:	e9 82 fb ff ff       	jmp    80100560 <vector_handler>

801009de <vector124>:
801009de:	6a 00                	push   $0x0
801009e0:	6a 7c                	push   $0x7c
801009e2:	e9 79 fb ff ff       	jmp    80100560 <vector_handler>

801009e7 <vector125>:
801009e7:	6a 00                	push   $0x0
801009e9:	6a 7d                	push   $0x7d
801009eb:	e9 70 fb ff ff       	jmp    80100560 <vector_handler>

801009f0 <vector126>:
801009f0:	6a 00                	push   $0x0
801009f2:	6a 7e                	push   $0x7e
801009f4:	e9 67 fb ff ff       	jmp    80100560 <vector_handler>

801009f9 <vector127>:
801009f9:	6a 00                	push   $0x0
801009fb:	6a 7f                	push   $0x7f
801009fd:	e9 5e fb ff ff       	jmp    80100560 <vector_handler>

80100a02 <vector128>:
80100a02:	6a 00                	push   $0x0
80100a04:	68 80 00 00 00       	push   $0x80
80100a09:	e9 52 fb ff ff       	jmp    80100560 <vector_handler>

80100a0e <vector129>:
80100a0e:	6a 00                	push   $0x0
80100a10:	68 81 00 00 00       	push   $0x81
80100a15:	e9 46 fb ff ff       	jmp    80100560 <vector_handler>

80100a1a <vector130>:
80100a1a:	6a 00                	push   $0x0
80100a1c:	68 82 00 00 00       	push   $0x82
80100a21:	e9 3a fb ff ff       	jmp    80100560 <vector_handler>

80100a26 <vector131>:
80100a26:	6a 00                	push   $0x0
80100a28:	68 83 00 00 00       	push   $0x83
80100a2d:	e9 2e fb ff ff       	jmp    80100560 <vector_handler>

80100a32 <vector132>:
80100a32:	6a 00                	push   $0x0
80100a34:	68 84 00 00 00       	push   $0x84
80100a39:	e9 22 fb ff ff       	jmp    80100560 <vector_handler>

80100a3e <vector133>:
80100a3e:	6a 00                	push   $0x0
80100a40:	68 85 00 00 00       	push   $0x85
80100a45:	e9 16 fb ff ff       	jmp    80100560 <vector_handler>

80100a4a <vector134>:
80100a4a:	6a 00                	push   $0x0
80100a4c:	68 86 00 00 00       	push   $0x86
80100a51:	e9 0a fb ff ff       	jmp    80100560 <vector_handler>

80100a56 <vector135>:
80100a56:	6a 00                	push   $0x0
80100a58:	68 87 00 00 00       	push   $0x87
80100a5d:	e9 fe fa ff ff       	jmp    80100560 <vector_handler>

80100a62 <vector136>:
80100a62:	6a 00                	push   $0x0
80100a64:	68 88 00 00 00       	push   $0x88
80100a69:	e9 f2 fa ff ff       	jmp    80100560 <vector_handler>

80100a6e <vector137>:
80100a6e:	6a 00                	push   $0x0
80100a70:	68 89 00 00 00       	push   $0x89
80100a75:	e9 e6 fa ff ff       	jmp    80100560 <vector_handler>

80100a7a <vector138>:
80100a7a:	6a 00                	push   $0x0
80100a7c:	68 8a 00 00 00       	push   $0x8a
80100a81:	e9 da fa ff ff       	jmp    80100560 <vector_handler>

80100a86 <vector139>:
80100a86:	6a 00                	push   $0x0
80100a88:	68 8b 00 00 00       	push   $0x8b
80100a8d:	e9 ce fa ff ff       	jmp    80100560 <vector_handler>

80100a92 <vector140>:
80100a92:	6a 00                	push   $0x0
80100a94:	68 8c 00 00 00       	push   $0x8c
80100a99:	e9 c2 fa ff ff       	jmp    80100560 <vector_handler>

80100a9e <vector141>:
80100a9e:	6a 00                	push   $0x0
80100aa0:	68 8d 00 00 00       	push   $0x8d
80100aa5:	e9 b6 fa ff ff       	jmp    80100560 <vector_handler>

80100aaa <vector142>:
80100aaa:	6a 00                	push   $0x0
80100aac:	68 8e 00 00 00       	push   $0x8e
80100ab1:	e9 aa fa ff ff       	jmp    80100560 <vector_handler>

80100ab6 <vector143>:
80100ab6:	6a 00                	push   $0x0
80100ab8:	68 8f 00 00 00       	push   $0x8f
80100abd:	e9 9e fa ff ff       	jmp    80100560 <vector_handler>

80100ac2 <vector144>:
80100ac2:	6a 00                	push   $0x0
80100ac4:	68 90 00 00 00       	push   $0x90
80100ac9:	e9 92 fa ff ff       	jmp    80100560 <vector_handler>

80100ace <vector145>:
80100ace:	6a 00                	push   $0x0
80100ad0:	68 91 00 00 00       	push   $0x91
80100ad5:	e9 86 fa ff ff       	jmp    80100560 <vector_handler>

80100ada <vector146>:
80100ada:	6a 00                	push   $0x0
80100adc:	68 92 00 00 00       	push   $0x92
80100ae1:	e9 7a fa ff ff       	jmp    80100560 <vector_handler>

80100ae6 <vector147>:
80100ae6:	6a 00                	push   $0x0
80100ae8:	68 93 00 00 00       	push   $0x93
80100aed:	e9 6e fa ff ff       	jmp    80100560 <vector_handler>

80100af2 <vector148>:
80100af2:	6a 00                	push   $0x0
80100af4:	68 94 00 00 00       	push   $0x94
80100af9:	e9 62 fa ff ff       	jmp    80100560 <vector_handler>

80100afe <vector149>:
80100afe:	6a 00                	push   $0x0
80100b00:	68 95 00 00 00       	push   $0x95
80100b05:	e9 56 fa ff ff       	jmp    80100560 <vector_handler>

80100b0a <vector150>:
80100b0a:	6a 00                	push   $0x0
80100b0c:	68 96 00 00 00       	push   $0x96
80100b11:	e9 4a fa ff ff       	jmp    80100560 <vector_handler>

80100b16 <vector151>:
80100b16:	6a 00                	push   $0x0
80100b18:	68 97 00 00 00       	push   $0x97
80100b1d:	e9 3e fa ff ff       	jmp    80100560 <vector_handler>

80100b22 <vector152>:
80100b22:	6a 00                	push   $0x0
80100b24:	68 98 00 00 00       	push   $0x98
80100b29:	e9 32 fa ff ff       	jmp    80100560 <vector_handler>

80100b2e <vector153>:
80100b2e:	6a 00                	push   $0x0
80100b30:	68 99 00 00 00       	push   $0x99
80100b35:	e9 26 fa ff ff       	jmp    80100560 <vector_handler>

80100b3a <vector154>:
80100b3a:	6a 00                	push   $0x0
80100b3c:	68 9a 00 00 00       	push   $0x9a
80100b41:	e9 1a fa ff ff       	jmp    80100560 <vector_handler>

80100b46 <vector155>:
80100b46:	6a 00                	push   $0x0
80100b48:	68 9b 00 00 00       	push   $0x9b
80100b4d:	e9 0e fa ff ff       	jmp    80100560 <vector_handler>

80100b52 <vector156>:
80100b52:	6a 00                	push   $0x0
80100b54:	68 9c 00 00 00       	push   $0x9c
80100b59:	e9 02 fa ff ff       	jmp    80100560 <vector_handler>

80100b5e <vector157>:
80100b5e:	6a 00                	push   $0x0
80100b60:	68 9d 00 00 00       	push   $0x9d
80100b65:	e9 f6 f9 ff ff       	jmp    80100560 <vector_handler>

80100b6a <vector158>:
80100b6a:	6a 00                	push   $0x0
80100b6c:	68 9e 00 00 00       	push   $0x9e
80100b71:	e9 ea f9 ff ff       	jmp    80100560 <vector_handler>

80100b76 <vector159>:
80100b76:	6a 00                	push   $0x0
80100b78:	68 9f 00 00 00       	push   $0x9f
80100b7d:	e9 de f9 ff ff       	jmp    80100560 <vector_handler>

80100b82 <vector160>:
80100b82:	6a 00                	push   $0x0
80100b84:	68 a0 00 00 00       	push   $0xa0
80100b89:	e9 d2 f9 ff ff       	jmp    80100560 <vector_handler>

80100b8e <vector161>:
80100b8e:	6a 00                	push   $0x0
80100b90:	68 a1 00 00 00       	push   $0xa1
80100b95:	e9 c6 f9 ff ff       	jmp    80100560 <vector_handler>

80100b9a <vector162>:
80100b9a:	6a 00                	push   $0x0
80100b9c:	68 a2 00 00 00       	push   $0xa2
80100ba1:	e9 ba f9 ff ff       	jmp    80100560 <vector_handler>

80100ba6 <vector163>:
80100ba6:	6a 00                	push   $0x0
80100ba8:	68 a3 00 00 00       	push   $0xa3
80100bad:	e9 ae f9 ff ff       	jmp    80100560 <vector_handler>

80100bb2 <vector164>:
80100bb2:	6a 00                	push   $0x0
80100bb4:	68 a4 00 00 00       	push   $0xa4
80100bb9:	e9 a2 f9 ff ff       	jmp    80100560 <vector_handler>

80100bbe <vector165>:
80100bbe:	6a 00                	push   $0x0
80100bc0:	68 a5 00 00 00       	push   $0xa5
80100bc5:	e9 96 f9 ff ff       	jmp    80100560 <vector_handler>

80100bca <vector166>:
80100bca:	6a 00                	push   $0x0
80100bcc:	68 a6 00 00 00       	push   $0xa6
80100bd1:	e9 8a f9 ff ff       	jmp    80100560 <vector_handler>

80100bd6 <vector167>:
80100bd6:	6a 00                	push   $0x0
80100bd8:	68 a7 00 00 00       	push   $0xa7
80100bdd:	e9 7e f9 ff ff       	jmp    80100560 <vector_handler>

80100be2 <vector168>:
80100be2:	6a 00                	push   $0x0
80100be4:	68 a8 00 00 00       	push   $0xa8
80100be9:	e9 72 f9 ff ff       	jmp    80100560 <vector_handler>

80100bee <vector169>:
80100bee:	6a 00                	push   $0x0
80100bf0:	68 a9 00 00 00       	push   $0xa9
80100bf5:	e9 66 f9 ff ff       	jmp    80100560 <vector_handler>

80100bfa <vector170>:
80100bfa:	6a 00                	push   $0x0
80100bfc:	68 aa 00 00 00       	push   $0xaa
80100c01:	e9 5a f9 ff ff       	jmp    80100560 <vector_handler>

80100c06 <vector171>:
80100c06:	6a 00                	push   $0x0
80100c08:	68 ab 00 00 00       	push   $0xab
80100c0d:	e9 4e f9 ff ff       	jmp    80100560 <vector_handler>

80100c12 <vector172>:
80100c12:	6a 00                	push   $0x0
80100c14:	68 ac 00 00 00       	push   $0xac
80100c19:	e9 42 f9 ff ff       	jmp    80100560 <vector_handler>

80100c1e <vector173>:
80100c1e:	6a 00                	push   $0x0
80100c20:	68 ad 00 00 00       	push   $0xad
80100c25:	e9 36 f9 ff ff       	jmp    80100560 <vector_handler>

80100c2a <vector174>:
80100c2a:	6a 00                	push   $0x0
80100c2c:	68 ae 00 00 00       	push   $0xae
80100c31:	e9 2a f9 ff ff       	jmp    80100560 <vector_handler>

80100c36 <vector175>:
80100c36:	6a 00                	push   $0x0
80100c38:	68 af 00 00 00       	push   $0xaf
80100c3d:	e9 1e f9 ff ff       	jmp    80100560 <vector_handler>

80100c42 <vector176>:
80100c42:	6a 00                	push   $0x0
80100c44:	68 b0 00 00 00       	push   $0xb0
80100c49:	e9 12 f9 ff ff       	jmp    80100560 <vector_handler>

80100c4e <vector177>:
80100c4e:	6a 00                	push   $0x0
80100c50:	68 b1 00 00 00       	push   $0xb1
80100c55:	e9 06 f9 ff ff       	jmp    80100560 <vector_handler>

80100c5a <vector178>:
80100c5a:	6a 00                	push   $0x0
80100c5c:	68 b2 00 00 00       	push   $0xb2
80100c61:	e9 fa f8 ff ff       	jmp    80100560 <vector_handler>

80100c66 <vector179>:
80100c66:	6a 00                	push   $0x0
80100c68:	68 b3 00 00 00       	push   $0xb3
80100c6d:	e9 ee f8 ff ff       	jmp    80100560 <vector_handler>

80100c72 <vector180>:
80100c72:	6a 00                	push   $0x0
80100c74:	68 b4 00 00 00       	push   $0xb4
80100c79:	e9 e2 f8 ff ff       	jmp    80100560 <vector_handler>

80100c7e <vector181>:
80100c7e:	6a 00                	push   $0x0
80100c80:	68 b5 00 00 00       	push   $0xb5
80100c85:	e9 d6 f8 ff ff       	jmp    80100560 <vector_handler>

80100c8a <vector182>:
80100c8a:	6a 00                	push   $0x0
80100c8c:	68 b6 00 00 00       	push   $0xb6
80100c91:	e9 ca f8 ff ff       	jmp    80100560 <vector_handler>

80100c96 <vector183>:
80100c96:	6a 00                	push   $0x0
80100c98:	68 b7 00 00 00       	push   $0xb7
80100c9d:	e9 be f8 ff ff       	jmp    80100560 <vector_handler>

80100ca2 <vector184>:
80100ca2:	6a 00                	push   $0x0
80100ca4:	68 b8 00 00 00       	push   $0xb8
80100ca9:	e9 b2 f8 ff ff       	jmp    80100560 <vector_handler>

80100cae <vector185>:
80100cae:	6a 00                	push   $0x0
80100cb0:	68 b9 00 00 00       	push   $0xb9
80100cb5:	e9 a6 f8 ff ff       	jmp    80100560 <vector_handler>

80100cba <vector186>:
80100cba:	6a 00                	push   $0x0
80100cbc:	68 ba 00 00 00       	push   $0xba
80100cc1:	e9 9a f8 ff ff       	jmp    80100560 <vector_handler>

80100cc6 <vector187>:
80100cc6:	6a 00                	push   $0x0
80100cc8:	68 bb 00 00 00       	push   $0xbb
80100ccd:	e9 8e f8 ff ff       	jmp    80100560 <vector_handler>

80100cd2 <vector188>:
80100cd2:	6a 00                	push   $0x0
80100cd4:	68 bc 00 00 00       	push   $0xbc
80100cd9:	e9 82 f8 ff ff       	jmp    80100560 <vector_handler>

80100cde <vector189>:
80100cde:	6a 00                	push   $0x0
80100ce0:	68 bd 00 00 00       	push   $0xbd
80100ce5:	e9 76 f8 ff ff       	jmp    80100560 <vector_handler>

80100cea <vector190>:
80100cea:	6a 00                	push   $0x0
80100cec:	68 be 00 00 00       	push   $0xbe
80100cf1:	e9 6a f8 ff ff       	jmp    80100560 <vector_handler>

80100cf6 <vector191>:
80100cf6:	6a 00                	push   $0x0
80100cf8:	68 bf 00 00 00       	push   $0xbf
80100cfd:	e9 5e f8 ff ff       	jmp    80100560 <vector_handler>

80100d02 <vector192>:
80100d02:	6a 00                	push   $0x0
80100d04:	68 c0 00 00 00       	push   $0xc0
80100d09:	e9 52 f8 ff ff       	jmp    80100560 <vector_handler>

80100d0e <vector193>:
80100d0e:	6a 00                	push   $0x0
80100d10:	68 c1 00 00 00       	push   $0xc1
80100d15:	e9 46 f8 ff ff       	jmp    80100560 <vector_handler>

80100d1a <vector194>:
80100d1a:	6a 00                	push   $0x0
80100d1c:	68 c2 00 00 00       	push   $0xc2
80100d21:	e9 3a f8 ff ff       	jmp    80100560 <vector_handler>

80100d26 <vector195>:
80100d26:	6a 00                	push   $0x0
80100d28:	68 c3 00 00 00       	push   $0xc3
80100d2d:	e9 2e f8 ff ff       	jmp    80100560 <vector_handler>

80100d32 <vector196>:
80100d32:	6a 00                	push   $0x0
80100d34:	68 c4 00 00 00       	push   $0xc4
80100d39:	e9 22 f8 ff ff       	jmp    80100560 <vector_handler>

80100d3e <vector197>:
80100d3e:	6a 00                	push   $0x0
80100d40:	68 c5 00 00 00       	push   $0xc5
80100d45:	e9 16 f8 ff ff       	jmp    80100560 <vector_handler>

80100d4a <vector198>:
80100d4a:	6a 00                	push   $0x0
80100d4c:	68 c6 00 00 00       	push   $0xc6
80100d51:	e9 0a f8 ff ff       	jmp    80100560 <vector_handler>

80100d56 <vector199>:
80100d56:	6a 00                	push   $0x0
80100d58:	68 c7 00 00 00       	push   $0xc7
80100d5d:	e9 fe f7 ff ff       	jmp    80100560 <vector_handler>

80100d62 <vector200>:
80100d62:	6a 00                	push   $0x0
80100d64:	68 c8 00 00 00       	push   $0xc8
80100d69:	e9 f2 f7 ff ff       	jmp    80100560 <vector_handler>

80100d6e <vector201>:
80100d6e:	6a 00                	push   $0x0
80100d70:	68 c9 00 00 00       	push   $0xc9
80100d75:	e9 e6 f7 ff ff       	jmp    80100560 <vector_handler>

80100d7a <vector202>:
80100d7a:	6a 00                	push   $0x0
80100d7c:	68 ca 00 00 00       	push   $0xca
80100d81:	e9 da f7 ff ff       	jmp    80100560 <vector_handler>

80100d86 <vector203>:
80100d86:	6a 00                	push   $0x0
80100d88:	68 cb 00 00 00       	push   $0xcb
80100d8d:	e9 ce f7 ff ff       	jmp    80100560 <vector_handler>

80100d92 <vector204>:
80100d92:	6a 00                	push   $0x0
80100d94:	68 cc 00 00 00       	push   $0xcc
80100d99:	e9 c2 f7 ff ff       	jmp    80100560 <vector_handler>

80100d9e <vector205>:
80100d9e:	6a 00                	push   $0x0
80100da0:	68 cd 00 00 00       	push   $0xcd
80100da5:	e9 b6 f7 ff ff       	jmp    80100560 <vector_handler>

80100daa <vector206>:
80100daa:	6a 00                	push   $0x0
80100dac:	68 ce 00 00 00       	push   $0xce
80100db1:	e9 aa f7 ff ff       	jmp    80100560 <vector_handler>

80100db6 <vector207>:
80100db6:	6a 00                	push   $0x0
80100db8:	68 cf 00 00 00       	push   $0xcf
80100dbd:	e9 9e f7 ff ff       	jmp    80100560 <vector_handler>

80100dc2 <vector208>:
80100dc2:	6a 00                	push   $0x0
80100dc4:	68 d0 00 00 00       	push   $0xd0
80100dc9:	e9 92 f7 ff ff       	jmp    80100560 <vector_handler>

80100dce <vector209>:
80100dce:	6a 00                	push   $0x0
80100dd0:	68 d1 00 00 00       	push   $0xd1
80100dd5:	e9 86 f7 ff ff       	jmp    80100560 <vector_handler>

80100dda <vector210>:
80100dda:	6a 00                	push   $0x0
80100ddc:	68 d2 00 00 00       	push   $0xd2
80100de1:	e9 7a f7 ff ff       	jmp    80100560 <vector_handler>

80100de6 <vector211>:
80100de6:	6a 00                	push   $0x0
80100de8:	68 d3 00 00 00       	push   $0xd3
80100ded:	e9 6e f7 ff ff       	jmp    80100560 <vector_handler>

80100df2 <vector212>:
80100df2:	6a 00                	push   $0x0
80100df4:	68 d4 00 00 00       	push   $0xd4
80100df9:	e9 62 f7 ff ff       	jmp    80100560 <vector_handler>

80100dfe <vector213>:
80100dfe:	6a 00                	push   $0x0
80100e00:	68 d5 00 00 00       	push   $0xd5
80100e05:	e9 56 f7 ff ff       	jmp    80100560 <vector_handler>

80100e0a <vector214>:
80100e0a:	6a 00                	push   $0x0
80100e0c:	68 d6 00 00 00       	push   $0xd6
80100e11:	e9 4a f7 ff ff       	jmp    80100560 <vector_handler>

80100e16 <vector215>:
80100e16:	6a 00                	push   $0x0
80100e18:	68 d7 00 00 00       	push   $0xd7
80100e1d:	e9 3e f7 ff ff       	jmp    80100560 <vector_handler>

80100e22 <vector216>:
80100e22:	6a 00                	push   $0x0
80100e24:	68 d8 00 00 00       	push   $0xd8
80100e29:	e9 32 f7 ff ff       	jmp    80100560 <vector_handler>

80100e2e <vector217>:
80100e2e:	6a 00                	push   $0x0
80100e30:	68 d9 00 00 00       	push   $0xd9
80100e35:	e9 26 f7 ff ff       	jmp    80100560 <vector_handler>

80100e3a <vector218>:
80100e3a:	6a 00                	push   $0x0
80100e3c:	68 da 00 00 00       	push   $0xda
80100e41:	e9 1a f7 ff ff       	jmp    80100560 <vector_handler>

80100e46 <vector219>:
80100e46:	6a 00                	push   $0x0
80100e48:	68 db 00 00 00       	push   $0xdb
80100e4d:	e9 0e f7 ff ff       	jmp    80100560 <vector_handler>

80100e52 <vector220>:
80100e52:	6a 00                	push   $0x0
80100e54:	68 dc 00 00 00       	push   $0xdc
80100e59:	e9 02 f7 ff ff       	jmp    80100560 <vector_handler>

80100e5e <vector221>:
80100e5e:	6a 00                	push   $0x0
80100e60:	68 dd 00 00 00       	push   $0xdd
80100e65:	e9 f6 f6 ff ff       	jmp    80100560 <vector_handler>

80100e6a <vector222>:
80100e6a:	6a 00                	push   $0x0
80100e6c:	68 de 00 00 00       	push   $0xde
80100e71:	e9 ea f6 ff ff       	jmp    80100560 <vector_handler>

80100e76 <vector223>:
80100e76:	6a 00                	push   $0x0
80100e78:	68 df 00 00 00       	push   $0xdf
80100e7d:	e9 de f6 ff ff       	jmp    80100560 <vector_handler>

80100e82 <vector224>:
80100e82:	6a 00                	push   $0x0
80100e84:	68 e0 00 00 00       	push   $0xe0
80100e89:	e9 d2 f6 ff ff       	jmp    80100560 <vector_handler>

80100e8e <vector225>:
80100e8e:	6a 00                	push   $0x0
80100e90:	68 e1 00 00 00       	push   $0xe1
80100e95:	e9 c6 f6 ff ff       	jmp    80100560 <vector_handler>

80100e9a <vector226>:
80100e9a:	6a 00                	push   $0x0
80100e9c:	68 e2 00 00 00       	push   $0xe2
80100ea1:	e9 ba f6 ff ff       	jmp    80100560 <vector_handler>

80100ea6 <vector227>:
80100ea6:	6a 00                	push   $0x0
80100ea8:	68 e3 00 00 00       	push   $0xe3
80100ead:	e9 ae f6 ff ff       	jmp    80100560 <vector_handler>

80100eb2 <vector228>:
80100eb2:	6a 00                	push   $0x0
80100eb4:	68 e4 00 00 00       	push   $0xe4
80100eb9:	e9 a2 f6 ff ff       	jmp    80100560 <vector_handler>

80100ebe <vector229>:
80100ebe:	6a 00                	push   $0x0
80100ec0:	68 e5 00 00 00       	push   $0xe5
80100ec5:	e9 96 f6 ff ff       	jmp    80100560 <vector_handler>

80100eca <vector230>:
80100eca:	6a 00                	push   $0x0
80100ecc:	68 e6 00 00 00       	push   $0xe6
80100ed1:	e9 8a f6 ff ff       	jmp    80100560 <vector_handler>

80100ed6 <vector231>:
80100ed6:	6a 00                	push   $0x0
80100ed8:	68 e7 00 00 00       	push   $0xe7
80100edd:	e9 7e f6 ff ff       	jmp    80100560 <vector_handler>

80100ee2 <vector232>:
80100ee2:	6a 00                	push   $0x0
80100ee4:	68 e8 00 00 00       	push   $0xe8
80100ee9:	e9 72 f6 ff ff       	jmp    80100560 <vector_handler>

80100eee <vector233>:
80100eee:	6a 00                	push   $0x0
80100ef0:	68 e9 00 00 00       	push   $0xe9
80100ef5:	e9 66 f6 ff ff       	jmp    80100560 <vector_handler>

80100efa <vector234>:
80100efa:	6a 00                	push   $0x0
80100efc:	68 ea 00 00 00       	push   $0xea
80100f01:	e9 5a f6 ff ff       	jmp    80100560 <vector_handler>

80100f06 <vector235>:
80100f06:	6a 00                	push   $0x0
80100f08:	68 eb 00 00 00       	push   $0xeb
80100f0d:	e9 4e f6 ff ff       	jmp    80100560 <vector_handler>

80100f12 <vector236>:
80100f12:	6a 00                	push   $0x0
80100f14:	68 ec 00 00 00       	push   $0xec
80100f19:	e9 42 f6 ff ff       	jmp    80100560 <vector_handler>

80100f1e <vector237>:
80100f1e:	6a 00                	push   $0x0
80100f20:	68 ed 00 00 00       	push   $0xed
80100f25:	e9 36 f6 ff ff       	jmp    80100560 <vector_handler>

80100f2a <vector238>:
80100f2a:	6a 00                	push   $0x0
80100f2c:	68 ee 00 00 00       	push   $0xee
80100f31:	e9 2a f6 ff ff       	jmp    80100560 <vector_handler>

80100f36 <vector239>:
80100f36:	6a 00                	push   $0x0
80100f38:	68 ef 00 00 00       	push   $0xef
80100f3d:	e9 1e f6 ff ff       	jmp    80100560 <vector_handler>

80100f42 <vector240>:
80100f42:	6a 00                	push   $0x0
80100f44:	68 f0 00 00 00       	push   $0xf0
80100f49:	e9 12 f6 ff ff       	jmp    80100560 <vector_handler>

80100f4e <vector241>:
80100f4e:	6a 00                	push   $0x0
80100f50:	68 f1 00 00 00       	push   $0xf1
80100f55:	e9 06 f6 ff ff       	jmp    80100560 <vector_handler>

80100f5a <vector242>:
80100f5a:	6a 00                	push   $0x0
80100f5c:	68 f2 00 00 00       	push   $0xf2
80100f61:	e9 fa f5 ff ff       	jmp    80100560 <vector_handler>

80100f66 <vector243>:
80100f66:	6a 00                	push   $0x0
80100f68:	68 f3 00 00 00       	push   $0xf3
80100f6d:	e9 ee f5 ff ff       	jmp    80100560 <vector_handler>

80100f72 <vector244>:
80100f72:	6a 00                	push   $0x0
80100f74:	68 f4 00 00 00       	push   $0xf4
80100f79:	e9 e2 f5 ff ff       	jmp    80100560 <vector_handler>

80100f7e <vector245>:
80100f7e:	6a 00                	push   $0x0
80100f80:	68 f5 00 00 00       	push   $0xf5
80100f85:	e9 d6 f5 ff ff       	jmp    80100560 <vector_handler>

80100f8a <vector246>:
80100f8a:	6a 00                	push   $0x0
80100f8c:	68 f6 00 00 00       	push   $0xf6
80100f91:	e9 ca f5 ff ff       	jmp    80100560 <vector_handler>

80100f96 <vector247>:
80100f96:	6a 00                	push   $0x0
80100f98:	68 f7 00 00 00       	push   $0xf7
80100f9d:	e9 be f5 ff ff       	jmp    80100560 <vector_handler>

80100fa2 <vector248>:
80100fa2:	6a 00                	push   $0x0
80100fa4:	68 f8 00 00 00       	push   $0xf8
80100fa9:	e9 b2 f5 ff ff       	jmp    80100560 <vector_handler>

80100fae <vector249>:
80100fae:	6a 00                	push   $0x0
80100fb0:	68 f9 00 00 00       	push   $0xf9
80100fb5:	e9 a6 f5 ff ff       	jmp    80100560 <vector_handler>

80100fba <vector250>:
80100fba:	6a 00                	push   $0x0
80100fbc:	68 fa 00 00 00       	push   $0xfa
80100fc1:	e9 9a f5 ff ff       	jmp    80100560 <vector_handler>

80100fc6 <vector251>:
80100fc6:	6a 00                	push   $0x0
80100fc8:	68 fb 00 00 00       	push   $0xfb
80100fcd:	e9 8e f5 ff ff       	jmp    80100560 <vector_handler>

80100fd2 <vector252>:
80100fd2:	6a 00                	push   $0x0
80100fd4:	68 fc 00 00 00       	push   $0xfc
80100fd9:	e9 82 f5 ff ff       	jmp    80100560 <vector_handler>

80100fde <vector253>:
80100fde:	6a 00                	push   $0x0
80100fe0:	68 fd 00 00 00       	push   $0xfd
80100fe5:	e9 76 f5 ff ff       	jmp    80100560 <vector_handler>

80100fea <vector254>:
80100fea:	6a 00                	push   $0x0
80100fec:	68 fe 00 00 00       	push   $0xfe
80100ff1:	e9 6a f5 ff ff       	jmp    80100560 <vector_handler>

80100ff6 <vector255>:
80100ff6:	6a 00                	push   $0x0
80100ff8:	68 ff 00 00 00       	push   $0xff
80100ffd:	e9 5e f5 ff ff       	jmp    80100560 <vector_handler>

80101002 <itoa>:
80101002:	55                   	push   %ebp
80101003:	57                   	push   %edi
80101004:	56                   	push   %esi
80101005:	53                   	push   %ebx
80101006:	8b 54 24 14          	mov    0x14(%esp),%edx
8010100a:	8b 6c 24 18          	mov    0x18(%esp),%ebp
8010100e:	8b 7c 24 1c          	mov    0x1c(%esp),%edi
80101012:	8d 47 fe             	lea    -0x2(%edi),%eax
80101015:	83 f8 22             	cmp    $0x22,%eax
80101018:	77 0a                	ja     80101024 <itoa+0x22>
8010101a:	85 d2                	test   %edx,%edx
8010101c:	78 11                	js     8010102f <itoa+0x2d>
8010101e:	89 eb                	mov    %ebp,%ebx
80101020:	89 d9                	mov    %ebx,%ecx
80101022:	eb 1f                	jmp    80101043 <itoa+0x41>
80101024:	c6 45 00 00          	movb   $0x0,0x0(%ebp)
80101028:	89 e8                	mov    %ebp,%eax
8010102a:	5b                   	pop    %ebx
8010102b:	5e                   	pop    %esi
8010102c:	5f                   	pop    %edi
8010102d:	5d                   	pop    %ebp
8010102e:	c3                   	ret    
8010102f:	83 ff 0a             	cmp    $0xa,%edi
80101032:	74 04                	je     80101038 <itoa+0x36>
80101034:	89 eb                	mov    %ebp,%ebx
80101036:	eb e8                	jmp    80101020 <itoa+0x1e>
80101038:	8d 5d 01             	lea    0x1(%ebp),%ebx
8010103b:	c6 45 00 2d          	movb   $0x2d,0x0(%ebp)
8010103f:	eb df                	jmp    80101020 <itoa+0x1e>
80101041:	89 f1                	mov    %esi,%ecx
80101043:	8d 71 01             	lea    0x1(%ecx),%esi
80101046:	89 d0                	mov    %edx,%eax
80101048:	99                   	cltd   
80101049:	f7 ff                	idiv   %edi
8010104b:	8a 92 df 12 10 80    	mov    -0x7fefed21(%edx),%dl
80101051:	88 56 ff             	mov    %dl,-0x1(%esi)
80101054:	89 c2                	mov    %eax,%edx
80101056:	85 c0                	test   %eax,%eax
80101058:	75 e7                	jne    80101041 <itoa+0x3f>
8010105a:	c6 41 01 00          	movb   $0x0,0x1(%ecx)
8010105e:	39 cb                	cmp    %ecx,%ebx
80101060:	73 c6                	jae    80101028 <itoa+0x26>
80101062:	8a 03                	mov    (%ebx),%al
80101064:	43                   	inc    %ebx
80101065:	8a 11                	mov    (%ecx),%dl
80101067:	88 53 ff             	mov    %dl,-0x1(%ebx)
8010106a:	49                   	dec    %ecx
8010106b:	88 41 01             	mov    %al,0x1(%ecx)
8010106e:	39 cb                	cmp    %ecx,%ebx
80101070:	72 f0                	jb     80101062 <itoa+0x60>
80101072:	eb b4                	jmp    80101028 <itoa+0x26>

80101074 <write_string>:
80101074:	55                   	push   %ebp
80101075:	57                   	push   %edi
80101076:	56                   	push   %esi
80101077:	53                   	push   %ebx
80101078:	8b 5c 24 14          	mov    0x14(%esp),%ebx
8010107c:	8b 4c 24 18          	mov    0x18(%esp),%ecx
80101080:	8a 01                	mov    (%ecx),%al
80101082:	41                   	inc    %ecx
80101083:	be 67 66 66 66       	mov    $0x66666667,%esi
80101088:	eb 2d                	jmp    801010b7 <write_string+0x43>
8010108a:	a1 00 34 10 80       	mov    0x80103400,%eax
8010108f:	8d a8 00 80 f4 ff    	lea    -0xb8000(%eax),%ebp
80101095:	89 e8                	mov    %ebp,%eax
80101097:	f7 ee                	imul   %esi
80101099:	89 d0                	mov    %edx,%eax
8010109b:	c1 f8 06             	sar    $0x6,%eax
8010109e:	c1 fd 1f             	sar    $0x1f,%ebp
801010a1:	29 e8                	sub    %ebp,%eax
801010a3:	8d 44 80 05          	lea    0x5(%eax,%eax,4),%eax
801010a7:	c1 e0 05             	shl    $0x5,%eax
801010aa:	05 00 80 0b 00       	add    $0xb8000,%eax
801010af:	a3 00 34 10 80       	mov    %eax,0x80103400
801010b4:	8a 07                	mov    (%edi),%al
801010b6:	41                   	inc    %ecx
801010b7:	89 cf                	mov    %ecx,%edi
801010b9:	80 79 ff 00          	cmpb   $0x0,-0x1(%ecx)
801010bd:	74 2a                	je     801010e9 <write_string+0x75>
801010bf:	3c 0a                	cmp    $0xa,%al
801010c1:	74 c7                	je     8010108a <write_string+0x16>
801010c3:	8b 15 00 34 10 80    	mov    0x80103400,%edx
801010c9:	8d 6a 01             	lea    0x1(%edx),%ebp
801010cc:	89 2d 00 34 10 80    	mov    %ebp,0x80103400
801010d2:	88 02                	mov    %al,(%edx)
801010d4:	8a 07                	mov    (%edi),%al
801010d6:	8b 15 00 34 10 80    	mov    0x80103400,%edx
801010dc:	8d 7a 01             	lea    0x1(%edx),%edi
801010df:	89 3d 00 34 10 80    	mov    %edi,0x80103400
801010e5:	88 1a                	mov    %bl,(%edx)
801010e7:	eb cd                	jmp    801010b6 <write_string+0x42>
801010e9:	5b                   	pop    %ebx
801010ea:	5e                   	pop    %esi
801010eb:	5f                   	pop    %edi
801010ec:	5d                   	pop    %ebp
801010ed:	c3                   	ret    

801010ee <clr_scr>:
801010ee:	c7 05 00 34 10 80 00 	movl   $0xb8000,0x80103400
801010f5:	80 0b 00 
801010f8:	b8 00 80 0b 00       	mov    $0xb8000,%eax
801010fd:	eb 02                	jmp    80101101 <clr_scr+0x13>
801010ff:	89 d0                	mov    %edx,%eax
80101101:	c6 00 00             	movb   $0x0,(%eax)
80101104:	8d 50 02             	lea    0x2(%eax),%edx
80101107:	c6 40 01 00          	movb   $0x0,0x1(%eax)
8010110b:	3d 9e 8f 0b 00       	cmp    $0xb8f9e,%eax
80101110:	75 ed                	jne    801010ff <clr_scr+0x11>
80101112:	c3                   	ret    

80101113 <memcpy>:
80101113:	56                   	push   %esi
80101114:	53                   	push   %ebx
80101115:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101119:	8b 5c 24 10          	mov    0x10(%esp),%ebx
8010111d:	8b 4c 24 14          	mov    0x14(%esp),%ecx
80101121:	39 c3                	cmp    %eax,%ebx
80101123:	72 19                	jb     8010113e <memcpy+0x2b>
80101125:	85 c9                	test   %ecx,%ecx
80101127:	7e 12                	jle    8010113b <memcpy+0x28>
80101129:	89 ce                	mov    %ecx,%esi
8010112b:	ba 00 00 00 00       	mov    $0x0,%edx
80101130:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
80101133:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101136:	42                   	inc    %edx
80101137:	39 d6                	cmp    %edx,%esi
80101139:	75 f5                	jne    80101130 <memcpy+0x1d>
8010113b:	5b                   	pop    %ebx
8010113c:	5e                   	pop    %esi
8010113d:	c3                   	ret    
8010113e:	8d 14 0b             	lea    (%ebx,%ecx,1),%edx
80101141:	39 d0                	cmp    %edx,%eax
80101143:	73 e0                	jae    80101125 <memcpy+0x12>
80101145:	8d 51 ff             	lea    -0x1(%ecx),%edx
80101148:	85 c9                	test   %ecx,%ecx
8010114a:	7e ef                	jle    8010113b <memcpy+0x28>
8010114c:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
8010114f:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101152:	4a                   	dec    %edx
80101153:	83 fa ff             	cmp    $0xffffffff,%edx
80101156:	75 f4                	jne    8010114c <memcpy+0x39>
80101158:	eb e1                	jmp    8010113b <memcpy+0x28>

8010115a <memset>:
8010115a:	57                   	push   %edi
8010115b:	8b 54 24 08          	mov    0x8(%esp),%edx
8010115f:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101163:	8b 4c 24 10          	mov    0x10(%esp),%ecx
80101167:	89 d7                	mov    %edx,%edi
80101169:	09 cf                	or     %ecx,%edi
8010116b:	f7 c7 03 00 00 00    	test   $0x3,%edi
80101171:	74 09                	je     8010117c <memset+0x22>
80101173:	89 d7                	mov    %edx,%edi
80101175:	fc                   	cld    
80101176:	f3 aa                	rep stos %al,%es:(%edi)
80101178:	89 d0                	mov    %edx,%eax
8010117a:	5f                   	pop    %edi
8010117b:	c3                   	ret    
8010117c:	89 cf                	mov    %ecx,%edi
8010117e:	85 c9                	test   %ecx,%ecx
80101180:	78 0c                	js     8010118e <memset+0x34>
80101182:	89 f9                	mov    %edi,%ecx
80101184:	c1 f9 02             	sar    $0x2,%ecx
80101187:	89 d7                	mov    %edx,%edi
80101189:	fc                   	cld    
8010118a:	f3 ab                	rep stos %eax,%es:(%edi)
8010118c:	eb ea                	jmp    80101178 <memset+0x1e>
8010118e:	8d 79 03             	lea    0x3(%ecx),%edi
80101191:	eb ef                	jmp    80101182 <memset+0x28>

80101193 <strlen>:
80101193:	8b 54 24 04          	mov    0x4(%esp),%edx
80101197:	80 3a 00             	cmpb   $0x0,(%edx)
8010119a:	74 0d                	je     801011a9 <strlen+0x16>
8010119c:	b8 00 00 00 00       	mov    $0x0,%eax
801011a1:	40                   	inc    %eax
801011a2:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
801011a6:	75 f9                	jne    801011a1 <strlen+0xe>
801011a8:	c3                   	ret    
801011a9:	b8 00 00 00 00       	mov    $0x0,%eax
801011ae:	c3                   	ret    

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
8010001c:	bc b0 53 10 80       	mov    $0x801053b0,%esp
80100021:	b8 be 00 10 80       	mov    $0x801000be,%eax
80100026:	ff e0                	jmp    *%eax

80100028 <interrupt_handler>:
80100028:	eb fe                	jmp    80100028 <interrupt_handler>

8010002a <init_8259A>:
8010002a:	53                   	push   %ebx
8010002b:	b0 11                	mov    $0x11,%al
8010002d:	ba 20 00 00 00       	mov    $0x20,%edx
80100032:	ee                   	out    %al,(%dx)
80100033:	ba a0 00 00 00       	mov    $0xa0,%edx
80100038:	ee                   	out    %al,(%dx)
80100039:	bb 21 00 00 00       	mov    $0x21,%ebx
8010003e:	b0 20                	mov    $0x20,%al
80100040:	89 da                	mov    %ebx,%edx
80100042:	ee                   	out    %al,(%dx)
80100043:	b9 a1 00 00 00       	mov    $0xa1,%ecx
80100048:	b0 28                	mov    $0x28,%al
8010004a:	89 ca                	mov    %ecx,%edx
8010004c:	ee                   	out    %al,(%dx)
8010004d:	b0 04                	mov    $0x4,%al
8010004f:	89 da                	mov    %ebx,%edx
80100051:	ee                   	out    %al,(%dx)
80100052:	b0 02                	mov    $0x2,%al
80100054:	89 ca                	mov    %ecx,%edx
80100056:	ee                   	out    %al,(%dx)
80100057:	b0 01                	mov    $0x1,%al
80100059:	89 da                	mov    %ebx,%edx
8010005b:	ee                   	out    %al,(%dx)
8010005c:	89 ca                	mov    %ecx,%edx
8010005e:	ee                   	out    %al,(%dx)
8010005f:	b0 ff                	mov    $0xff,%al
80100061:	89 da                	mov    %ebx,%edx
80100063:	ee                   	out    %al,(%dx)
80100064:	89 ca                	mov    %ecx,%edx
80100066:	ee                   	out    %al,(%dx)
80100067:	5b                   	pop    %ebx
80100068:	c3                   	ret    

80100069 <make_desc>:
80100069:	57                   	push   %edi
8010006a:	56                   	push   %esi
8010006b:	53                   	push   %ebx
8010006c:	8b 5c 24 14          	mov    0x14(%esp),%ebx
80100070:	8b 7c 24 18          	mov    0x18(%esp),%edi
80100074:	8b 54 24 1c          	mov    0x1c(%esp),%edx
80100078:	8b 74 24 10          	mov    0x10(%esp),%esi
8010007c:	c1 e6 06             	shl    $0x6,%esi
8010007f:	8d 8e 00 00 20 80    	lea    -0x7fe00000(%esi),%ecx
80100085:	66 89 be 00 00 20 80 	mov    %di,-0x7fe00000(%esi)
8010008c:	66 89 9e 02 00 20 80 	mov    %bx,-0x7fdffffe(%esi)
80100093:	89 de                	mov    %ebx,%esi
80100095:	c1 ee 10             	shr    $0x10,%esi
80100098:	89 f0                	mov    %esi,%eax
8010009a:	88 41 04             	mov    %al,0x4(%ecx)
8010009d:	c1 eb 18             	shr    $0x18,%ebx
801000a0:	88 59 07             	mov    %bl,0x7(%ecx)
801000a3:	88 51 05             	mov    %dl,0x5(%ecx)
801000a6:	89 f8                	mov    %edi,%eax
801000a8:	c1 e8 10             	shr    $0x10,%eax
801000ab:	83 e0 0f             	and    $0xf,%eax
801000ae:	66 c1 ea 08          	shr    $0x8,%dx
801000b2:	83 e2 f0             	and    $0xfffffff0,%edx
801000b5:	09 d0                	or     %edx,%eax
801000b7:	88 41 06             	mov    %al,0x6(%ecx)
801000ba:	5b                   	pop    %ebx
801000bb:	5e                   	pop    %esi
801000bc:	5f                   	pop    %edi
801000bd:	c3                   	ret    

801000be <main>:
801000be:	8d 4c 24 04          	lea    0x4(%esp),%ecx
801000c2:	83 e4 f0             	and    $0xfffffff0,%esp
801000c5:	ff 71 fc             	pushl  -0x4(%ecx)
801000c8:	55                   	push   %ebp
801000c9:	89 e5                	mov    %esp,%ebp
801000cb:	56                   	push   %esi
801000cc:	53                   	push   %ebx
801000cd:	51                   	push   %ecx
801000ce:	83 ec 3c             	sub    $0x3c,%esp
801000d1:	e8 18 10 00 00       	call   801010ee <clr_scr>
801000d6:	6a 00                	push   $0x0
801000d8:	6a 00                	push   $0x0
801000da:	6a 00                	push   $0x0
801000dc:	6a 00                	push   $0x0
801000de:	e8 86 ff ff ff       	call   80100069 <make_desc>
801000e3:	68 98 c0 00 00       	push   $0xc098
801000e8:	68 00 00 f0 80       	push   $0x80f00000
801000ed:	6a 00                	push   $0x0
801000ef:	6a 01                	push   $0x1
801000f1:	e8 73 ff ff ff       	call   80100069 <make_desc>
801000f6:	83 c4 20             	add    $0x20,%esp
801000f9:	68 92 c0 00 00       	push   $0xc092
801000fe:	68 00 00 f0 80       	push   $0x80f00000
80100103:	6a 00                	push   $0x0
80100105:	6a 02                	push   $0x2
80100107:	e8 5d ff ff ff       	call   80100069 <make_desc>
8010010c:	68 f2 00 00 00       	push   $0xf2
80100111:	68 ff ff 00 00       	push   $0xffff
80100116:	68 00 80 0b 00       	push   $0xb8000
8010011b:	6a 03                	push   $0x3
8010011d:	e8 47 ff ff ff       	call   80100069 <make_desc>
80100122:	66 c7 05 b8 53 10 80 	movw   $0x1f,0x801053b8
80100129:	1f 00 
8010012b:	c7 05 ba 53 10 80 00 	movl   $0x80200000,0x801053ba
80100132:	00 20 80 
80100135:	0f 01 15 b8 53 10 80 	lgdtl  0x801053b8
8010013c:	83 c4 1c             	add    $0x1c,%esp
8010013f:	6a 10                	push   $0x10
80100141:	8d 5d c8             	lea    -0x38(%ebp),%ebx
80100144:	53                   	push   %ebx
80100145:	68 00 30 10 80       	push   $0x80103000
8010014a:	e8 b3 0e 00 00       	call   80101002 <itoa>
8010014f:	83 c4 08             	add    $0x8,%esp
80100152:	68 b0 11 10 80       	push   $0x801011b0
80100157:	6a 0f                	push   $0xf
80100159:	e8 16 0f 00 00       	call   80101074 <write_string>
8010015e:	83 c4 08             	add    $0x8,%esp
80100161:	53                   	push   %ebx
80100162:	6a 0f                	push   $0xf
80100164:	e8 0b 0f 00 00       	call   80101074 <write_string>
80100169:	83 c4 08             	add    $0x8,%esp
8010016c:	68 f1 11 10 80       	push   $0x801011f1
80100171:	6a 0f                	push   $0xf
80100173:	e8 fc 0e 00 00       	call   80101074 <write_string>
80100178:	b9 00 30 10 80       	mov    $0x80103000,%ecx
8010017d:	83 c4 10             	add    $0x10,%esp
80100180:	b8 00 04 20 80       	mov    $0x80200400,%eax
80100185:	8b 11                	mov    (%ecx),%edx
80100187:	66 89 10             	mov    %dx,(%eax)
8010018a:	c1 ea 10             	shr    $0x10,%edx
8010018d:	66 89 50 06          	mov    %dx,0x6(%eax)
80100191:	66 c7 40 02 08 00    	movw   $0x8,0x2(%eax)
80100197:	c6 40 04 00          	movb   $0x0,0x4(%eax)
8010019b:	c6 40 05 8e          	movb   $0x8e,0x5(%eax)
8010019f:	83 c1 04             	add    $0x4,%ecx
801001a2:	83 c0 08             	add    $0x8,%eax
801001a5:	3d 00 0c 20 80       	cmp    $0x80200c00,%eax
801001aa:	75 d9                	jne    80100185 <main+0xc7>
801001ac:	66 c7 05 b0 53 10 80 	movw   $0x7ff,0x801053b0
801001b3:	ff 07 
801001b5:	c7 05 b2 53 10 80 00 	movl   $0x80200400,0x801053b2
801001bc:	04 20 80 
801001bf:	0f 01 1d b0 53 10 80 	lidtl  0x801053b0
801001c6:	e8 5f fe ff ff       	call   8010002a <init_8259A>
801001cb:	83 ec 08             	sub    $0x8,%esp
801001ce:	68 ca 11 10 80       	push   $0x801011ca
801001d3:	6a 0f                	push   $0xf
801001d5:	e8 9a 0e 00 00       	call   80101074 <write_string>
801001da:	83 c4 08             	add    $0x8,%esp
801001dd:	68 d8 11 10 80       	push   $0x801011d8
801001e2:	6a 0f                	push   $0xf
801001e4:	e8 8b 0e 00 00       	call   80101074 <write_string>
801001e9:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
801001ef:	8d 04 12             	lea    (%edx,%edx,1),%eax
801001f2:	01 d0                	add    %edx,%eax
801001f4:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
801001fb:	83 c4 10             	add    $0x10,%esp
801001fe:	3d 00 80 00 00       	cmp    $0x8000,%eax
80100203:	0f 86 18 01 00 00    	jbe    80100321 <main+0x263>
80100209:	bb 00 80 00 00       	mov    $0x8000,%ebx
8010020e:	8d 75 b8             	lea    -0x48(%ebp),%esi
80100211:	eb 2f                	jmp    80100242 <main+0x184>
80100213:	83 ec 08             	sub    $0x8,%esp
80100216:	68 13 12 10 80       	push   $0x80101213
8010021b:	6a 0f                	push   $0xf
8010021d:	e8 52 0e 00 00       	call   80101074 <write_string>
80100222:	83 c4 10             	add    $0x10,%esp
80100225:	83 c3 18             	add    $0x18,%ebx
80100228:	8b 15 fc 7f 00 00    	mov    0x7ffc,%edx
8010022e:	8d 04 12             	lea    (%edx,%edx,1),%eax
80100231:	01 d0                	add    %edx,%eax
80100233:	8d 04 c5 00 80 00 00 	lea    0x8000(,%eax,8),%eax
8010023a:	39 c3                	cmp    %eax,%ebx
8010023c:	0f 83 df 00 00 00    	jae    80100321 <main+0x263>
80100242:	83 ec 04             	sub    $0x4,%esp
80100245:	6a 10                	push   $0x10
80100247:	56                   	push   %esi
80100248:	8b 43 04             	mov    0x4(%ebx),%eax
8010024b:	c1 e0 10             	shl    $0x10,%eax
8010024e:	0b 03                	or     (%ebx),%eax
80100250:	50                   	push   %eax
80100251:	e8 ac 0d 00 00       	call   80101002 <itoa>
80100256:	83 c4 08             	add    $0x8,%esp
80100259:	68 f3 11 10 80       	push   $0x801011f3
8010025e:	6a 0f                	push   $0xf
80100260:	e8 0f 0e 00 00       	call   80101074 <write_string>
80100265:	83 c4 08             	add    $0x8,%esp
80100268:	56                   	push   %esi
80100269:	6a 0f                	push   $0xf
8010026b:	e8 04 0e 00 00       	call   80101074 <write_string>
80100270:	83 c4 08             	add    $0x8,%esp
80100273:	68 ff 11 10 80       	push   $0x801011ff
80100278:	6a 0f                	push   $0xf
8010027a:	e8 f5 0d 00 00       	call   80101074 <write_string>
8010027f:	83 c4 0c             	add    $0xc,%esp
80100282:	6a 10                	push   $0x10
80100284:	56                   	push   %esi
80100285:	8b 43 0c             	mov    0xc(%ebx),%eax
80100288:	c1 e0 10             	shl    $0x10,%eax
8010028b:	0b 43 08             	or     0x8(%ebx),%eax
8010028e:	50                   	push   %eax
8010028f:	e8 6e 0d 00 00       	call   80101002 <itoa>
80100294:	83 c4 08             	add    $0x8,%esp
80100297:	56                   	push   %esi
80100298:	6a 0f                	push   $0xf
8010029a:	e8 d5 0d 00 00       	call   80101074 <write_string>
8010029f:	83 c4 08             	add    $0x8,%esp
801002a2:	68 0a 12 10 80       	push   $0x8010120a
801002a7:	6a 0f                	push   $0xf
801002a9:	e8 c6 0d 00 00       	call   80101074 <write_string>
801002ae:	83 c4 10             	add    $0x10,%esp
801002b1:	83 7b 10 05          	cmpl   $0x5,0x10(%ebx)
801002b5:	0f 87 6a ff ff ff    	ja     80100225 <main+0x167>
801002bb:	8b 43 10             	mov    0x10(%ebx),%eax
801002be:	ff 24 85 44 12 10 80 	jmp    *-0x7fefedbc(,%eax,4)
801002c5:	83 ec 08             	sub    $0x8,%esp
801002c8:	68 1b 12 10 80       	push   $0x8010121b
801002cd:	6a 0f                	push   $0xf
801002cf:	e8 a0 0d 00 00       	call   80101074 <write_string>
801002d4:	83 c4 10             	add    $0x10,%esp
801002d7:	e9 49 ff ff ff       	jmp    80100225 <main+0x167>
801002dc:	83 ec 08             	sub    $0x8,%esp
801002df:	68 25 12 10 80       	push   $0x80101225
801002e4:	6a 0f                	push   $0xf
801002e6:	e8 89 0d 00 00       	call   80101074 <write_string>
801002eb:	83 c4 10             	add    $0x10,%esp
801002ee:	e9 32 ff ff ff       	jmp    80100225 <main+0x167>
801002f3:	83 ec 08             	sub    $0x8,%esp
801002f6:	68 32 12 10 80       	push   $0x80101232
801002fb:	6a 0f                	push   $0xf
801002fd:	e8 72 0d 00 00       	call   80101074 <write_string>
80100302:	83 c4 10             	add    $0x10,%esp
80100305:	e9 1b ff ff ff       	jmp    80100225 <main+0x167>
8010030a:	83 ec 08             	sub    $0x8,%esp
8010030d:	68 3c 12 10 80       	push   $0x8010123c
80100312:	6a 0f                	push   $0xf
80100314:	e8 5b 0d 00 00       	call   80101074 <write_string>
80100319:	83 c4 10             	add    $0x10,%esp
8010031c:	e9 04 ff ff ff       	jmp    80100225 <main+0x167>
80100321:	cd 10                	int    $0x10
80100323:	eb fe                	jmp    80100323 <main+0x265>

80100325 <found_tbl_by_va>:
80100325:	8b 44 24 04          	mov    0x4(%esp),%eax
80100329:	c1 e8 16             	shr    $0x16,%eax
8010032c:	c1 e0 02             	shl    $0x2,%eax
8010032f:	f6 80 00 0c 20 80 01 	testb  $0x1,-0x7fdff400(%eax)
80100336:	75 0a                	jne    80100342 <found_tbl_by_va+0x1d>
80100338:	c7 80 00 0c 20 80 00 	movl   $0x80201c00,-0x7fdff400(%eax)
8010033f:	1c 20 80 
80100342:	8b 80 00 0c 20 80    	mov    -0x7fdff400(%eax),%eax
80100348:	c3                   	ret    

80100349 <vm_map>:
80100349:	55                   	push   %ebp
8010034a:	57                   	push   %edi
8010034b:	56                   	push   %esi
8010034c:	53                   	push   %ebx
8010034d:	83 ec 1c             	sub    $0x1c,%esp
80100350:	8b 44 24 30          	mov    0x30(%esp),%eax
80100354:	89 c5                	mov    %eax,%ebp
80100356:	81 e5 00 f0 ff ff    	and    $0xfffff000,%ebp
8010035c:	8b 54 24 34          	mov    0x34(%esp),%edx
80100360:	8d 74 10 ff          	lea    -0x1(%eax,%edx,1),%esi
80100364:	81 e6 00 f0 ff ff    	and    $0xfffff000,%esi
8010036a:	55                   	push   %ebp
8010036b:	e8 b5 ff ff ff       	call   80100325 <found_tbl_by_va>
80100370:	6a 10                	push   $0x10
80100372:	8d 7c 24 08          	lea    0x8(%esp),%edi
80100376:	57                   	push   %edi
80100377:	50                   	push   %eax
80100378:	e8 85 0c 00 00       	call   80101002 <itoa>
8010037d:	83 c4 08             	add    $0x8,%esp
80100380:	57                   	push   %edi
80100381:	6a 0f                	push   $0xf
80100383:	e8 ec 0c 00 00       	call   80101074 <write_string>
80100388:	83 c4 08             	add    $0x8,%esp
8010038b:	68 f1 11 10 80       	push   $0x801011f1
80100390:	6a 0f                	push   $0xf
80100392:	e8 dd 0c 00 00       	call   80101074 <write_string>
80100397:	83 c4 10             	add    $0x10,%esp
8010039a:	39 f5                	cmp    %esi,%ebp
8010039c:	77 25                	ja     801003c3 <vm_map+0x7a>
8010039e:	89 eb                	mov    %ebp,%ebx
801003a0:	8b 7c 24 38          	mov    0x38(%esp),%edi
801003a4:	83 cf 07             	or     $0x7,%edi
801003a7:	29 ee                	sub    %ebp,%esi
801003a9:	83 ec 0c             	sub    $0xc,%esp
801003ac:	53                   	push   %ebx
801003ad:	e8 73 ff ff ff       	call   80100325 <found_tbl_by_va>
801003b2:	89 38                	mov    %edi,(%eax)
801003b4:	81 c3 00 40 00 00    	add    $0x4000,%ebx
801003ba:	83 c4 10             	add    $0x10,%esp
801003bd:	89 d8                	mov    %ebx,%eax
801003bf:	01 f0                	add    %esi,%eax
801003c1:	73 e6                	jae    801003a9 <vm_map+0x60>
801003c3:	83 c4 1c             	add    $0x1c,%esp
801003c6:	5b                   	pop    %ebx
801003c7:	5e                   	pop    %esi
801003c8:	5f                   	pop    %edi
801003c9:	5d                   	pop    %ebp
801003ca:	c3                   	ret    

801003cb <kinit_paging>:
801003cb:	57                   	push   %edi
801003cc:	56                   	push   %esi
801003cd:	53                   	push   %ebx
801003ce:	83 ec 10             	sub    $0x10,%esp
801003d1:	8b 54 24 20          	mov    0x20(%esp),%edx
801003d5:	8b 4c 24 24          	mov    0x24(%esp),%ecx
801003d9:	8d 04 12             	lea    (%edx,%edx,1),%eax
801003dc:	01 d0                	add    %edx,%eax
801003de:	8d 34 c1             	lea    (%ecx,%eax,8),%esi
801003e1:	39 f1                	cmp    %esi,%ecx
801003e3:	73 2b                	jae    80100410 <kinit_paging+0x45>
801003e5:	bb 00 00 00 00       	mov    $0x0,%ebx
801003ea:	eb 07                	jmp    801003f3 <kinit_paging+0x28>
801003ec:	83 c1 18             	add    $0x18,%ecx
801003ef:	39 f1                	cmp    %esi,%ecx
801003f1:	73 22                	jae    80100415 <kinit_paging+0x4a>
801003f3:	8b 41 04             	mov    0x4(%ecx),%eax
801003f6:	c1 e0 10             	shl    $0x10,%eax
801003f9:	0b 01                	or     (%ecx),%eax
801003fb:	89 c2                	mov    %eax,%edx
801003fd:	8b 41 0c             	mov    0xc(%ecx),%eax
80100400:	c1 e0 10             	shl    $0x10,%eax
80100403:	0b 41 08             	or     0x8(%ecx),%eax
80100406:	01 d0                	add    %edx,%eax
80100408:	39 c3                	cmp    %eax,%ebx
8010040a:	73 e0                	jae    801003ec <kinit_paging+0x21>
8010040c:	89 c3                	mov    %eax,%ebx
8010040e:	eb dc                	jmp    801003ec <kinit_paging+0x21>
80100410:	bb 00 00 00 00       	mov    $0x0,%ebx
80100415:	83 ec 08             	sub    $0x8,%esp
80100418:	68 5c 12 10 80       	push   $0x8010125c
8010041d:	6a 0f                	push   $0xf
8010041f:	e8 50 0c 00 00       	call   80101074 <write_string>
80100424:	83 c4 0c             	add    $0xc,%esp
80100427:	6a 0a                	push   $0xa
80100429:	8d 7c 24 08          	lea    0x8(%esp),%edi
8010042d:	57                   	push   %edi
8010042e:	89 d8                	mov    %ebx,%eax
80100430:	c1 e8 14             	shr    $0x14,%eax
80100433:	50                   	push   %eax
80100434:	e8 c9 0b 00 00       	call   80101002 <itoa>
80100439:	83 c4 08             	add    $0x8,%esp
8010043c:	57                   	push   %edi
8010043d:	6a 0f                	push   $0xf
8010043f:	e8 30 0c 00 00       	call   80101074 <write_string>
80100444:	83 c4 08             	add    $0x8,%esp
80100447:	68 6c 12 10 80       	push   $0x8010126c
8010044c:	6a 0f                	push   $0xf
8010044e:	e8 21 0c 00 00       	call   80101074 <write_string>
80100453:	83 c4 0c             	add    $0xc,%esp
80100456:	f7 c3 ff ff 3f 00    	test   $0x3fffff,%ebx
8010045c:	0f 95 c0             	setne  %al
8010045f:	0f b6 c0             	movzbl %al,%eax
80100462:	c1 eb 16             	shr    $0x16,%ebx
80100465:	01 c3                	add    %eax,%ebx
80100467:	89 de                	mov    %ebx,%esi
80100469:	6a 0a                	push   $0xa
8010046b:	57                   	push   %edi
8010046c:	53                   	push   %ebx
8010046d:	e8 90 0b 00 00       	call   80101002 <itoa>
80100472:	83 c4 08             	add    $0x8,%esp
80100475:	68 71 12 10 80       	push   $0x80101271
8010047a:	6a 0f                	push   $0xf
8010047c:	e8 f3 0b 00 00       	call   80101074 <write_string>
80100481:	83 c4 08             	add    $0x8,%esp
80100484:	57                   	push   %edi
80100485:	6a 0f                	push   $0xf
80100487:	e8 e8 0b 00 00       	call   80101074 <write_string>
8010048c:	83 c4 08             	add    $0x8,%esp
8010048f:	68 7a 12 10 80       	push   $0x8010127a
80100494:	6a 0f                	push   $0xf
80100496:	e8 d9 0b 00 00       	call   80101074 <write_string>
8010049b:	83 c4 10             	add    $0x10,%esp
8010049e:	85 db                	test   %ebx,%ebx
801004a0:	74 43                	je     801004e5 <kinit_paging+0x11a>
801004a2:	b9 00 00 00 00       	mov    $0x0,%ecx
801004a7:	ba 07 1c 20 00       	mov    $0x201c07,%edx
801004ac:	b8 00 0c 20 80       	mov    $0x80200c00,%eax
801004b1:	89 10                	mov    %edx,(%eax)
801004b3:	81 c2 00 10 00 00    	add    $0x1000,%edx
801004b9:	41                   	inc    %ecx
801004ba:	8d 40 04             	lea    0x4(%eax),%eax
801004bd:	39 ce                	cmp    %ecx,%esi
801004bf:	75 f0                	jne    801004b1 <kinit_paging+0xe6>
801004c1:	c1 e3 0a             	shl    $0xa,%ebx
801004c4:	74 1f                	je     801004e5 <kinit_paging+0x11a>
801004c6:	b9 00 00 00 00       	mov    $0x0,%ecx
801004cb:	ba 07 00 00 00       	mov    $0x7,%edx
801004d0:	b8 00 1c 20 80       	mov    $0x80201c00,%eax
801004d5:	89 10                	mov    %edx,(%eax)
801004d7:	81 c2 00 10 00 00    	add    $0x1000,%edx
801004dd:	41                   	inc    %ecx
801004de:	8d 40 04             	lea    0x4(%eax),%eax
801004e1:	39 cb                	cmp    %ecx,%ebx
801004e3:	75 f0                	jne    801004d5 <kinit_paging+0x10a>
801004e5:	83 ec 08             	sub    $0x8,%esp
801004e8:	68 98 12 10 80       	push   $0x80101298
801004ed:	6a 0f                	push   $0xf
801004ef:	e8 80 0b 00 00       	call   80101074 <write_string>
801004f4:	83 c4 0c             	add    $0xc,%esp
801004f7:	6a 00                	push   $0x0
801004f9:	68 00 00 00 0f       	push   $0xf000000
801004fe:	68 00 00 00 80       	push   $0x80000000
80100503:	e8 41 fe ff ff       	call   80100349 <vm_map>
80100508:	83 c4 08             	add    $0x8,%esp
8010050b:	68 ad 12 10 80       	push   $0x801012ad
80100510:	6a 0f                	push   $0xf
80100512:	e8 5d 0b 00 00       	call   80101074 <write_string>
80100517:	83 c4 0c             	add    $0xc,%esp
8010051a:	6a 10                	push   $0x10
8010051c:	8d 5c 24 08          	lea    0x8(%esp),%ebx
80100520:	53                   	push   %ebx
80100521:	68 00 0c 20 00       	push   $0x200c00
80100526:	e8 d7 0a 00 00       	call   80101002 <itoa>
8010052b:	83 c4 08             	add    $0x8,%esp
8010052e:	53                   	push   %ebx
8010052f:	6a 0f                	push   $0xf
80100531:	e8 3e 0b 00 00       	call   80101074 <write_string>
80100536:	83 c4 08             	add    $0x8,%esp
80100539:	68 f1 11 10 80       	push   $0x801011f1
8010053e:	6a 0f                	push   $0xf
80100540:	e8 2f 0b 00 00       	call   80101074 <write_string>
80100545:	b8 00 0c 20 00       	mov    $0x200c00,%eax
8010054a:	0f 22 d8             	mov    %eax,%cr3
8010054d:	83 c4 20             	add    $0x20,%esp
80100550:	5b                   	pop    %ebx
80100551:	5e                   	pop    %esi
80100552:	5f                   	pop    %edi
80100553:	c3                   	ret    

80100554 <kinit_gdt>:
80100554:	c3                   	ret    
80100555:	66 90                	xchg   %ax,%ax
80100557:	66 90                	xchg   %ax,%ax
80100559:	66 90                	xchg   %ax,%ax
8010055b:	66 90                	xchg   %ax,%ax
8010055d:	66 90                	xchg   %ax,%ax
8010055f:	90                   	nop

80100560 <vector_handler>:
80100560:	60                   	pusha  
80100561:	1e                   	push   %ds
80100562:	06                   	push   %es
80100563:	0f a0                	push   %fs
80100565:	0f a8                	push   %gs
80100567:	89 d6                	mov    %edx,%esi
80100569:	66 b8 10 00          	mov    $0x10,%ax
8010056d:	8e d8                	mov    %eax,%ds
8010056f:	8e c0                	mov    %eax,%es
80100571:	89 f2                	mov    %esi,%edx
80100573:	54                   	push   %esp
80100574:	e8 af fa ff ff       	call   80100028 <interrupt_handler>
80100579:	83 c4 04             	add    $0x4,%esp

8010057c <vector_handler.vector_handler_ret>:
8010057c:	0f a9                	pop    %gs
8010057e:	0f a1                	pop    %fs
80100580:	07                   	pop    %es
80100581:	1f                   	pop    %ds
80100582:	61                   	popa   
80100583:	83 c4 08             	add    $0x8,%esp
80100586:	cf                   	iret   
80100587:	66 90                	xchg   %ax,%ax
80100589:	66 90                	xchg   %ax,%ax
8010058b:	66 90                	xchg   %ax,%ax
8010058d:	66 90                	xchg   %ax,%ax
8010058f:	90                   	nop

80100590 <vector0>:
80100590:	6a 00                	push   $0x0
80100592:	6a 00                	push   $0x0
80100594:	e9 c7 ff ff ff       	jmp    80100560 <vector_handler>

80100599 <vector1>:
80100599:	6a 00                	push   $0x0
8010059b:	6a 01                	push   $0x1
8010059d:	e9 be ff ff ff       	jmp    80100560 <vector_handler>

801005a2 <vector2>:
801005a2:	6a 00                	push   $0x0
801005a4:	6a 02                	push   $0x2
801005a6:	e9 b5 ff ff ff       	jmp    80100560 <vector_handler>

801005ab <vector3>:
801005ab:	6a 00                	push   $0x0
801005ad:	6a 03                	push   $0x3
801005af:	e9 ac ff ff ff       	jmp    80100560 <vector_handler>

801005b4 <vector4>:
801005b4:	6a 00                	push   $0x0
801005b6:	6a 04                	push   $0x4
801005b8:	e9 a3 ff ff ff       	jmp    80100560 <vector_handler>

801005bd <vector5>:
801005bd:	6a 00                	push   $0x0
801005bf:	6a 05                	push   $0x5
801005c1:	e9 9a ff ff ff       	jmp    80100560 <vector_handler>

801005c6 <vector6>:
801005c6:	6a 00                	push   $0x0
801005c8:	6a 06                	push   $0x6
801005ca:	e9 91 ff ff ff       	jmp    80100560 <vector_handler>

801005cf <vector7>:
801005cf:	6a 00                	push   $0x0
801005d1:	6a 07                	push   $0x7
801005d3:	e9 88 ff ff ff       	jmp    80100560 <vector_handler>

801005d8 <vector8>:
801005d8:	6a 08                	push   $0x8
801005da:	e9 81 ff ff ff       	jmp    80100560 <vector_handler>

801005df <vector9>:
801005df:	6a 00                	push   $0x0
801005e1:	6a 09                	push   $0x9
801005e3:	e9 78 ff ff ff       	jmp    80100560 <vector_handler>

801005e8 <vector10>:
801005e8:	6a 0a                	push   $0xa
801005ea:	e9 71 ff ff ff       	jmp    80100560 <vector_handler>

801005ef <vector11>:
801005ef:	6a 0b                	push   $0xb
801005f1:	e9 6a ff ff ff       	jmp    80100560 <vector_handler>

801005f6 <vector12>:
801005f6:	6a 0c                	push   $0xc
801005f8:	e9 63 ff ff ff       	jmp    80100560 <vector_handler>

801005fd <vector13>:
801005fd:	6a 0d                	push   $0xd
801005ff:	e9 5c ff ff ff       	jmp    80100560 <vector_handler>

80100604 <vector14>:
80100604:	6a 0e                	push   $0xe
80100606:	e9 55 ff ff ff       	jmp    80100560 <vector_handler>

8010060b <vector15>:
8010060b:	6a 00                	push   $0x0
8010060d:	6a 0f                	push   $0xf
8010060f:	e9 4c ff ff ff       	jmp    80100560 <vector_handler>

80100614 <vector16>:
80100614:	6a 00                	push   $0x0
80100616:	6a 10                	push   $0x10
80100618:	e9 43 ff ff ff       	jmp    80100560 <vector_handler>

8010061d <vector17>:
8010061d:	6a 11                	push   $0x11
8010061f:	e9 3c ff ff ff       	jmp    80100560 <vector_handler>

80100624 <vector18>:
80100624:	6a 00                	push   $0x0
80100626:	6a 12                	push   $0x12
80100628:	e9 33 ff ff ff       	jmp    80100560 <vector_handler>

8010062d <vector19>:
8010062d:	6a 00                	push   $0x0
8010062f:	6a 13                	push   $0x13
80100631:	e9 2a ff ff ff       	jmp    80100560 <vector_handler>

80100636 <vector20>:
80100636:	6a 00                	push   $0x0
80100638:	6a 14                	push   $0x14
8010063a:	e9 21 ff ff ff       	jmp    80100560 <vector_handler>

8010063f <vector21>:
8010063f:	6a 00                	push   $0x0
80100641:	6a 15                	push   $0x15
80100643:	e9 18 ff ff ff       	jmp    80100560 <vector_handler>

80100648 <vector22>:
80100648:	6a 00                	push   $0x0
8010064a:	6a 16                	push   $0x16
8010064c:	e9 0f ff ff ff       	jmp    80100560 <vector_handler>

80100651 <vector23>:
80100651:	6a 00                	push   $0x0
80100653:	6a 17                	push   $0x17
80100655:	e9 06 ff ff ff       	jmp    80100560 <vector_handler>

8010065a <vector24>:
8010065a:	6a 00                	push   $0x0
8010065c:	6a 18                	push   $0x18
8010065e:	e9 fd fe ff ff       	jmp    80100560 <vector_handler>

80100663 <vector25>:
80100663:	6a 00                	push   $0x0
80100665:	6a 19                	push   $0x19
80100667:	e9 f4 fe ff ff       	jmp    80100560 <vector_handler>

8010066c <vector26>:
8010066c:	6a 00                	push   $0x0
8010066e:	6a 1a                	push   $0x1a
80100670:	e9 eb fe ff ff       	jmp    80100560 <vector_handler>

80100675 <vector27>:
80100675:	6a 00                	push   $0x0
80100677:	6a 1b                	push   $0x1b
80100679:	e9 e2 fe ff ff       	jmp    80100560 <vector_handler>

8010067e <vector28>:
8010067e:	6a 00                	push   $0x0
80100680:	6a 1c                	push   $0x1c
80100682:	e9 d9 fe ff ff       	jmp    80100560 <vector_handler>

80100687 <vector29>:
80100687:	6a 00                	push   $0x0
80100689:	6a 1d                	push   $0x1d
8010068b:	e9 d0 fe ff ff       	jmp    80100560 <vector_handler>

80100690 <vector30>:
80100690:	6a 00                	push   $0x0
80100692:	6a 1e                	push   $0x1e
80100694:	e9 c7 fe ff ff       	jmp    80100560 <vector_handler>

80100699 <vector31>:
80100699:	6a 00                	push   $0x0
8010069b:	6a 1f                	push   $0x1f
8010069d:	e9 be fe ff ff       	jmp    80100560 <vector_handler>

801006a2 <vector32>:
801006a2:	6a 00                	push   $0x0
801006a4:	6a 20                	push   $0x20
801006a6:	e9 b5 fe ff ff       	jmp    80100560 <vector_handler>

801006ab <vector33>:
801006ab:	6a 00                	push   $0x0
801006ad:	6a 21                	push   $0x21
801006af:	e9 ac fe ff ff       	jmp    80100560 <vector_handler>

801006b4 <vector34>:
801006b4:	6a 00                	push   $0x0
801006b6:	6a 22                	push   $0x22
801006b8:	e9 a3 fe ff ff       	jmp    80100560 <vector_handler>

801006bd <vector35>:
801006bd:	6a 00                	push   $0x0
801006bf:	6a 23                	push   $0x23
801006c1:	e9 9a fe ff ff       	jmp    80100560 <vector_handler>

801006c6 <vector36>:
801006c6:	6a 00                	push   $0x0
801006c8:	6a 24                	push   $0x24
801006ca:	e9 91 fe ff ff       	jmp    80100560 <vector_handler>

801006cf <vector37>:
801006cf:	6a 00                	push   $0x0
801006d1:	6a 25                	push   $0x25
801006d3:	e9 88 fe ff ff       	jmp    80100560 <vector_handler>

801006d8 <vector38>:
801006d8:	6a 00                	push   $0x0
801006da:	6a 26                	push   $0x26
801006dc:	e9 7f fe ff ff       	jmp    80100560 <vector_handler>

801006e1 <vector39>:
801006e1:	6a 00                	push   $0x0
801006e3:	6a 27                	push   $0x27
801006e5:	e9 76 fe ff ff       	jmp    80100560 <vector_handler>

801006ea <vector40>:
801006ea:	6a 00                	push   $0x0
801006ec:	6a 28                	push   $0x28
801006ee:	e9 6d fe ff ff       	jmp    80100560 <vector_handler>

801006f3 <vector41>:
801006f3:	6a 00                	push   $0x0
801006f5:	6a 29                	push   $0x29
801006f7:	e9 64 fe ff ff       	jmp    80100560 <vector_handler>

801006fc <vector42>:
801006fc:	6a 00                	push   $0x0
801006fe:	6a 2a                	push   $0x2a
80100700:	e9 5b fe ff ff       	jmp    80100560 <vector_handler>

80100705 <vector43>:
80100705:	6a 00                	push   $0x0
80100707:	6a 2b                	push   $0x2b
80100709:	e9 52 fe ff ff       	jmp    80100560 <vector_handler>

8010070e <vector44>:
8010070e:	6a 00                	push   $0x0
80100710:	6a 2c                	push   $0x2c
80100712:	e9 49 fe ff ff       	jmp    80100560 <vector_handler>

80100717 <vector45>:
80100717:	6a 00                	push   $0x0
80100719:	6a 2d                	push   $0x2d
8010071b:	e9 40 fe ff ff       	jmp    80100560 <vector_handler>

80100720 <vector46>:
80100720:	6a 00                	push   $0x0
80100722:	6a 2e                	push   $0x2e
80100724:	e9 37 fe ff ff       	jmp    80100560 <vector_handler>

80100729 <vector47>:
80100729:	6a 00                	push   $0x0
8010072b:	6a 2f                	push   $0x2f
8010072d:	e9 2e fe ff ff       	jmp    80100560 <vector_handler>

80100732 <vector48>:
80100732:	6a 00                	push   $0x0
80100734:	6a 30                	push   $0x30
80100736:	e9 25 fe ff ff       	jmp    80100560 <vector_handler>

8010073b <vector49>:
8010073b:	6a 00                	push   $0x0
8010073d:	6a 31                	push   $0x31
8010073f:	e9 1c fe ff ff       	jmp    80100560 <vector_handler>

80100744 <vector50>:
80100744:	6a 00                	push   $0x0
80100746:	6a 32                	push   $0x32
80100748:	e9 13 fe ff ff       	jmp    80100560 <vector_handler>

8010074d <vector51>:
8010074d:	6a 00                	push   $0x0
8010074f:	6a 33                	push   $0x33
80100751:	e9 0a fe ff ff       	jmp    80100560 <vector_handler>

80100756 <vector52>:
80100756:	6a 00                	push   $0x0
80100758:	6a 34                	push   $0x34
8010075a:	e9 01 fe ff ff       	jmp    80100560 <vector_handler>

8010075f <vector53>:
8010075f:	6a 00                	push   $0x0
80100761:	6a 35                	push   $0x35
80100763:	e9 f8 fd ff ff       	jmp    80100560 <vector_handler>

80100768 <vector54>:
80100768:	6a 00                	push   $0x0
8010076a:	6a 36                	push   $0x36
8010076c:	e9 ef fd ff ff       	jmp    80100560 <vector_handler>

80100771 <vector55>:
80100771:	6a 00                	push   $0x0
80100773:	6a 37                	push   $0x37
80100775:	e9 e6 fd ff ff       	jmp    80100560 <vector_handler>

8010077a <vector56>:
8010077a:	6a 00                	push   $0x0
8010077c:	6a 38                	push   $0x38
8010077e:	e9 dd fd ff ff       	jmp    80100560 <vector_handler>

80100783 <vector57>:
80100783:	6a 00                	push   $0x0
80100785:	6a 39                	push   $0x39
80100787:	e9 d4 fd ff ff       	jmp    80100560 <vector_handler>

8010078c <vector58>:
8010078c:	6a 00                	push   $0x0
8010078e:	6a 3a                	push   $0x3a
80100790:	e9 cb fd ff ff       	jmp    80100560 <vector_handler>

80100795 <vector59>:
80100795:	6a 00                	push   $0x0
80100797:	6a 3b                	push   $0x3b
80100799:	e9 c2 fd ff ff       	jmp    80100560 <vector_handler>

8010079e <vector60>:
8010079e:	6a 00                	push   $0x0
801007a0:	6a 3c                	push   $0x3c
801007a2:	e9 b9 fd ff ff       	jmp    80100560 <vector_handler>

801007a7 <vector61>:
801007a7:	6a 00                	push   $0x0
801007a9:	6a 3d                	push   $0x3d
801007ab:	e9 b0 fd ff ff       	jmp    80100560 <vector_handler>

801007b0 <vector62>:
801007b0:	6a 00                	push   $0x0
801007b2:	6a 3e                	push   $0x3e
801007b4:	e9 a7 fd ff ff       	jmp    80100560 <vector_handler>

801007b9 <vector63>:
801007b9:	6a 00                	push   $0x0
801007bb:	6a 3f                	push   $0x3f
801007bd:	e9 9e fd ff ff       	jmp    80100560 <vector_handler>

801007c2 <vector64>:
801007c2:	6a 00                	push   $0x0
801007c4:	6a 40                	push   $0x40
801007c6:	e9 95 fd ff ff       	jmp    80100560 <vector_handler>

801007cb <vector65>:
801007cb:	6a 00                	push   $0x0
801007cd:	6a 41                	push   $0x41
801007cf:	e9 8c fd ff ff       	jmp    80100560 <vector_handler>

801007d4 <vector66>:
801007d4:	6a 00                	push   $0x0
801007d6:	6a 42                	push   $0x42
801007d8:	e9 83 fd ff ff       	jmp    80100560 <vector_handler>

801007dd <vector67>:
801007dd:	6a 00                	push   $0x0
801007df:	6a 43                	push   $0x43
801007e1:	e9 7a fd ff ff       	jmp    80100560 <vector_handler>

801007e6 <vector68>:
801007e6:	6a 00                	push   $0x0
801007e8:	6a 44                	push   $0x44
801007ea:	e9 71 fd ff ff       	jmp    80100560 <vector_handler>

801007ef <vector69>:
801007ef:	6a 00                	push   $0x0
801007f1:	6a 45                	push   $0x45
801007f3:	e9 68 fd ff ff       	jmp    80100560 <vector_handler>

801007f8 <vector70>:
801007f8:	6a 00                	push   $0x0
801007fa:	6a 46                	push   $0x46
801007fc:	e9 5f fd ff ff       	jmp    80100560 <vector_handler>

80100801 <vector71>:
80100801:	6a 00                	push   $0x0
80100803:	6a 47                	push   $0x47
80100805:	e9 56 fd ff ff       	jmp    80100560 <vector_handler>

8010080a <vector72>:
8010080a:	6a 00                	push   $0x0
8010080c:	6a 48                	push   $0x48
8010080e:	e9 4d fd ff ff       	jmp    80100560 <vector_handler>

80100813 <vector73>:
80100813:	6a 00                	push   $0x0
80100815:	6a 49                	push   $0x49
80100817:	e9 44 fd ff ff       	jmp    80100560 <vector_handler>

8010081c <vector74>:
8010081c:	6a 00                	push   $0x0
8010081e:	6a 4a                	push   $0x4a
80100820:	e9 3b fd ff ff       	jmp    80100560 <vector_handler>

80100825 <vector75>:
80100825:	6a 00                	push   $0x0
80100827:	6a 4b                	push   $0x4b
80100829:	e9 32 fd ff ff       	jmp    80100560 <vector_handler>

8010082e <vector76>:
8010082e:	6a 00                	push   $0x0
80100830:	6a 4c                	push   $0x4c
80100832:	e9 29 fd ff ff       	jmp    80100560 <vector_handler>

80100837 <vector77>:
80100837:	6a 00                	push   $0x0
80100839:	6a 4d                	push   $0x4d
8010083b:	e9 20 fd ff ff       	jmp    80100560 <vector_handler>

80100840 <vector78>:
80100840:	6a 00                	push   $0x0
80100842:	6a 4e                	push   $0x4e
80100844:	e9 17 fd ff ff       	jmp    80100560 <vector_handler>

80100849 <vector79>:
80100849:	6a 00                	push   $0x0
8010084b:	6a 4f                	push   $0x4f
8010084d:	e9 0e fd ff ff       	jmp    80100560 <vector_handler>

80100852 <vector80>:
80100852:	6a 00                	push   $0x0
80100854:	6a 50                	push   $0x50
80100856:	e9 05 fd ff ff       	jmp    80100560 <vector_handler>

8010085b <vector81>:
8010085b:	6a 00                	push   $0x0
8010085d:	6a 51                	push   $0x51
8010085f:	e9 fc fc ff ff       	jmp    80100560 <vector_handler>

80100864 <vector82>:
80100864:	6a 00                	push   $0x0
80100866:	6a 52                	push   $0x52
80100868:	e9 f3 fc ff ff       	jmp    80100560 <vector_handler>

8010086d <vector83>:
8010086d:	6a 00                	push   $0x0
8010086f:	6a 53                	push   $0x53
80100871:	e9 ea fc ff ff       	jmp    80100560 <vector_handler>

80100876 <vector84>:
80100876:	6a 00                	push   $0x0
80100878:	6a 54                	push   $0x54
8010087a:	e9 e1 fc ff ff       	jmp    80100560 <vector_handler>

8010087f <vector85>:
8010087f:	6a 00                	push   $0x0
80100881:	6a 55                	push   $0x55
80100883:	e9 d8 fc ff ff       	jmp    80100560 <vector_handler>

80100888 <vector86>:
80100888:	6a 00                	push   $0x0
8010088a:	6a 56                	push   $0x56
8010088c:	e9 cf fc ff ff       	jmp    80100560 <vector_handler>

80100891 <vector87>:
80100891:	6a 00                	push   $0x0
80100893:	6a 57                	push   $0x57
80100895:	e9 c6 fc ff ff       	jmp    80100560 <vector_handler>

8010089a <vector88>:
8010089a:	6a 00                	push   $0x0
8010089c:	6a 58                	push   $0x58
8010089e:	e9 bd fc ff ff       	jmp    80100560 <vector_handler>

801008a3 <vector89>:
801008a3:	6a 00                	push   $0x0
801008a5:	6a 59                	push   $0x59
801008a7:	e9 b4 fc ff ff       	jmp    80100560 <vector_handler>

801008ac <vector90>:
801008ac:	6a 00                	push   $0x0
801008ae:	6a 5a                	push   $0x5a
801008b0:	e9 ab fc ff ff       	jmp    80100560 <vector_handler>

801008b5 <vector91>:
801008b5:	6a 00                	push   $0x0
801008b7:	6a 5b                	push   $0x5b
801008b9:	e9 a2 fc ff ff       	jmp    80100560 <vector_handler>

801008be <vector92>:
801008be:	6a 00                	push   $0x0
801008c0:	6a 5c                	push   $0x5c
801008c2:	e9 99 fc ff ff       	jmp    80100560 <vector_handler>

801008c7 <vector93>:
801008c7:	6a 00                	push   $0x0
801008c9:	6a 5d                	push   $0x5d
801008cb:	e9 90 fc ff ff       	jmp    80100560 <vector_handler>

801008d0 <vector94>:
801008d0:	6a 00                	push   $0x0
801008d2:	6a 5e                	push   $0x5e
801008d4:	e9 87 fc ff ff       	jmp    80100560 <vector_handler>

801008d9 <vector95>:
801008d9:	6a 00                	push   $0x0
801008db:	6a 5f                	push   $0x5f
801008dd:	e9 7e fc ff ff       	jmp    80100560 <vector_handler>

801008e2 <vector96>:
801008e2:	6a 00                	push   $0x0
801008e4:	6a 60                	push   $0x60
801008e6:	e9 75 fc ff ff       	jmp    80100560 <vector_handler>

801008eb <vector97>:
801008eb:	6a 00                	push   $0x0
801008ed:	6a 61                	push   $0x61
801008ef:	e9 6c fc ff ff       	jmp    80100560 <vector_handler>

801008f4 <vector98>:
801008f4:	6a 00                	push   $0x0
801008f6:	6a 62                	push   $0x62
801008f8:	e9 63 fc ff ff       	jmp    80100560 <vector_handler>

801008fd <vector99>:
801008fd:	6a 00                	push   $0x0
801008ff:	6a 63                	push   $0x63
80100901:	e9 5a fc ff ff       	jmp    80100560 <vector_handler>

80100906 <vector100>:
80100906:	6a 00                	push   $0x0
80100908:	6a 64                	push   $0x64
8010090a:	e9 51 fc ff ff       	jmp    80100560 <vector_handler>

8010090f <vector101>:
8010090f:	6a 00                	push   $0x0
80100911:	6a 65                	push   $0x65
80100913:	e9 48 fc ff ff       	jmp    80100560 <vector_handler>

80100918 <vector102>:
80100918:	6a 00                	push   $0x0
8010091a:	6a 66                	push   $0x66
8010091c:	e9 3f fc ff ff       	jmp    80100560 <vector_handler>

80100921 <vector103>:
80100921:	6a 00                	push   $0x0
80100923:	6a 67                	push   $0x67
80100925:	e9 36 fc ff ff       	jmp    80100560 <vector_handler>

8010092a <vector104>:
8010092a:	6a 00                	push   $0x0
8010092c:	6a 68                	push   $0x68
8010092e:	e9 2d fc ff ff       	jmp    80100560 <vector_handler>

80100933 <vector105>:
80100933:	6a 00                	push   $0x0
80100935:	6a 69                	push   $0x69
80100937:	e9 24 fc ff ff       	jmp    80100560 <vector_handler>

8010093c <vector106>:
8010093c:	6a 00                	push   $0x0
8010093e:	6a 6a                	push   $0x6a
80100940:	e9 1b fc ff ff       	jmp    80100560 <vector_handler>

80100945 <vector107>:
80100945:	6a 00                	push   $0x0
80100947:	6a 6b                	push   $0x6b
80100949:	e9 12 fc ff ff       	jmp    80100560 <vector_handler>

8010094e <vector108>:
8010094e:	6a 00                	push   $0x0
80100950:	6a 6c                	push   $0x6c
80100952:	e9 09 fc ff ff       	jmp    80100560 <vector_handler>

80100957 <vector109>:
80100957:	6a 00                	push   $0x0
80100959:	6a 6d                	push   $0x6d
8010095b:	e9 00 fc ff ff       	jmp    80100560 <vector_handler>

80100960 <vector110>:
80100960:	6a 00                	push   $0x0
80100962:	6a 6e                	push   $0x6e
80100964:	e9 f7 fb ff ff       	jmp    80100560 <vector_handler>

80100969 <vector111>:
80100969:	6a 00                	push   $0x0
8010096b:	6a 6f                	push   $0x6f
8010096d:	e9 ee fb ff ff       	jmp    80100560 <vector_handler>

80100972 <vector112>:
80100972:	6a 00                	push   $0x0
80100974:	6a 70                	push   $0x70
80100976:	e9 e5 fb ff ff       	jmp    80100560 <vector_handler>

8010097b <vector113>:
8010097b:	6a 00                	push   $0x0
8010097d:	6a 71                	push   $0x71
8010097f:	e9 dc fb ff ff       	jmp    80100560 <vector_handler>

80100984 <vector114>:
80100984:	6a 00                	push   $0x0
80100986:	6a 72                	push   $0x72
80100988:	e9 d3 fb ff ff       	jmp    80100560 <vector_handler>

8010098d <vector115>:
8010098d:	6a 00                	push   $0x0
8010098f:	6a 73                	push   $0x73
80100991:	e9 ca fb ff ff       	jmp    80100560 <vector_handler>

80100996 <vector116>:
80100996:	6a 00                	push   $0x0
80100998:	6a 74                	push   $0x74
8010099a:	e9 c1 fb ff ff       	jmp    80100560 <vector_handler>

8010099f <vector117>:
8010099f:	6a 00                	push   $0x0
801009a1:	6a 75                	push   $0x75
801009a3:	e9 b8 fb ff ff       	jmp    80100560 <vector_handler>

801009a8 <vector118>:
801009a8:	6a 00                	push   $0x0
801009aa:	6a 76                	push   $0x76
801009ac:	e9 af fb ff ff       	jmp    80100560 <vector_handler>

801009b1 <vector119>:
801009b1:	6a 00                	push   $0x0
801009b3:	6a 77                	push   $0x77
801009b5:	e9 a6 fb ff ff       	jmp    80100560 <vector_handler>

801009ba <vector120>:
801009ba:	6a 00                	push   $0x0
801009bc:	6a 78                	push   $0x78
801009be:	e9 9d fb ff ff       	jmp    80100560 <vector_handler>

801009c3 <vector121>:
801009c3:	6a 00                	push   $0x0
801009c5:	6a 79                	push   $0x79
801009c7:	e9 94 fb ff ff       	jmp    80100560 <vector_handler>

801009cc <vector122>:
801009cc:	6a 00                	push   $0x0
801009ce:	6a 7a                	push   $0x7a
801009d0:	e9 8b fb ff ff       	jmp    80100560 <vector_handler>

801009d5 <vector123>:
801009d5:	6a 00                	push   $0x0
801009d7:	6a 7b                	push   $0x7b
801009d9:	e9 82 fb ff ff       	jmp    80100560 <vector_handler>

801009de <vector124>:
801009de:	6a 00                	push   $0x0
801009e0:	6a 7c                	push   $0x7c
801009e2:	e9 79 fb ff ff       	jmp    80100560 <vector_handler>

801009e7 <vector125>:
801009e7:	6a 00                	push   $0x0
801009e9:	6a 7d                	push   $0x7d
801009eb:	e9 70 fb ff ff       	jmp    80100560 <vector_handler>

801009f0 <vector126>:
801009f0:	6a 00                	push   $0x0
801009f2:	6a 7e                	push   $0x7e
801009f4:	e9 67 fb ff ff       	jmp    80100560 <vector_handler>

801009f9 <vector127>:
801009f9:	6a 00                	push   $0x0
801009fb:	6a 7f                	push   $0x7f
801009fd:	e9 5e fb ff ff       	jmp    80100560 <vector_handler>

80100a02 <vector128>:
80100a02:	6a 00                	push   $0x0
80100a04:	68 80 00 00 00       	push   $0x80
80100a09:	e9 52 fb ff ff       	jmp    80100560 <vector_handler>

80100a0e <vector129>:
80100a0e:	6a 00                	push   $0x0
80100a10:	68 81 00 00 00       	push   $0x81
80100a15:	e9 46 fb ff ff       	jmp    80100560 <vector_handler>

80100a1a <vector130>:
80100a1a:	6a 00                	push   $0x0
80100a1c:	68 82 00 00 00       	push   $0x82
80100a21:	e9 3a fb ff ff       	jmp    80100560 <vector_handler>

80100a26 <vector131>:
80100a26:	6a 00                	push   $0x0
80100a28:	68 83 00 00 00       	push   $0x83
80100a2d:	e9 2e fb ff ff       	jmp    80100560 <vector_handler>

80100a32 <vector132>:
80100a32:	6a 00                	push   $0x0
80100a34:	68 84 00 00 00       	push   $0x84
80100a39:	e9 22 fb ff ff       	jmp    80100560 <vector_handler>

80100a3e <vector133>:
80100a3e:	6a 00                	push   $0x0
80100a40:	68 85 00 00 00       	push   $0x85
80100a45:	e9 16 fb ff ff       	jmp    80100560 <vector_handler>

80100a4a <vector134>:
80100a4a:	6a 00                	push   $0x0
80100a4c:	68 86 00 00 00       	push   $0x86
80100a51:	e9 0a fb ff ff       	jmp    80100560 <vector_handler>

80100a56 <vector135>:
80100a56:	6a 00                	push   $0x0
80100a58:	68 87 00 00 00       	push   $0x87
80100a5d:	e9 fe fa ff ff       	jmp    80100560 <vector_handler>

80100a62 <vector136>:
80100a62:	6a 00                	push   $0x0
80100a64:	68 88 00 00 00       	push   $0x88
80100a69:	e9 f2 fa ff ff       	jmp    80100560 <vector_handler>

80100a6e <vector137>:
80100a6e:	6a 00                	push   $0x0
80100a70:	68 89 00 00 00       	push   $0x89
80100a75:	e9 e6 fa ff ff       	jmp    80100560 <vector_handler>

80100a7a <vector138>:
80100a7a:	6a 00                	push   $0x0
80100a7c:	68 8a 00 00 00       	push   $0x8a
80100a81:	e9 da fa ff ff       	jmp    80100560 <vector_handler>

80100a86 <vector139>:
80100a86:	6a 00                	push   $0x0
80100a88:	68 8b 00 00 00       	push   $0x8b
80100a8d:	e9 ce fa ff ff       	jmp    80100560 <vector_handler>

80100a92 <vector140>:
80100a92:	6a 00                	push   $0x0
80100a94:	68 8c 00 00 00       	push   $0x8c
80100a99:	e9 c2 fa ff ff       	jmp    80100560 <vector_handler>

80100a9e <vector141>:
80100a9e:	6a 00                	push   $0x0
80100aa0:	68 8d 00 00 00       	push   $0x8d
80100aa5:	e9 b6 fa ff ff       	jmp    80100560 <vector_handler>

80100aaa <vector142>:
80100aaa:	6a 00                	push   $0x0
80100aac:	68 8e 00 00 00       	push   $0x8e
80100ab1:	e9 aa fa ff ff       	jmp    80100560 <vector_handler>

80100ab6 <vector143>:
80100ab6:	6a 00                	push   $0x0
80100ab8:	68 8f 00 00 00       	push   $0x8f
80100abd:	e9 9e fa ff ff       	jmp    80100560 <vector_handler>

80100ac2 <vector144>:
80100ac2:	6a 00                	push   $0x0
80100ac4:	68 90 00 00 00       	push   $0x90
80100ac9:	e9 92 fa ff ff       	jmp    80100560 <vector_handler>

80100ace <vector145>:
80100ace:	6a 00                	push   $0x0
80100ad0:	68 91 00 00 00       	push   $0x91
80100ad5:	e9 86 fa ff ff       	jmp    80100560 <vector_handler>

80100ada <vector146>:
80100ada:	6a 00                	push   $0x0
80100adc:	68 92 00 00 00       	push   $0x92
80100ae1:	e9 7a fa ff ff       	jmp    80100560 <vector_handler>

80100ae6 <vector147>:
80100ae6:	6a 00                	push   $0x0
80100ae8:	68 93 00 00 00       	push   $0x93
80100aed:	e9 6e fa ff ff       	jmp    80100560 <vector_handler>

80100af2 <vector148>:
80100af2:	6a 00                	push   $0x0
80100af4:	68 94 00 00 00       	push   $0x94
80100af9:	e9 62 fa ff ff       	jmp    80100560 <vector_handler>

80100afe <vector149>:
80100afe:	6a 00                	push   $0x0
80100b00:	68 95 00 00 00       	push   $0x95
80100b05:	e9 56 fa ff ff       	jmp    80100560 <vector_handler>

80100b0a <vector150>:
80100b0a:	6a 00                	push   $0x0
80100b0c:	68 96 00 00 00       	push   $0x96
80100b11:	e9 4a fa ff ff       	jmp    80100560 <vector_handler>

80100b16 <vector151>:
80100b16:	6a 00                	push   $0x0
80100b18:	68 97 00 00 00       	push   $0x97
80100b1d:	e9 3e fa ff ff       	jmp    80100560 <vector_handler>

80100b22 <vector152>:
80100b22:	6a 00                	push   $0x0
80100b24:	68 98 00 00 00       	push   $0x98
80100b29:	e9 32 fa ff ff       	jmp    80100560 <vector_handler>

80100b2e <vector153>:
80100b2e:	6a 00                	push   $0x0
80100b30:	68 99 00 00 00       	push   $0x99
80100b35:	e9 26 fa ff ff       	jmp    80100560 <vector_handler>

80100b3a <vector154>:
80100b3a:	6a 00                	push   $0x0
80100b3c:	68 9a 00 00 00       	push   $0x9a
80100b41:	e9 1a fa ff ff       	jmp    80100560 <vector_handler>

80100b46 <vector155>:
80100b46:	6a 00                	push   $0x0
80100b48:	68 9b 00 00 00       	push   $0x9b
80100b4d:	e9 0e fa ff ff       	jmp    80100560 <vector_handler>

80100b52 <vector156>:
80100b52:	6a 00                	push   $0x0
80100b54:	68 9c 00 00 00       	push   $0x9c
80100b59:	e9 02 fa ff ff       	jmp    80100560 <vector_handler>

80100b5e <vector157>:
80100b5e:	6a 00                	push   $0x0
80100b60:	68 9d 00 00 00       	push   $0x9d
80100b65:	e9 f6 f9 ff ff       	jmp    80100560 <vector_handler>

80100b6a <vector158>:
80100b6a:	6a 00                	push   $0x0
80100b6c:	68 9e 00 00 00       	push   $0x9e
80100b71:	e9 ea f9 ff ff       	jmp    80100560 <vector_handler>

80100b76 <vector159>:
80100b76:	6a 00                	push   $0x0
80100b78:	68 9f 00 00 00       	push   $0x9f
80100b7d:	e9 de f9 ff ff       	jmp    80100560 <vector_handler>

80100b82 <vector160>:
80100b82:	6a 00                	push   $0x0
80100b84:	68 a0 00 00 00       	push   $0xa0
80100b89:	e9 d2 f9 ff ff       	jmp    80100560 <vector_handler>

80100b8e <vector161>:
80100b8e:	6a 00                	push   $0x0
80100b90:	68 a1 00 00 00       	push   $0xa1
80100b95:	e9 c6 f9 ff ff       	jmp    80100560 <vector_handler>

80100b9a <vector162>:
80100b9a:	6a 00                	push   $0x0
80100b9c:	68 a2 00 00 00       	push   $0xa2
80100ba1:	e9 ba f9 ff ff       	jmp    80100560 <vector_handler>

80100ba6 <vector163>:
80100ba6:	6a 00                	push   $0x0
80100ba8:	68 a3 00 00 00       	push   $0xa3
80100bad:	e9 ae f9 ff ff       	jmp    80100560 <vector_handler>

80100bb2 <vector164>:
80100bb2:	6a 00                	push   $0x0
80100bb4:	68 a4 00 00 00       	push   $0xa4
80100bb9:	e9 a2 f9 ff ff       	jmp    80100560 <vector_handler>

80100bbe <vector165>:
80100bbe:	6a 00                	push   $0x0
80100bc0:	68 a5 00 00 00       	push   $0xa5
80100bc5:	e9 96 f9 ff ff       	jmp    80100560 <vector_handler>

80100bca <vector166>:
80100bca:	6a 00                	push   $0x0
80100bcc:	68 a6 00 00 00       	push   $0xa6
80100bd1:	e9 8a f9 ff ff       	jmp    80100560 <vector_handler>

80100bd6 <vector167>:
80100bd6:	6a 00                	push   $0x0
80100bd8:	68 a7 00 00 00       	push   $0xa7
80100bdd:	e9 7e f9 ff ff       	jmp    80100560 <vector_handler>

80100be2 <vector168>:
80100be2:	6a 00                	push   $0x0
80100be4:	68 a8 00 00 00       	push   $0xa8
80100be9:	e9 72 f9 ff ff       	jmp    80100560 <vector_handler>

80100bee <vector169>:
80100bee:	6a 00                	push   $0x0
80100bf0:	68 a9 00 00 00       	push   $0xa9
80100bf5:	e9 66 f9 ff ff       	jmp    80100560 <vector_handler>

80100bfa <vector170>:
80100bfa:	6a 00                	push   $0x0
80100bfc:	68 aa 00 00 00       	push   $0xaa
80100c01:	e9 5a f9 ff ff       	jmp    80100560 <vector_handler>

80100c06 <vector171>:
80100c06:	6a 00                	push   $0x0
80100c08:	68 ab 00 00 00       	push   $0xab
80100c0d:	e9 4e f9 ff ff       	jmp    80100560 <vector_handler>

80100c12 <vector172>:
80100c12:	6a 00                	push   $0x0
80100c14:	68 ac 00 00 00       	push   $0xac
80100c19:	e9 42 f9 ff ff       	jmp    80100560 <vector_handler>

80100c1e <vector173>:
80100c1e:	6a 00                	push   $0x0
80100c20:	68 ad 00 00 00       	push   $0xad
80100c25:	e9 36 f9 ff ff       	jmp    80100560 <vector_handler>

80100c2a <vector174>:
80100c2a:	6a 00                	push   $0x0
80100c2c:	68 ae 00 00 00       	push   $0xae
80100c31:	e9 2a f9 ff ff       	jmp    80100560 <vector_handler>

80100c36 <vector175>:
80100c36:	6a 00                	push   $0x0
80100c38:	68 af 00 00 00       	push   $0xaf
80100c3d:	e9 1e f9 ff ff       	jmp    80100560 <vector_handler>

80100c42 <vector176>:
80100c42:	6a 00                	push   $0x0
80100c44:	68 b0 00 00 00       	push   $0xb0
80100c49:	e9 12 f9 ff ff       	jmp    80100560 <vector_handler>

80100c4e <vector177>:
80100c4e:	6a 00                	push   $0x0
80100c50:	68 b1 00 00 00       	push   $0xb1
80100c55:	e9 06 f9 ff ff       	jmp    80100560 <vector_handler>

80100c5a <vector178>:
80100c5a:	6a 00                	push   $0x0
80100c5c:	68 b2 00 00 00       	push   $0xb2
80100c61:	e9 fa f8 ff ff       	jmp    80100560 <vector_handler>

80100c66 <vector179>:
80100c66:	6a 00                	push   $0x0
80100c68:	68 b3 00 00 00       	push   $0xb3
80100c6d:	e9 ee f8 ff ff       	jmp    80100560 <vector_handler>

80100c72 <vector180>:
80100c72:	6a 00                	push   $0x0
80100c74:	68 b4 00 00 00       	push   $0xb4
80100c79:	e9 e2 f8 ff ff       	jmp    80100560 <vector_handler>

80100c7e <vector181>:
80100c7e:	6a 00                	push   $0x0
80100c80:	68 b5 00 00 00       	push   $0xb5
80100c85:	e9 d6 f8 ff ff       	jmp    80100560 <vector_handler>

80100c8a <vector182>:
80100c8a:	6a 00                	push   $0x0
80100c8c:	68 b6 00 00 00       	push   $0xb6
80100c91:	e9 ca f8 ff ff       	jmp    80100560 <vector_handler>

80100c96 <vector183>:
80100c96:	6a 00                	push   $0x0
80100c98:	68 b7 00 00 00       	push   $0xb7
80100c9d:	e9 be f8 ff ff       	jmp    80100560 <vector_handler>

80100ca2 <vector184>:
80100ca2:	6a 00                	push   $0x0
80100ca4:	68 b8 00 00 00       	push   $0xb8
80100ca9:	e9 b2 f8 ff ff       	jmp    80100560 <vector_handler>

80100cae <vector185>:
80100cae:	6a 00                	push   $0x0
80100cb0:	68 b9 00 00 00       	push   $0xb9
80100cb5:	e9 a6 f8 ff ff       	jmp    80100560 <vector_handler>

80100cba <vector186>:
80100cba:	6a 00                	push   $0x0
80100cbc:	68 ba 00 00 00       	push   $0xba
80100cc1:	e9 9a f8 ff ff       	jmp    80100560 <vector_handler>

80100cc6 <vector187>:
80100cc6:	6a 00                	push   $0x0
80100cc8:	68 bb 00 00 00       	push   $0xbb
80100ccd:	e9 8e f8 ff ff       	jmp    80100560 <vector_handler>

80100cd2 <vector188>:
80100cd2:	6a 00                	push   $0x0
80100cd4:	68 bc 00 00 00       	push   $0xbc
80100cd9:	e9 82 f8 ff ff       	jmp    80100560 <vector_handler>

80100cde <vector189>:
80100cde:	6a 00                	push   $0x0
80100ce0:	68 bd 00 00 00       	push   $0xbd
80100ce5:	e9 76 f8 ff ff       	jmp    80100560 <vector_handler>

80100cea <vector190>:
80100cea:	6a 00                	push   $0x0
80100cec:	68 be 00 00 00       	push   $0xbe
80100cf1:	e9 6a f8 ff ff       	jmp    80100560 <vector_handler>

80100cf6 <vector191>:
80100cf6:	6a 00                	push   $0x0
80100cf8:	68 bf 00 00 00       	push   $0xbf
80100cfd:	e9 5e f8 ff ff       	jmp    80100560 <vector_handler>

80100d02 <vector192>:
80100d02:	6a 00                	push   $0x0
80100d04:	68 c0 00 00 00       	push   $0xc0
80100d09:	e9 52 f8 ff ff       	jmp    80100560 <vector_handler>

80100d0e <vector193>:
80100d0e:	6a 00                	push   $0x0
80100d10:	68 c1 00 00 00       	push   $0xc1
80100d15:	e9 46 f8 ff ff       	jmp    80100560 <vector_handler>

80100d1a <vector194>:
80100d1a:	6a 00                	push   $0x0
80100d1c:	68 c2 00 00 00       	push   $0xc2
80100d21:	e9 3a f8 ff ff       	jmp    80100560 <vector_handler>

80100d26 <vector195>:
80100d26:	6a 00                	push   $0x0
80100d28:	68 c3 00 00 00       	push   $0xc3
80100d2d:	e9 2e f8 ff ff       	jmp    80100560 <vector_handler>

80100d32 <vector196>:
80100d32:	6a 00                	push   $0x0
80100d34:	68 c4 00 00 00       	push   $0xc4
80100d39:	e9 22 f8 ff ff       	jmp    80100560 <vector_handler>

80100d3e <vector197>:
80100d3e:	6a 00                	push   $0x0
80100d40:	68 c5 00 00 00       	push   $0xc5
80100d45:	e9 16 f8 ff ff       	jmp    80100560 <vector_handler>

80100d4a <vector198>:
80100d4a:	6a 00                	push   $0x0
80100d4c:	68 c6 00 00 00       	push   $0xc6
80100d51:	e9 0a f8 ff ff       	jmp    80100560 <vector_handler>

80100d56 <vector199>:
80100d56:	6a 00                	push   $0x0
80100d58:	68 c7 00 00 00       	push   $0xc7
80100d5d:	e9 fe f7 ff ff       	jmp    80100560 <vector_handler>

80100d62 <vector200>:
80100d62:	6a 00                	push   $0x0
80100d64:	68 c8 00 00 00       	push   $0xc8
80100d69:	e9 f2 f7 ff ff       	jmp    80100560 <vector_handler>

80100d6e <vector201>:
80100d6e:	6a 00                	push   $0x0
80100d70:	68 c9 00 00 00       	push   $0xc9
80100d75:	e9 e6 f7 ff ff       	jmp    80100560 <vector_handler>

80100d7a <vector202>:
80100d7a:	6a 00                	push   $0x0
80100d7c:	68 ca 00 00 00       	push   $0xca
80100d81:	e9 da f7 ff ff       	jmp    80100560 <vector_handler>

80100d86 <vector203>:
80100d86:	6a 00                	push   $0x0
80100d88:	68 cb 00 00 00       	push   $0xcb
80100d8d:	e9 ce f7 ff ff       	jmp    80100560 <vector_handler>

80100d92 <vector204>:
80100d92:	6a 00                	push   $0x0
80100d94:	68 cc 00 00 00       	push   $0xcc
80100d99:	e9 c2 f7 ff ff       	jmp    80100560 <vector_handler>

80100d9e <vector205>:
80100d9e:	6a 00                	push   $0x0
80100da0:	68 cd 00 00 00       	push   $0xcd
80100da5:	e9 b6 f7 ff ff       	jmp    80100560 <vector_handler>

80100daa <vector206>:
80100daa:	6a 00                	push   $0x0
80100dac:	68 ce 00 00 00       	push   $0xce
80100db1:	e9 aa f7 ff ff       	jmp    80100560 <vector_handler>

80100db6 <vector207>:
80100db6:	6a 00                	push   $0x0
80100db8:	68 cf 00 00 00       	push   $0xcf
80100dbd:	e9 9e f7 ff ff       	jmp    80100560 <vector_handler>

80100dc2 <vector208>:
80100dc2:	6a 00                	push   $0x0
80100dc4:	68 d0 00 00 00       	push   $0xd0
80100dc9:	e9 92 f7 ff ff       	jmp    80100560 <vector_handler>

80100dce <vector209>:
80100dce:	6a 00                	push   $0x0
80100dd0:	68 d1 00 00 00       	push   $0xd1
80100dd5:	e9 86 f7 ff ff       	jmp    80100560 <vector_handler>

80100dda <vector210>:
80100dda:	6a 00                	push   $0x0
80100ddc:	68 d2 00 00 00       	push   $0xd2
80100de1:	e9 7a f7 ff ff       	jmp    80100560 <vector_handler>

80100de6 <vector211>:
80100de6:	6a 00                	push   $0x0
80100de8:	68 d3 00 00 00       	push   $0xd3
80100ded:	e9 6e f7 ff ff       	jmp    80100560 <vector_handler>

80100df2 <vector212>:
80100df2:	6a 00                	push   $0x0
80100df4:	68 d4 00 00 00       	push   $0xd4
80100df9:	e9 62 f7 ff ff       	jmp    80100560 <vector_handler>

80100dfe <vector213>:
80100dfe:	6a 00                	push   $0x0
80100e00:	68 d5 00 00 00       	push   $0xd5
80100e05:	e9 56 f7 ff ff       	jmp    80100560 <vector_handler>

80100e0a <vector214>:
80100e0a:	6a 00                	push   $0x0
80100e0c:	68 d6 00 00 00       	push   $0xd6
80100e11:	e9 4a f7 ff ff       	jmp    80100560 <vector_handler>

80100e16 <vector215>:
80100e16:	6a 00                	push   $0x0
80100e18:	68 d7 00 00 00       	push   $0xd7
80100e1d:	e9 3e f7 ff ff       	jmp    80100560 <vector_handler>

80100e22 <vector216>:
80100e22:	6a 00                	push   $0x0
80100e24:	68 d8 00 00 00       	push   $0xd8
80100e29:	e9 32 f7 ff ff       	jmp    80100560 <vector_handler>

80100e2e <vector217>:
80100e2e:	6a 00                	push   $0x0
80100e30:	68 d9 00 00 00       	push   $0xd9
80100e35:	e9 26 f7 ff ff       	jmp    80100560 <vector_handler>

80100e3a <vector218>:
80100e3a:	6a 00                	push   $0x0
80100e3c:	68 da 00 00 00       	push   $0xda
80100e41:	e9 1a f7 ff ff       	jmp    80100560 <vector_handler>

80100e46 <vector219>:
80100e46:	6a 00                	push   $0x0
80100e48:	68 db 00 00 00       	push   $0xdb
80100e4d:	e9 0e f7 ff ff       	jmp    80100560 <vector_handler>

80100e52 <vector220>:
80100e52:	6a 00                	push   $0x0
80100e54:	68 dc 00 00 00       	push   $0xdc
80100e59:	e9 02 f7 ff ff       	jmp    80100560 <vector_handler>

80100e5e <vector221>:
80100e5e:	6a 00                	push   $0x0
80100e60:	68 dd 00 00 00       	push   $0xdd
80100e65:	e9 f6 f6 ff ff       	jmp    80100560 <vector_handler>

80100e6a <vector222>:
80100e6a:	6a 00                	push   $0x0
80100e6c:	68 de 00 00 00       	push   $0xde
80100e71:	e9 ea f6 ff ff       	jmp    80100560 <vector_handler>

80100e76 <vector223>:
80100e76:	6a 00                	push   $0x0
80100e78:	68 df 00 00 00       	push   $0xdf
80100e7d:	e9 de f6 ff ff       	jmp    80100560 <vector_handler>

80100e82 <vector224>:
80100e82:	6a 00                	push   $0x0
80100e84:	68 e0 00 00 00       	push   $0xe0
80100e89:	e9 d2 f6 ff ff       	jmp    80100560 <vector_handler>

80100e8e <vector225>:
80100e8e:	6a 00                	push   $0x0
80100e90:	68 e1 00 00 00       	push   $0xe1
80100e95:	e9 c6 f6 ff ff       	jmp    80100560 <vector_handler>

80100e9a <vector226>:
80100e9a:	6a 00                	push   $0x0
80100e9c:	68 e2 00 00 00       	push   $0xe2
80100ea1:	e9 ba f6 ff ff       	jmp    80100560 <vector_handler>

80100ea6 <vector227>:
80100ea6:	6a 00                	push   $0x0
80100ea8:	68 e3 00 00 00       	push   $0xe3
80100ead:	e9 ae f6 ff ff       	jmp    80100560 <vector_handler>

80100eb2 <vector228>:
80100eb2:	6a 00                	push   $0x0
80100eb4:	68 e4 00 00 00       	push   $0xe4
80100eb9:	e9 a2 f6 ff ff       	jmp    80100560 <vector_handler>

80100ebe <vector229>:
80100ebe:	6a 00                	push   $0x0
80100ec0:	68 e5 00 00 00       	push   $0xe5
80100ec5:	e9 96 f6 ff ff       	jmp    80100560 <vector_handler>

80100eca <vector230>:
80100eca:	6a 00                	push   $0x0
80100ecc:	68 e6 00 00 00       	push   $0xe6
80100ed1:	e9 8a f6 ff ff       	jmp    80100560 <vector_handler>

80100ed6 <vector231>:
80100ed6:	6a 00                	push   $0x0
80100ed8:	68 e7 00 00 00       	push   $0xe7
80100edd:	e9 7e f6 ff ff       	jmp    80100560 <vector_handler>

80100ee2 <vector232>:
80100ee2:	6a 00                	push   $0x0
80100ee4:	68 e8 00 00 00       	push   $0xe8
80100ee9:	e9 72 f6 ff ff       	jmp    80100560 <vector_handler>

80100eee <vector233>:
80100eee:	6a 00                	push   $0x0
80100ef0:	68 e9 00 00 00       	push   $0xe9
80100ef5:	e9 66 f6 ff ff       	jmp    80100560 <vector_handler>

80100efa <vector234>:
80100efa:	6a 00                	push   $0x0
80100efc:	68 ea 00 00 00       	push   $0xea
80100f01:	e9 5a f6 ff ff       	jmp    80100560 <vector_handler>

80100f06 <vector235>:
80100f06:	6a 00                	push   $0x0
80100f08:	68 eb 00 00 00       	push   $0xeb
80100f0d:	e9 4e f6 ff ff       	jmp    80100560 <vector_handler>

80100f12 <vector236>:
80100f12:	6a 00                	push   $0x0
80100f14:	68 ec 00 00 00       	push   $0xec
80100f19:	e9 42 f6 ff ff       	jmp    80100560 <vector_handler>

80100f1e <vector237>:
80100f1e:	6a 00                	push   $0x0
80100f20:	68 ed 00 00 00       	push   $0xed
80100f25:	e9 36 f6 ff ff       	jmp    80100560 <vector_handler>

80100f2a <vector238>:
80100f2a:	6a 00                	push   $0x0
80100f2c:	68 ee 00 00 00       	push   $0xee
80100f31:	e9 2a f6 ff ff       	jmp    80100560 <vector_handler>

80100f36 <vector239>:
80100f36:	6a 00                	push   $0x0
80100f38:	68 ef 00 00 00       	push   $0xef
80100f3d:	e9 1e f6 ff ff       	jmp    80100560 <vector_handler>

80100f42 <vector240>:
80100f42:	6a 00                	push   $0x0
80100f44:	68 f0 00 00 00       	push   $0xf0
80100f49:	e9 12 f6 ff ff       	jmp    80100560 <vector_handler>

80100f4e <vector241>:
80100f4e:	6a 00                	push   $0x0
80100f50:	68 f1 00 00 00       	push   $0xf1
80100f55:	e9 06 f6 ff ff       	jmp    80100560 <vector_handler>

80100f5a <vector242>:
80100f5a:	6a 00                	push   $0x0
80100f5c:	68 f2 00 00 00       	push   $0xf2
80100f61:	e9 fa f5 ff ff       	jmp    80100560 <vector_handler>

80100f66 <vector243>:
80100f66:	6a 00                	push   $0x0
80100f68:	68 f3 00 00 00       	push   $0xf3
80100f6d:	e9 ee f5 ff ff       	jmp    80100560 <vector_handler>

80100f72 <vector244>:
80100f72:	6a 00                	push   $0x0
80100f74:	68 f4 00 00 00       	push   $0xf4
80100f79:	e9 e2 f5 ff ff       	jmp    80100560 <vector_handler>

80100f7e <vector245>:
80100f7e:	6a 00                	push   $0x0
80100f80:	68 f5 00 00 00       	push   $0xf5
80100f85:	e9 d6 f5 ff ff       	jmp    80100560 <vector_handler>

80100f8a <vector246>:
80100f8a:	6a 00                	push   $0x0
80100f8c:	68 f6 00 00 00       	push   $0xf6
80100f91:	e9 ca f5 ff ff       	jmp    80100560 <vector_handler>

80100f96 <vector247>:
80100f96:	6a 00                	push   $0x0
80100f98:	68 f7 00 00 00       	push   $0xf7
80100f9d:	e9 be f5 ff ff       	jmp    80100560 <vector_handler>

80100fa2 <vector248>:
80100fa2:	6a 00                	push   $0x0
80100fa4:	68 f8 00 00 00       	push   $0xf8
80100fa9:	e9 b2 f5 ff ff       	jmp    80100560 <vector_handler>

80100fae <vector249>:
80100fae:	6a 00                	push   $0x0
80100fb0:	68 f9 00 00 00       	push   $0xf9
80100fb5:	e9 a6 f5 ff ff       	jmp    80100560 <vector_handler>

80100fba <vector250>:
80100fba:	6a 00                	push   $0x0
80100fbc:	68 fa 00 00 00       	push   $0xfa
80100fc1:	e9 9a f5 ff ff       	jmp    80100560 <vector_handler>

80100fc6 <vector251>:
80100fc6:	6a 00                	push   $0x0
80100fc8:	68 fb 00 00 00       	push   $0xfb
80100fcd:	e9 8e f5 ff ff       	jmp    80100560 <vector_handler>

80100fd2 <vector252>:
80100fd2:	6a 00                	push   $0x0
80100fd4:	68 fc 00 00 00       	push   $0xfc
80100fd9:	e9 82 f5 ff ff       	jmp    80100560 <vector_handler>

80100fde <vector253>:
80100fde:	6a 00                	push   $0x0
80100fe0:	68 fd 00 00 00       	push   $0xfd
80100fe5:	e9 76 f5 ff ff       	jmp    80100560 <vector_handler>

80100fea <vector254>:
80100fea:	6a 00                	push   $0x0
80100fec:	68 fe 00 00 00       	push   $0xfe
80100ff1:	e9 6a f5 ff ff       	jmp    80100560 <vector_handler>

80100ff6 <vector255>:
80100ff6:	6a 00                	push   $0x0
80100ff8:	68 ff 00 00 00       	push   $0xff
80100ffd:	e9 5e f5 ff ff       	jmp    80100560 <vector_handler>

80101002 <itoa>:
80101002:	55                   	push   %ebp
80101003:	57                   	push   %edi
80101004:	56                   	push   %esi
80101005:	53                   	push   %ebx
80101006:	8b 54 24 14          	mov    0x14(%esp),%edx
8010100a:	8b 6c 24 18          	mov    0x18(%esp),%ebp
8010100e:	8b 7c 24 1c          	mov    0x1c(%esp),%edi
80101012:	8d 47 fe             	lea    -0x2(%edi),%eax
80101015:	83 f8 22             	cmp    $0x22,%eax
80101018:	77 0a                	ja     80101024 <itoa+0x22>
8010101a:	85 d2                	test   %edx,%edx
8010101c:	78 11                	js     8010102f <itoa+0x2d>
8010101e:	89 eb                	mov    %ebp,%ebx
80101020:	89 d9                	mov    %ebx,%ecx
80101022:	eb 1f                	jmp    80101043 <itoa+0x41>
80101024:	c6 45 00 00          	movb   $0x0,0x0(%ebp)
80101028:	89 e8                	mov    %ebp,%eax
8010102a:	5b                   	pop    %ebx
8010102b:	5e                   	pop    %esi
8010102c:	5f                   	pop    %edi
8010102d:	5d                   	pop    %ebp
8010102e:	c3                   	ret    
8010102f:	83 ff 0a             	cmp    $0xa,%edi
80101032:	74 04                	je     80101038 <itoa+0x36>
80101034:	89 eb                	mov    %ebp,%ebx
80101036:	eb e8                	jmp    80101020 <itoa+0x1e>
80101038:	8d 5d 01             	lea    0x1(%ebp),%ebx
8010103b:	c6 45 00 2d          	movb   $0x2d,0x0(%ebp)
8010103f:	eb df                	jmp    80101020 <itoa+0x1e>
80101041:	89 f1                	mov    %esi,%ecx
80101043:	8d 71 01             	lea    0x1(%ecx),%esi
80101046:	89 d0                	mov    %edx,%eax
80101048:	99                   	cltd   
80101049:	f7 ff                	idiv   %edi
8010104b:	8a 92 df 12 10 80    	mov    -0x7fefed21(%edx),%dl
80101051:	88 56 ff             	mov    %dl,-0x1(%esi)
80101054:	89 c2                	mov    %eax,%edx
80101056:	85 c0                	test   %eax,%eax
80101058:	75 e7                	jne    80101041 <itoa+0x3f>
8010105a:	c6 41 01 00          	movb   $0x0,0x1(%ecx)
8010105e:	39 cb                	cmp    %ecx,%ebx
80101060:	73 c6                	jae    80101028 <itoa+0x26>
80101062:	8a 03                	mov    (%ebx),%al
80101064:	43                   	inc    %ebx
80101065:	8a 11                	mov    (%ecx),%dl
80101067:	88 53 ff             	mov    %dl,-0x1(%ebx)
8010106a:	49                   	dec    %ecx
8010106b:	88 41 01             	mov    %al,0x1(%ecx)
8010106e:	39 cb                	cmp    %ecx,%ebx
80101070:	72 f0                	jb     80101062 <itoa+0x60>
80101072:	eb b4                	jmp    80101028 <itoa+0x26>

80101074 <write_string>:
80101074:	55                   	push   %ebp
80101075:	57                   	push   %edi
80101076:	56                   	push   %esi
80101077:	53                   	push   %ebx
80101078:	8b 5c 24 14          	mov    0x14(%esp),%ebx
8010107c:	8b 4c 24 18          	mov    0x18(%esp),%ecx
80101080:	8a 01                	mov    (%ecx),%al
80101082:	41                   	inc    %ecx
80101083:	be 67 66 66 66       	mov    $0x66666667,%esi
80101088:	eb 2d                	jmp    801010b7 <write_string+0x43>
8010108a:	a1 00 34 10 80       	mov    0x80103400,%eax
8010108f:	8d a8 00 80 f4 ff    	lea    -0xb8000(%eax),%ebp
80101095:	89 e8                	mov    %ebp,%eax
80101097:	f7 ee                	imul   %esi
80101099:	89 d0                	mov    %edx,%eax
8010109b:	c1 f8 06             	sar    $0x6,%eax
8010109e:	c1 fd 1f             	sar    $0x1f,%ebp
801010a1:	29 e8                	sub    %ebp,%eax
801010a3:	8d 44 80 05          	lea    0x5(%eax,%eax,4),%eax
801010a7:	c1 e0 05             	shl    $0x5,%eax
801010aa:	05 00 80 0b 00       	add    $0xb8000,%eax
801010af:	a3 00 34 10 80       	mov    %eax,0x80103400
801010b4:	8a 07                	mov    (%edi),%al
801010b6:	41                   	inc    %ecx
801010b7:	89 cf                	mov    %ecx,%edi
801010b9:	80 79 ff 00          	cmpb   $0x0,-0x1(%ecx)
801010bd:	74 2a                	je     801010e9 <write_string+0x75>
801010bf:	3c 0a                	cmp    $0xa,%al
801010c1:	74 c7                	je     8010108a <write_string+0x16>
801010c3:	8b 15 00 34 10 80    	mov    0x80103400,%edx
801010c9:	8d 6a 01             	lea    0x1(%edx),%ebp
801010cc:	89 2d 00 34 10 80    	mov    %ebp,0x80103400
801010d2:	88 02                	mov    %al,(%edx)
801010d4:	8a 07                	mov    (%edi),%al
801010d6:	8b 15 00 34 10 80    	mov    0x80103400,%edx
801010dc:	8d 7a 01             	lea    0x1(%edx),%edi
801010df:	89 3d 00 34 10 80    	mov    %edi,0x80103400
801010e5:	88 1a                	mov    %bl,(%edx)
801010e7:	eb cd                	jmp    801010b6 <write_string+0x42>
801010e9:	5b                   	pop    %ebx
801010ea:	5e                   	pop    %esi
801010eb:	5f                   	pop    %edi
801010ec:	5d                   	pop    %ebp
801010ed:	c3                   	ret    

801010ee <clr_scr>:
801010ee:	c7 05 00 34 10 80 00 	movl   $0xb8000,0x80103400
801010f5:	80 0b 00 
801010f8:	b8 00 80 0b 00       	mov    $0xb8000,%eax
801010fd:	eb 02                	jmp    80101101 <clr_scr+0x13>
801010ff:	89 d0                	mov    %edx,%eax
80101101:	c6 00 00             	movb   $0x0,(%eax)
80101104:	8d 50 02             	lea    0x2(%eax),%edx
80101107:	c6 40 01 00          	movb   $0x0,0x1(%eax)
8010110b:	3d 9e 8f 0b 00       	cmp    $0xb8f9e,%eax
80101110:	75 ed                	jne    801010ff <clr_scr+0x11>
80101112:	c3                   	ret    

80101113 <memcpy>:
80101113:	56                   	push   %esi
80101114:	53                   	push   %ebx
80101115:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101119:	8b 5c 24 10          	mov    0x10(%esp),%ebx
8010111d:	8b 4c 24 14          	mov    0x14(%esp),%ecx
80101121:	39 c3                	cmp    %eax,%ebx
80101123:	72 19                	jb     8010113e <memcpy+0x2b>
80101125:	85 c9                	test   %ecx,%ecx
80101127:	7e 12                	jle    8010113b <memcpy+0x28>
80101129:	89 ce                	mov    %ecx,%esi
8010112b:	ba 00 00 00 00       	mov    $0x0,%edx
80101130:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
80101133:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101136:	42                   	inc    %edx
80101137:	39 d6                	cmp    %edx,%esi
80101139:	75 f5                	jne    80101130 <memcpy+0x1d>
8010113b:	5b                   	pop    %ebx
8010113c:	5e                   	pop    %esi
8010113d:	c3                   	ret    
8010113e:	8d 14 0b             	lea    (%ebx,%ecx,1),%edx
80101141:	39 d0                	cmp    %edx,%eax
80101143:	73 e0                	jae    80101125 <memcpy+0x12>
80101145:	8d 51 ff             	lea    -0x1(%ecx),%edx
80101148:	85 c9                	test   %ecx,%ecx
8010114a:	7e ef                	jle    8010113b <memcpy+0x28>
8010114c:	8a 0c 13             	mov    (%ebx,%edx,1),%cl
8010114f:	88 0c 10             	mov    %cl,(%eax,%edx,1)
80101152:	4a                   	dec    %edx
80101153:	83 fa ff             	cmp    $0xffffffff,%edx
80101156:	75 f4                	jne    8010114c <memcpy+0x39>
80101158:	eb e1                	jmp    8010113b <memcpy+0x28>

8010115a <memset>:
8010115a:	57                   	push   %edi
8010115b:	8b 54 24 08          	mov    0x8(%esp),%edx
8010115f:	8b 44 24 0c          	mov    0xc(%esp),%eax
80101163:	8b 4c 24 10          	mov    0x10(%esp),%ecx
80101167:	89 d7                	mov    %edx,%edi
80101169:	09 cf                	or     %ecx,%edi
8010116b:	f7 c7 03 00 00 00    	test   $0x3,%edi
80101171:	74 09                	je     8010117c <memset+0x22>
80101173:	89 d7                	mov    %edx,%edi
80101175:	fc                   	cld    
80101176:	f3 aa                	rep stos %al,%es:(%edi)
80101178:	89 d0                	mov    %edx,%eax
8010117a:	5f                   	pop    %edi
8010117b:	c3                   	ret    
8010117c:	89 cf                	mov    %ecx,%edi
8010117e:	85 c9                	test   %ecx,%ecx
80101180:	78 0c                	js     8010118e <memset+0x34>
80101182:	89 f9                	mov    %edi,%ecx
80101184:	c1 f9 02             	sar    $0x2,%ecx
80101187:	89 d7                	mov    %edx,%edi
80101189:	fc                   	cld    
8010118a:	f3 ab                	rep stos %eax,%es:(%edi)
8010118c:	eb ea                	jmp    80101178 <memset+0x1e>
8010118e:	8d 79 03             	lea    0x3(%ecx),%edi
80101191:	eb ef                	jmp    80101182 <memset+0x28>

80101193 <strlen>:
80101193:	8b 54 24 04          	mov    0x4(%esp),%edx
80101197:	80 3a 00             	cmpb   $0x0,(%edx)
8010119a:	74 0d                	je     801011a9 <strlen+0x16>
8010119c:	b8 00 00 00 00       	mov    $0x0,%eax
801011a1:	40                   	inc    %eax
801011a2:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
801011a6:	75 f9                	jne    801011a1 <strlen+0xe>
801011a8:	c3                   	ret    
801011a9:	b8 00 00 00 00       	mov    $0x0,%eax
801011ae:	c3                   	ret    

Disassembly of section .rodata:

801011b0 <.rodata>:
801011b0:	46                   	inc    %esi
801011b1:	69 72 73 74 20 69 6e 	imul   $0x6e692074,0x73(%edx),%esi
801011b8:	74 65                	je     8010121f <strlen+0x8c>
801011ba:	72 72                	jb     8010122e <strlen+0x9b>
801011bc:	75 70                	jne    8010122e <strlen+0x9b>
801011be:	74 20                	je     801011e0 <strlen+0x4d>
801011c0:	74 61                	je     80101223 <strlen+0x90>
801011c2:	62 6c 65 20          	bound  %ebp,0x20(%ebp,%eiz,2)
801011c6:	69 73 20 00 48 65 6c 	imul   $0x6c654800,0x20(%ebx),%esi
801011cd:	6c                   	insb   (%dx),%es:(%edi)
801011ce:	6f                   	outsl  %ds:(%esi),(%dx)
801011cf:	20 77 6f             	and    %dh,0x6f(%edi)
801011d2:	72 6c                	jb     80101240 <strlen+0xad>
801011d4:	64 21 0a             	and    %ecx,%fs:(%edx)
801011d7:	00 57 65             	add    %dl,0x65(%edi)
801011da:	20 67 6f             	and    %ah,0x6f(%edi)
801011dd:	74 20                	je     801011ff <strlen+0x6c>
801011df:	6d                   	insl   (%dx),%es:(%edi)
801011e0:	65 6d                	gs insl (%dx),%es:(%edi)
801011e2:	6f                   	outsl  %ds:(%esi),(%dx)
801011e3:	72 79                	jb     8010125e <strlen+0xcb>
801011e5:	20 69 6e             	and    %ch,0x6e(%ecx)
801011e8:	66 6f                	outsw  %ds:(%esi),(%dx)
801011ea:	20 6c 69 6b          	and    %ch,0x6b(%ecx,%ebp,2)
801011ee:	65 3a 20             	cmp    %gs:(%eax),%ah
801011f1:	0a 00                	or     (%eax),%al
801011f3:	42                   	inc    %edx
801011f4:	61                   	popa   
801011f5:	73 65                	jae    8010125c <strlen+0xc9>
801011f7:	20 61 64             	and    %ah,0x64(%ecx)
801011fa:	64 72 3a             	fs jb  80101237 <strlen+0xa4>
801011fd:	20 00                	and    %al,(%eax)
801011ff:	20 20                	and    %ah,(%eax)
80101201:	4c                   	dec    %esp
80101202:	65 6e                	outsb  %gs:(%esi),(%dx)
80101204:	67 74 68             	addr16 je 8010126f <strlen+0xdc>
80101207:	3a 20                	cmp    (%eax),%ah
80101209:	00 20                	add    %ah,(%eax)
8010120b:	20 54 79 70          	and    %dl,0x70(%ecx,%edi,2)
8010120f:	65 3a 20             	cmp    %gs:(%eax),%ah
80101212:	00 55 53             	add    %dl,0x53(%ebp)
80101215:	41                   	inc    %ecx
80101216:	42                   	inc    %edx
80101217:	4c                   	dec    %esp
80101218:	45                   	inc    %ebp
80101219:	0a 00                	or     (%eax),%al
8010121b:	52                   	push   %edx
8010121c:	45                   	inc    %ebp
8010121d:	53                   	push   %ebx
8010121e:	45                   	inc    %ebp
8010121f:	52                   	push   %edx
80101220:	56                   	push   %esi
80101221:	45                   	inc    %ebp
80101222:	44                   	inc    %esp
80101223:	0a 00                	or     (%eax),%al
80101225:	41                   	inc    %ecx
80101226:	43                   	inc    %ebx
80101227:	50                   	push   %eax
80101228:	49                   	dec    %ecx
80101229:	20 52 45             	and    %dl,0x45(%edx)
8010122c:	43                   	inc    %ebx
8010122d:	4c                   	dec    %esp
8010122e:	41                   	inc    %ecx
8010122f:	4d                   	dec    %ebp
80101230:	0a 00                	or     (%eax),%al
80101232:	41                   	inc    %ecx
80101233:	43                   	inc    %ebx
80101234:	50                   	push   %eax
80101235:	49                   	dec    %ecx
80101236:	20 4e 56             	and    %cl,0x56(%esi)
80101239:	53                   	push   %ebx
8010123a:	0a 00                	or     (%eax),%al
8010123c:	42                   	inc    %edx
8010123d:	61                   	popa   
8010123e:	64 0a 00             	or     %fs:(%eax),%al
80101241:	00 00                	add    %al,(%eax)
80101243:	00 25 02 10 80 13    	add    %ah,0x13801002
80101249:	02 10                	add    (%eax),%dl
8010124b:	80 c5 02             	add    $0x2,%ch
8010124e:	10 80 dc 02 10 80    	adc    %al,-0x7feffd24(%eax)
80101254:	f3 02 10             	repz add (%eax),%dl
80101257:	80 0a 03             	orb    $0x3,(%edx)
8010125a:	10 80 4d 65 6d 6f    	adc    %al,0x6f6d654d(%eax)
80101260:	72 79                	jb     801012db <strlen+0x148>
80101262:	20 73 69             	and    %dh,0x69(%ebx)
80101265:	7a 65                	jp     801012cc <strlen+0x139>
80101267:	20 69 73             	and    %ch,0x73(%ecx)
8010126a:	20 00                	and    %al,(%eax)
8010126c:	20 4d 42             	and    %cl,0x42(%ebp)
8010126f:	0a 00                	or     (%eax),%al
80101271:	57                   	push   %edi
80101272:	65 20 6e 65          	and    %ch,%gs:0x65(%esi)
80101276:	65 64 20 00          	gs and %al,%fs:(%eax)
8010127a:	20 70 61             	and    %dh,0x61(%eax)
8010127d:	67 65 20 64 69       	and    %ah,%gs:0x69(%si)
80101282:	72 73                	jb     801012f7 <strlen+0x164>
80101284:	20 74 6f 20          	and    %dh,0x20(%edi,%ebp,2)
80101288:	6d                   	insl   (%dx),%es:(%edi)
80101289:	61                   	popa   
8010128a:	70 20                	jo     801012ac <strlen+0x119>
8010128c:	61                   	popa   
8010128d:	6c                   	insb   (%dx),%es:(%edi)
8010128e:	6c                   	insb   (%dx),%es:(%edi)
8010128f:	20 6d 65             	and    %ch,0x65(%ebp)
80101292:	6d                   	insl   (%dx),%es:(%edi)
80101293:	6f                   	outsl  %ds:(%esi),(%dx)
80101294:	72 79                	jb     8010130f <strlen+0x17c>
80101296:	0a 00                	or     (%eax),%al
80101298:	4d                   	dec    %ebp
80101299:	61                   	popa   
8010129a:	70 70                	jo     8010130c <strlen+0x179>
8010129c:	69 6e 67 20 6b 65 72 	imul   $0x72656b20,0x67(%esi),%ebp
801012a3:	6e                   	outsb  %ds:(%esi),(%dx)
801012a4:	65 6c                	gs insb (%dx),%es:(%edi)
801012a6:	20 61 64             	and    %ah,0x64(%ecx)
801012a9:	64 72 0a             	fs jb  801012b6 <strlen+0x123>
801012ac:	00 57 72             	add    %dl,0x72(%edi)
801012af:	69 74 65 20 63 72 33 	imul   $0x3a337263,0x20(%ebp,%eiz,2),%esi
801012b6:	3a 
801012b7:	20 30                	and    %dh,(%eax)
801012b9:	78 00                	js     801012bb <strlen+0x128>
801012bb:	00 5a 59             	add    %bl,0x59(%edx)
801012be:	58                   	pop    %eax
801012bf:	57                   	push   %edi
801012c0:	56                   	push   %esi
801012c1:	55                   	push   %ebp
801012c2:	54                   	push   %esp
801012c3:	53                   	push   %ebx
801012c4:	52                   	push   %edx
801012c5:	51                   	push   %ecx
801012c6:	50                   	push   %eax
801012c7:	4f                   	dec    %edi
801012c8:	4e                   	dec    %esi
801012c9:	4d                   	dec    %ebp
801012ca:	4c                   	dec    %esp
801012cb:	4b                   	dec    %ebx
801012cc:	4a                   	dec    %edx
801012cd:	49                   	dec    %ecx
801012ce:	48                   	dec    %eax
801012cf:	47                   	inc    %edi
801012d0:	46                   	inc    %esi
801012d1:	45                   	inc    %ebp
801012d2:	44                   	inc    %esp
801012d3:	43                   	inc    %ebx
801012d4:	42                   	inc    %edx
801012d5:	41                   	inc    %ecx
801012d6:	39 38                	cmp    %edi,(%eax)
801012d8:	37                   	aaa    
801012d9:	36 35 34 33 32 31    	ss xor $0x31323334,%eax
801012df:	30 31                	xor    %dh,(%ecx)
801012e1:	32 33                	xor    (%ebx),%dh
801012e3:	34 35                	xor    $0x35,%al
801012e5:	36 37                	ss aaa 
801012e7:	38 39                	cmp    %bh,(%ecx)
801012e9:	41                   	inc    %ecx
801012ea:	42                   	inc    %edx
801012eb:	43                   	inc    %ebx
801012ec:	44                   	inc    %esp
801012ed:	45                   	inc    %ebp
801012ee:	46                   	inc    %esi
801012ef:	47                   	inc    %edi
801012f0:	48                   	dec    %eax
801012f1:	49                   	dec    %ecx
801012f2:	4a                   	dec    %edx
801012f3:	4b                   	dec    %ebx
801012f4:	4c                   	dec    %esp
801012f5:	4d                   	dec    %ebp
801012f6:	4e                   	dec    %esi
801012f7:	4f                   	dec    %edi
801012f8:	50                   	push   %eax
801012f9:	51                   	push   %ecx
801012fa:	52                   	push   %edx
801012fb:	53                   	push   %ebx
801012fc:	54                   	push   %esp
801012fd:	55                   	push   %ebp
801012fe:	56                   	push   %esi
801012ff:	57                   	push   %edi
80101300:	58                   	pop    %eax
80101301:	59                   	pop    %ecx
80101302:	5a                   	pop    %edx
	...

Disassembly of section .multboot:

80101304 <.multboot>:
80101304:	02 b0 ad 1b 00 00    	add    0x1bad(%eax),%dh
8010130a:	00 00                	add    %al,(%eax)
8010130c:	fe 4f 52             	decb   0x52(%edi)
8010130f:	e4                   	.byte 0xe4

Disassembly of section .stab:

80101310 <.stab>:
	...

Disassembly of section .stabstr:

80101311 <.stabstr>:
	...

Disassembly of section .data:

80102000 <temp_page_dir>:
80102000:	83 00 00             	addl   $0x0,(%eax)
	...
801027ff:	00 83 00 00 00 00    	add    %al,0x0(%ebx)
	...

80103000 <vector_table>:
80103000:	90                   	nop
80103001:	05 10 80 99 05       	add    $0x5998010,%eax
80103006:	10 80 a2 05 10 80    	adc    %al,-0x7feffa5e(%eax)
8010300c:	ab                   	stos   %eax,%es:(%edi)
8010300d:	05 10 80 b4 05       	add    $0x5b48010,%eax
80103012:	10 80 bd 05 10 80    	adc    %al,-0x7feffa43(%eax)
80103018:	c6 05 10 80 cf 05 10 	movb   $0x10,0x5cf8010
8010301f:	80 d8 05             	sbb    $0x5,%al
80103022:	10 80 df 05 10 80    	adc    %al,-0x7feffa21(%eax)
80103028:	e8 05 10 80 ef       	call   6f904032 <MAGIC_NUMBER+0x53e29030>
8010302d:	05 10 80 f6 05       	add    $0x5f68010,%eax
80103032:	10 80 fd 05 10 80    	adc    %al,-0x7feffa03(%eax)
80103038:	04 06                	add    $0x6,%al
8010303a:	10 80 0b 06 10 80    	adc    %al,-0x7feff9f5(%eax)
80103040:	14 06                	adc    $0x6,%al
80103042:	10 80 1d 06 10 80    	adc    %al,-0x7feff9e3(%eax)
80103048:	24 06                	and    $0x6,%al
8010304a:	10 80 2d 06 10 80    	adc    %al,-0x7feff9d3(%eax)
80103050:	36 06                	ss push %es
80103052:	10 80 3f 06 10 80    	adc    %al,-0x7feff9c1(%eax)
80103058:	48                   	dec    %eax
80103059:	06                   	push   %es
8010305a:	10 80 51 06 10 80    	adc    %al,-0x7feff9af(%eax)
80103060:	5a                   	pop    %edx
80103061:	06                   	push   %es
80103062:	10 80 63 06 10 80    	adc    %al,-0x7feff99d(%eax)
80103068:	6c                   	insb   (%dx),%es:(%edi)
80103069:	06                   	push   %es
8010306a:	10 80 75 06 10 80    	adc    %al,-0x7feff98b(%eax)
80103070:	7e 06                	jle    80103078 <vector_table+0x78>
80103072:	10 80 87 06 10 80    	adc    %al,-0x7feff979(%eax)
80103078:	90                   	nop
80103079:	06                   	push   %es
8010307a:	10 80 99 06 10 80    	adc    %al,-0x7feff967(%eax)
80103080:	a2 06 10 80 ab       	mov    %al,0xab801006
80103085:	06                   	push   %es
80103086:	10 80 b4 06 10 80    	adc    %al,-0x7feff94c(%eax)
8010308c:	bd 06 10 80 c6       	mov    $0xc6801006,%ebp
80103091:	06                   	push   %es
80103092:	10 80 cf 06 10 80    	adc    %al,-0x7feff931(%eax)
80103098:	d8 06                	fadds  (%esi)
8010309a:	10 80 e1 06 10 80    	adc    %al,-0x7feff91f(%eax)
801030a0:	ea 06 10 80 f3 06 10 	ljmp   $0x1006,$0xf3801006
801030a7:	80 fc 06             	cmp    $0x6,%ah
801030aa:	10 80 05 07 10 80    	adc    %al,-0x7feff8fb(%eax)
801030b0:	0e                   	push   %cs
801030b1:	07                   	pop    %es
801030b2:	10 80 17 07 10 80    	adc    %al,-0x7feff8e9(%eax)
801030b8:	20 07                	and    %al,(%edi)
801030ba:	10 80 29 07 10 80    	adc    %al,-0x7feff8d7(%eax)
801030c0:	32 07                	xor    (%edi),%al
801030c2:	10 80 3b 07 10 80    	adc    %al,-0x7feff8c5(%eax)
801030c8:	44                   	inc    %esp
801030c9:	07                   	pop    %es
801030ca:	10 80 4d 07 10 80    	adc    %al,-0x7feff8b3(%eax)
801030d0:	56                   	push   %esi
801030d1:	07                   	pop    %es
801030d2:	10 80 5f 07 10 80    	adc    %al,-0x7feff8a1(%eax)
801030d8:	68 07 10 80 71       	push   $0x71801007
801030dd:	07                   	pop    %es
801030de:	10 80 7a 07 10 80    	adc    %al,-0x7feff886(%eax)
801030e4:	83 07 10             	addl   $0x10,(%edi)
801030e7:	80 8c 07 10 80 95 07 	orb    $0x10,0x7958010(%edi,%eax,1)
801030ee:	10 
801030ef:	80 9e 07 10 80 a7 07 	sbbb   $0x7,-0x587feff9(%esi)
801030f6:	10 80 b0 07 10 80    	adc    %al,-0x7feff850(%eax)
801030fc:	b9 07 10 80 c2       	mov    $0xc2801007,%ecx
80103101:	07                   	pop    %es
80103102:	10 80 cb 07 10 80    	adc    %al,-0x7feff835(%eax)
80103108:	d4 07                	aam    $0x7
8010310a:	10 80 dd 07 10 80    	adc    %al,-0x7feff823(%eax)
80103110:	e6 07                	out    %al,$0x7
80103112:	10 80 ef 07 10 80    	adc    %al,-0x7feff811(%eax)
80103118:	f8                   	clc    
80103119:	07                   	pop    %es
8010311a:	10 80 01 08 10 80    	adc    %al,-0x7feff7ff(%eax)
80103120:	0a 08                	or     (%eax),%cl
80103122:	10 80 13 08 10 80    	adc    %al,-0x7feff7ed(%eax)
80103128:	1c 08                	sbb    $0x8,%al
8010312a:	10 80 25 08 10 80    	adc    %al,-0x7feff7db(%eax)
80103130:	2e 08 10             	or     %dl,%cs:(%eax)
80103133:	80 37 08             	xorb   $0x8,(%edi)
80103136:	10 80 40 08 10 80    	adc    %al,-0x7feff7c0(%eax)
8010313c:	49                   	dec    %ecx
8010313d:	08 10                	or     %dl,(%eax)
8010313f:	80 52 08 10          	adcb   $0x10,0x8(%edx)
80103143:	80 5b 08 10          	sbbb   $0x10,0x8(%ebx)
80103147:	80 64 08 10 80       	andb   $0x80,0x10(%eax,%ecx,1)
8010314c:	6d                   	insl   (%dx),%es:(%edi)
8010314d:	08 10                	or     %dl,(%eax)
8010314f:	80 76 08 10          	xorb   $0x10,0x8(%esi)
80103153:	80 7f 08 10          	cmpb   $0x10,0x8(%edi)
80103157:	80 88 08 10 80 91 08 	orb    $0x8,-0x6e7feff8(%eax)
8010315e:	10 80 9a 08 10 80    	adc    %al,-0x7feff766(%eax)
80103164:	a3 08 10 80 ac       	mov    %eax,0xac801008
80103169:	08 10                	or     %dl,(%eax)
8010316b:	80 b5 08 10 80 be 08 	xorb   $0x8,-0x417feff8(%ebp)
80103172:	10 80 c7 08 10 80    	adc    %al,-0x7feff739(%eax)
80103178:	d0 08                	rorb   (%eax)
8010317a:	10 80 d9 08 10 80    	adc    %al,-0x7feff727(%eax)
80103180:	e2 08                	loop   8010318a <vector_table+0x18a>
80103182:	10 80 eb 08 10 80    	adc    %al,-0x7feff715(%eax)
80103188:	f4                   	hlt    
80103189:	08 10                	or     %dl,(%eax)
8010318b:	80 fd 08             	cmp    $0x8,%ch
8010318e:	10 80 06 09 10 80    	adc    %al,-0x7feff6fa(%eax)
80103194:	0f 09                	wbinvd 
80103196:	10 80 18 09 10 80    	adc    %al,-0x7feff6e8(%eax)
8010319c:	21 09                	and    %ecx,(%ecx)
8010319e:	10 80 2a 09 10 80    	adc    %al,-0x7feff6d6(%eax)
801031a4:	33 09                	xor    (%ecx),%ecx
801031a6:	10 80 3c 09 10 80    	adc    %al,-0x7feff6c4(%eax)
801031ac:	45                   	inc    %ebp
801031ad:	09 10                	or     %edx,(%eax)
801031af:	80 4e 09 10          	orb    $0x10,0x9(%esi)
801031b3:	80 57 09 10          	adcb   $0x10,0x9(%edi)
801031b7:	80 60 09 10          	andb   $0x10,0x9(%eax)
801031bb:	80 69 09 10          	subb   $0x10,0x9(%ecx)
801031bf:	80 72 09 10          	xorb   $0x10,0x9(%edx)
801031c3:	80 7b 09 10          	cmpb   $0x10,0x9(%ebx)
801031c7:	80 84 09 10 80 8d 09 	addb   $0x10,0x98d8010(%ecx,%ecx,1)
801031ce:	10 
801031cf:	80 96 09 10 80 9f 09 	adcb   $0x9,-0x607feff7(%esi)
801031d6:	10 80 a8 09 10 80    	adc    %al,-0x7feff658(%eax)
801031dc:	b1 09                	mov    $0x9,%cl
801031de:	10 80 ba 09 10 80    	adc    %al,-0x7feff646(%eax)
801031e4:	c3                   	ret    
801031e5:	09 10                	or     %edx,(%eax)
801031e7:	80 cc 09             	or     $0x9,%ah
801031ea:	10 80 d5 09 10 80    	adc    %al,-0x7feff62b(%eax)
801031f0:	de 09                	fimuls (%ecx)
801031f2:	10 80 e7 09 10 80    	adc    %al,-0x7feff619(%eax)
801031f8:	f0 09 10             	lock or %edx,(%eax)
801031fb:	80 f9 09             	cmp    $0x9,%cl
801031fe:	10 80 02 0a 10 80    	adc    %al,-0x7feff5fe(%eax)
80103204:	0e                   	push   %cs
80103205:	0a 10                	or     (%eax),%dl
80103207:	80 1a 0a             	sbbb   $0xa,(%edx)
8010320a:	10 80 26 0a 10 80    	adc    %al,-0x7feff5da(%eax)
80103210:	32 0a                	xor    (%edx),%cl
80103212:	10 80 3e 0a 10 80    	adc    %al,-0x7feff5c2(%eax)
80103218:	4a                   	dec    %edx
80103219:	0a 10                	or     (%eax),%dl
8010321b:	80 56 0a 10          	adcb   $0x10,0xa(%esi)
8010321f:	80 62 0a 10          	andb   $0x10,0xa(%edx)
80103223:	80 6e 0a 10          	subb   $0x10,0xa(%esi)
80103227:	80 7a 0a 10          	cmpb   $0x10,0xa(%edx)
8010322b:	80 86 0a 10 80 92 0a 	addb   $0xa,-0x6d7feff6(%esi)
80103232:	10 80 9e 0a 10 80    	adc    %al,-0x7feff562(%eax)
80103238:	aa                   	stos   %al,%es:(%edi)
80103239:	0a 10                	or     (%eax),%dl
8010323b:	80 b6 0a 10 80 c2 0a 	xorb   $0xa,-0x3d7feff6(%esi)
80103242:	10 80 ce 0a 10 80    	adc    %al,-0x7feff532(%eax)
80103248:	da 0a                	fimull (%edx)
8010324a:	10 80 e6 0a 10 80    	adc    %al,-0x7feff51a(%eax)
80103250:	f2 0a 10             	repnz or (%eax),%dl
80103253:	80 fe 0a             	cmp    $0xa,%dh
80103256:	10 80 0a 0b 10 80    	adc    %al,-0x7feff4f6(%eax)
8010325c:	16                   	push   %ss
8010325d:	0b 10                	or     (%eax),%edx
8010325f:	80 22 0b             	andb   $0xb,(%edx)
80103262:	10 80 2e 0b 10 80    	adc    %al,-0x7feff4d2(%eax)
80103268:	3a 0b                	cmp    (%ebx),%cl
8010326a:	10 80 46 0b 10 80    	adc    %al,-0x7feff4ba(%eax)
80103270:	52                   	push   %edx
80103271:	0b 10                	or     (%eax),%edx
80103273:	80 5e 0b 10          	sbbb   $0x10,0xb(%esi)
80103277:	80 6a 0b 10          	subb   $0x10,0xb(%edx)
8010327b:	80 76 0b 10          	xorb   $0x10,0xb(%esi)
8010327f:	80 82 0b 10 80 8e 0b 	addb   $0xb,-0x717feff5(%edx)
80103286:	10 80 9a 0b 10 80    	adc    %al,-0x7feff466(%eax)
8010328c:	a6                   	cmpsb  %es:(%edi),%ds:(%esi)
8010328d:	0b 10                	or     (%eax),%edx
8010328f:	80 b2 0b 10 80 be 0b 	xorb   $0xb,-0x417feff5(%edx)
80103296:	10 80 ca 0b 10 80    	adc    %al,-0x7feff436(%eax)
8010329c:	d6                   	(bad)  
8010329d:	0b 10                	or     (%eax),%edx
8010329f:	80 e2 0b             	and    $0xb,%dl
801032a2:	10 80 ee 0b 10 80    	adc    %al,-0x7feff412(%eax)
801032a8:	fa                   	cli    
801032a9:	0b 10                	or     (%eax),%edx
801032ab:	80 06 0c             	addb   $0xc,(%esi)
801032ae:	10 80 12 0c 10 80    	adc    %al,-0x7feff3ee(%eax)
801032b4:	1e                   	push   %ds
801032b5:	0c 10                	or     $0x10,%al
801032b7:	80 2a 0c             	subb   $0xc,(%edx)
801032ba:	10 80 36 0c 10 80    	adc    %al,-0x7feff3ca(%eax)
801032c0:	42                   	inc    %edx
801032c1:	0c 10                	or     $0x10,%al
801032c3:	80 4e 0c 10          	orb    $0x10,0xc(%esi)
801032c7:	80 5a 0c 10          	sbbb   $0x10,0xc(%edx)
801032cb:	80 66 0c 10          	andb   $0x10,0xc(%esi)
801032cf:	80 72 0c 10          	xorb   $0x10,0xc(%edx)
801032d3:	80 7e 0c 10          	cmpb   $0x10,0xc(%esi)
801032d7:	80 8a 0c 10 80 96 0c 	orb    $0xc,-0x697feff4(%edx)
801032de:	10 80 a2 0c 10 80    	adc    %al,-0x7feff35e(%eax)
801032e4:	ae                   	scas   %es:(%edi),%al
801032e5:	0c 10                	or     $0x10,%al
801032e7:	80 ba 0c 10 80 c6 0c 	cmpb   $0xc,-0x397feff4(%edx)
801032ee:	10 80 d2 0c 10 80    	adc    %al,-0x7feff32e(%eax)
801032f4:	de 0c 10             	fimuls (%eax,%edx,1)
801032f7:	80 ea 0c             	sub    $0xc,%dl
801032fa:	10 80 f6 0c 10 80    	adc    %al,-0x7feff30a(%eax)
80103300:	02 0d 10 80 0e 0d    	add    0xd0e8010,%cl
80103306:	10 80 1a 0d 10 80    	adc    %al,-0x7feff2e6(%eax)
8010330c:	26 0d 10 80 32 0d    	es or  $0xd328010,%eax
80103312:	10 80 3e 0d 10 80    	adc    %al,-0x7feff2c2(%eax)
80103318:	4a                   	dec    %edx
80103319:	0d 10 80 56 0d       	or     $0xd568010,%eax
8010331e:	10 80 62 0d 10 80    	adc    %al,-0x7feff29e(%eax)
80103324:	6e                   	outsb  %ds:(%esi),(%dx)
80103325:	0d 10 80 7a 0d       	or     $0xd7a8010,%eax
8010332a:	10 80 86 0d 10 80    	adc    %al,-0x7feff27a(%eax)
80103330:	92                   	xchg   %eax,%edx
80103331:	0d 10 80 9e 0d       	or     $0xd9e8010,%eax
80103336:	10 80 aa 0d 10 80    	adc    %al,-0x7feff256(%eax)
8010333c:	b6 0d                	mov    $0xd,%dh
8010333e:	10 80 c2 0d 10 80    	adc    %al,-0x7feff23e(%eax)
80103344:	ce                   	into   
80103345:	0d 10 80 da 0d       	or     $0xdda8010,%eax
8010334a:	10 80 e6 0d 10 80    	adc    %al,-0x7feff21a(%eax)
80103350:	f2 0d 10 80 fe 0d    	repnz or $0xdfe8010,%eax
80103356:	10 80 0a 0e 10 80    	adc    %al,-0x7feff1f6(%eax)
8010335c:	16                   	push   %ss
8010335d:	0e                   	push   %cs
8010335e:	10 80 22 0e 10 80    	adc    %al,-0x7feff1de(%eax)
80103364:	2e 0e                	cs push %cs
80103366:	10 80 3a 0e 10 80    	adc    %al,-0x7feff1c6(%eax)
8010336c:	46                   	inc    %esi
8010336d:	0e                   	push   %cs
8010336e:	10 80 52 0e 10 80    	adc    %al,-0x7feff1ae(%eax)
80103374:	5e                   	pop    %esi
80103375:	0e                   	push   %cs
80103376:	10 80 6a 0e 10 80    	adc    %al,-0x7feff196(%eax)
8010337c:	76 0e                	jbe    8010338c <vector_table+0x38c>
8010337e:	10 80 82 0e 10 80    	adc    %al,-0x7feff17e(%eax)
80103384:	8e 0e                	mov    (%esi),%cs
80103386:	10 80 9a 0e 10 80    	adc    %al,-0x7feff166(%eax)
8010338c:	a6                   	cmpsb  %es:(%edi),%ds:(%esi)
8010338d:	0e                   	push   %cs
8010338e:	10 80 b2 0e 10 80    	adc    %al,-0x7feff14e(%eax)
80103394:	be 0e 10 80 ca       	mov    $0xca80100e,%esi
80103399:	0e                   	push   %cs
8010339a:	10 80 d6 0e 10 80    	adc    %al,-0x7feff12a(%eax)
801033a0:	e2 0e                	loop   801033b0 <vector_table+0x3b0>
801033a2:	10 80 ee 0e 10 80    	adc    %al,-0x7feff112(%eax)
801033a8:	fa                   	cli    
801033a9:	0e                   	push   %cs
801033aa:	10 80 06 0f 10 80    	adc    %al,-0x7feff0fa(%eax)
801033b0:	12 0f                	adc    (%edi),%cl
801033b2:	10 80 1e 0f 10 80    	adc    %al,-0x7feff0e2(%eax)
801033b8:	2a 0f                	sub    (%edi),%cl
801033ba:	10 80 36 0f 10 80    	adc    %al,-0x7feff0ca(%eax)
801033c0:	42                   	inc    %edx
801033c1:	0f 10 80 4e 0f 10 80 	movups -0x7feff0b2(%eax),%xmm0
801033c8:	5a                   	pop    %edx
801033c9:	0f 10 80 66 0f 10 80 	movups -0x7feff09a(%eax),%xmm0
801033d0:	72 0f                	jb     801033e1 <vector_table+0x3e1>
801033d2:	10 80 7e 0f 10 80    	adc    %al,-0x7feff082(%eax)
801033d8:	8a 0f                	mov    (%edi),%cl
801033da:	10 80 96 0f 10 80    	adc    %al,-0x7feff06a(%eax)
801033e0:	a2 0f 10 80 ae       	mov    %al,0xae80100f
801033e5:	0f 10 80 ba 0f 10 80 	movups -0x7feff046(%eax),%xmm0
801033ec:	c6                   	(bad)  
801033ed:	0f 10 80 d2 0f 10 80 	movups -0x7feff02e(%eax),%xmm0
801033f4:	de 0f                	fimuls (%edi)
801033f6:	10 80 ea 0f 10 80    	adc    %al,-0x7feff016(%eax)
801033fc:	f6 0f 10             	testb  $0x10,(%edi)
801033ff:	80               	addb   $0x80,(%eax)

80103400 <pDisp>:
80103400:	00                   	.byte 0x0
80103401:	80 0b 00             	orb    $0x0,(%ebx)

Disassembly of section .bss:

80103410 <stack_bottom>:
	...

801053b0 <idt_ptr>:
	...

801053b8 <gdt_ptr>:
801053b8:	00 00                	add    %al,(%eax)
801053ba:	00 00                	add    %al,(%eax)
	...

Disassembly of section .comment:

00000000 <.comment>:
   0:	47                   	inc    %edi
   1:	43                   	inc    %ebx
   2:	43                   	inc    %ebx
   3:	3a 20                	cmp    (%eax),%ah
   5:	28 47 4e             	sub    %al,0x4e(%edi)
   8:	55                   	push   %ebp
   9:	29 20                	sub    %esp,(%eax)
   b:	37                   	aaa    
   c:	2e 33 2e             	xor    %cs:(%esi),%ebp
   f:	30 00                	xor    %al,(%eax)
