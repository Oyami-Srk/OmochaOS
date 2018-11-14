
bs.o:     file format elf32-i386


Disassembly of section .text:

00007c00 <_gdt-0x3>:
    7c00:	eb 27                	jmp    7c29 <start>
    7c02:	90                   	nop

00007c03 <_gdt>:
	...

00007c0b <code_desc>:
    7c0b:	ff                   	(bad)  
    7c0c:	ff 00                	incl   (%eax)
    7c0e:	00 00                	add    %al,(%eax)
    7c10:	98                   	cwtl   
    7c11:	cf                   	iret   
	...

00007c13 <data_desc>:
    7c13:	ff                   	(bad)  
    7c14:	ff 00                	incl   (%eax)
    7c16:	00 00                	add    %al,(%eax)
    7c18:	92                   	xchg   %eax,%edx
    7c19:	cf                   	iret   
	...

00007c1b <scrn_desc>:
    7c1b:	ff                   	(bad)  
    7c1c:	ff 00                	incl   (%eax)
    7c1e:	80 0b 92             	orb    $0x92,(%ebx)
	...

00007c23 <pGDT>:
    7c23:	1f                   	pop    %ds
    7c24:	00 03                	add    %al,(%ebx)
    7c26:	7c 00                	jl     7c28 <pGDT+0x5>
	...

00007c29 <start>:
    7c29:	8c c8                	mov    %cs,%eax
    7c2b:	8e d8                	mov    %eax,%ds
    7c2d:	8e c0                	mov    %eax,%es
    7c2f:	8e d0                	mov    %eax,%ss
    7c31:	bc 00 7c 0f 01       	mov    $0x10f7c00,%esp
    7c36:	16                   	push   %ss
    7c37:	23 7c fa e4          	and    -0x1c(%edx,%edi,8),%edi
    7c3b:	92                   	xchg   %eax,%edx
    7c3c:	0c 02                	or     $0x2,%al
    7c3e:	e6 92                	out    %al,$0x92
    7c40:	0f 20 c0             	mov    %cr0,%eax
    7c43:	66 83 c8 01          	or     $0x1,%ax
    7c47:	0f 22 c0             	mov    %eax,%cr0
    7c4a:	66 ea 60 7c 00 00    	ljmpw  $0x0,$0x7c60
    7c50:	08 00                	or     %al,(%eax)
    7c52:	f4                   	hlt    
    7c53:	90                   	nop
    7c54:	90                   	nop
    7c55:	90                   	nop
    7c56:	90                   	nop
    7c57:	90                   	nop
    7c58:	90                   	nop
    7c59:	90                   	nop
    7c5a:	90                   	nop
    7c5b:	90                   	nop
    7c5c:	90                   	nop
    7c5d:	90                   	nop
    7c5e:	90                   	nop
    7c5f:	90                   	nop

00007c60 <_start32>:
    7c60:	66 b8 18 00          	mov    $0x18,%ax
    7c64:	8e e8                	mov    %eax,%gs
    7c66:	66 b8 10 00          	mov    $0x10,%ax
    7c6a:	8e d8                	mov    %eax,%ds
    7c6c:	8e c0                	mov    %eax,%es
    7c6e:	8e e0                	mov    %eax,%fs
    7c70:	8e d0                	mov    %eax,%ss
    7c72:	bc 00 7c 00 00       	mov    $0x7c00,%esp
    7c77:	bc 29 7c 00 00       	mov    $0x7c29,%esp
    7c7c:	e8 11 00 00 00       	call   7c92 <boot_main>
    7c81:	66 b8 00 8a          	mov    $0x8a00,%ax
    7c85:	66 89 c2             	mov    %ax,%dx
    7c88:	66 ef                	out    %ax,(%dx)
    7c8a:	66 b8 e0 8a          	mov    $0x8ae0,%ax
    7c8e:	66 ef                	out    %ax,(%dx)
    7c90:	eb fe                	jmp    7c90 <_start32+0x30>

00007c92 <boot_main>:
    7c92:	ba 00 00 00 00       	mov    $0x0,%edx
    7c97:	b9 00 80 0b 00       	mov    $0xb8000,%ecx
    7c9c:	b0 4e                	mov    $0x4e,%al
    7c9e:	0f b6 c0             	movzbl %al,%eax
    7ca1:	80 cc 07             	or     $0x7,%ah
    7ca4:	66 89 01             	mov    %ax,(%ecx)
    7ca7:	42                   	inc    %edx
    7ca8:	8a 82 bd 7c 00 00    	mov    0x7cbd(%edx),%al
    7cae:	8d 49 02             	lea    0x2(%ecx),%ecx
    7cb1:	84 c0                	test   %al,%al
    7cb3:	75 e9                	jne    7c9e <boot_main+0xc>
    7cb5:	83 fa 0a             	cmp    $0xa,%edx
    7cb8:	7f 02                	jg     7cbc <boot_main+0x2a>
    7cba:	eb fe                	jmp    7cba <boot_main+0x28>
    7cbc:	c3                   	ret    
