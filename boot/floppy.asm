org 0x7c00
bits 16
start:
    ; set segment registers
    xor ax, ax
    mov ds, ax
    cli
    mov ss, ax
    mov sp, 0x7c00
    sti
    mov ax, 0x1000
    mov es, ax
    ; store disk id
    mov [var_disk], dl
    ; load kernel
    .load:
        cmp word [var_blocks], 0
        je .done
        mov cl, [var_sector]
        mov ch, [var_cyl]
        mov dh, [var_head]
        mov dl, [var_disk]
        xor bx, bx
        mov ax, 0x201
        int 0x13
        jnc .read
        xor ax, ax
        mov dl, [var_disk]
        int 0x13
        mov ax, 0xe00 | '!'
        xor bx, bx
        int 0x10
        jmp .load
        .read:
        inc word [var_sector]
        mov ax, [var_sector]
        cmp ax, [var_sectors]
        jbe .next
        mov word [var_sector], 1
        inc word [var_head]
        mov ax, [var_head]
        cmp ax, [var_heads]
        jb .next
        mov word [var_head], 0
        inc word [var_cyl]
        .next:
        mov ax, es
        add ax, 512/16
        mov es, ax
        dec word [var_blocks]
        mov ax, 0xe00 | '.'
        xor bx, bx
        int 0x10
        jmp .load
    .done:
    mov ax, 0xe00 | 'O'
    xor bx, bx
    int 0x10
    mov ax, 0xe00 | 'K'
    xor bx, bx
    int 0x10
    mov ax, 0xe00 | 13
    xor bx, bx
    int 0x10
    mov ax, 0xe00 | 10
    xor bx, bx
    int 0x10
    ; get low memory size
    xor ax, ax
    int 0x12
    mov [var_lowmem], ax
    ; get high memory size
    mov ah, 0x88
    int 0x15
    mov [var_highmem], ax
    ; restore es = 0
    mov ax, ds
    mov es, ax
    ; disable interrupts
    cli
    ; enable a20
    call kb_wait
    mov al, 0xd1
    out 0x64, al
    call kb_wait
    mov al, 0xdf
    out 0x64, al
    ; switch to protected
    lgdt [gdt_ptr]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x8:start32

bits 32
start32:
    mov ax, 0x10
    mov ds, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7c00
    mov eax, kernel_args
    mov ebx, [var_lowmem]
    mov ecx, [var_highmem]
    jmp 0x8:0x10000
    .loop:
    hlt
    jmp .loop

kb_wait:
    in al, 0x64
    test al, 2
    jnz kb_wait
    ret

var_disk: dw 0
var_sector: dw 2
var_head: dw 0
var_cyl: dw 0
var_lowmem: dd 0
var_highmem: dd 0

times 290-($-$$) db 0
db 'HC'
var_blocks: dw 1024
var_sectors: dw 18
var_heads: dw 2
var_cyls: dw 80
times 300-($-$$) db 0



kernel_args:
    db "-console cgatext",0





times 460-($-$$) db 0
gdt_ptr:
    dw gdt_table.end - gdt_table - 1
    dd gdt_table
gdt_table:
    dw 0, 0
    db 0, 0, 0, 0
    dw 0xffff, 0x0000
    db 0x00, 0x9a, 0xcf, 0x00
    dw 0xffff, 0x0000
    db 0x00, 0x92, 0xcf, 0x00
    .end:


times 510-($-$$) db 0
dw 0xaa55