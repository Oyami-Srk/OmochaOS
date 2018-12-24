  ; Coding for fun, not for salary.
  ; Coding for will, till the top.
  ; 码乐隔币，码德至璋

  ; 描述符属性
  ; 以下代码为32位，未来需要移植到64位
%define DA_32    0x4000
%define DA_4K    0x8000

%define DA_DPL_0 0x00
%define DA_DPL_1 0x20
%define DA_DPL_2 0x40
%define DA_DPL_3 0x80

%define DA_D_R 0x90
%define DA_D_RW 0x92
%define DA_D_RWA 0x93
%define DA_C_E 0x98
%define DA_C_ER 0x9A
%define DA_C_ECO 0x9C
%define DA_C_ERCO 0x9E

  ; 描述符制作宏
  ; Descriptor base, limit, attributes
%macro Descriptor 3
  dw %2 & 0xFFFF
  dw %1 & 0xFFFF
  db (%1 >> 16) & 0xFF
  dw ((%2 >> 8) & 0xF00) | (%3 & 0xF0FF)
  db (%1 >> 24) & 0xFF
%endmacro                       ; 8bytes


