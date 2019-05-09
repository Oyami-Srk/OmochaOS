; Auto Generated Code


;================================
struc __process
.stack resb stack_frame_size
.selector_ldt resb 2
.ldts resb Descriptor_size
.status resb 4
.pid resb 4
.name resb 1
.p_msg resd 1
.quene_head_sending_to_this_process resd 1
.quene_body resd 1
endstruc 
;==============================

;================================
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
;==============================

;================================
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
;==============================

;================================
struc Descriptor
.limit_l resb 2
.base_l resb 2
.base_m resb 1
.attr1 resb 1
.limit_h_attr2 resb 1
.base_h resb 1
endstruc 
;==============================

;================================
struc Gate
.offset_l resb 2
.selector resb 2
.zero resb 1
.type resb 4
.s resb 4
.dpl resb 4
.p resb 4
.offset_h resb 2
endstruc 
;==============================


;================================
struc message
.sender resb 4
.receiver resb 4
.type resb 4
.major_data resb 4
.data resb 16
endstruc 
;==============================

;================================
struc __list
.data resb 4
.next resd 1
endstruc 
;==============================

