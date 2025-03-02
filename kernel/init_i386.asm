cpu 386
bits 32
global __start
__start:
    ; Replace Bootloader GDT
    lgdt [_gdt_ptr]
    jmp 0x8:.continue
    .continue:
    mov dx, 0x10
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    mov esp, stack_top
    ; Call HCIX Initializer
    push ecx
    push ebx
    push eax
    extern _hcix_init
    call _hcix_init
    .loop:
    hlt
    jmp .loop
section .data
_gdt_ptr:
    dw _gdt_table.end - _gdt_table - 1
    dd _gdt_table
_gdt_table:
    dw 0, 0
    db 0, 0, 0, 0
    dw 0xffff, 0x0000
    db 0x00, 0x9a, 0xcf, 0x00
    dw 0xffff, 0x0000
    db 0x00, 0x92, 0xcf, 0x00
    dw 0xffff, 0x0000
    db 0x00, 0x8a, 0xcf, 0x00
    dw 0xffff, 0x0000
    db 0x00, 0x82, 0xcf, 0x00
    .end:
    
section .bss
    resd 10240
stack_top: