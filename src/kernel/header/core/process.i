;# vi: ft=nasm

struc stack_frame
.gs resb 2
.pd1 resb 2
.fs resb 2
.pd2 resb 2
.es resb 2
.pd3 resb 2
.ds resb 2
.pd4 resb 2
.edi resb 4
.esi resb 4
.ebp resb 4
.oesp resb 4
.ebx resb 4
.edx resb 4
.ecx resb 4
.eax resb 4
.trap_no resb 4
.err_code resb 4
.eip resb 4
.cs resb 2
.pd5 resb 2
.eflags resb 4
.esp resb 4
.ss resb 2
.pd6 resb 2
endstruc 

struc __tss
.backlink resb 4
.esp0 resb 4
.ss0 resb 4
.esp1 resb 4
.ss1 resb 4
.esp2 resb 4
.ss2 resb 4
.cr3 resb 4
.eip resb 4
.flags resb 4
.eax resb 4
.ecx resb 4
.edx resb 4
.ebx resb 4
.esp resb 4
.ebp resb 4
.esi resb 4
.edi resb 4
.es resb 4
.cs resb 4
.ss resb 4
.ds resb 4
.fs resb 4
.gs resb 4
.ldt resb 4
.trap resb 2
.iobase resb 2
endstruc 

struc process
.stack resb stack_frame_size
.page_dir resd 1
.kstack resd 1
endstruc
