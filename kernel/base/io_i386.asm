global _io_write_uint8
_io_write_uint8:
    mov dx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret

global _io_write_uint16
_io_write_uint16:
    mov dx, [esp+4]
    mov ax, [esp+8]
    out dx, ax
    ret

global _io_write_uint32
_io_write_uint32:
    mov dx, [esp+4]
    mov eax, [esp+8]
    out dx, eax
    ret

global _io_read_uint8
_io_read_uint8:
    mov dx, [esp+4]
    xor eax, eax
    in al, dx
    ret

global _io_read_uint16
_io_read_uint16:
    mov dx, [esp+4]
    xor eax, eax
    in ax, dx
    ret

global _io_read_uint32
_io_read_uint32:
    mov dx, [esp+4]
    in eax, dx
    ret