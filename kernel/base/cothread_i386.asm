cpu 386
bits 32
section .text
global _cothread_init_regs
_cothread_init_regs:
    xor eax, eax
    mov [__cothread_curr_regs._esp_], eax
    mov [__cothread_curr_regs._ebp_], eax
    mov [__cothread_curr_regs._eax_], eax
    mov [__cothread_curr_regs._ebx_], eax
    mov [__cothread_curr_regs._ecx_], eax
    mov [__cothread_curr_regs._edx_], eax
    mov [__cothread_curr_regs._esi_], eax
    mov [__cothread_curr_regs._edi_], eax
    mov eax, cr3
    mov [__cothread_curr_regs._cr3_], eax
    pushf
    pop dword [__cothread_curr_regs._eflags_]
    ret

global _cothread_switch
_cothread_switch:
    mov [__cothread_curr_regs._esp_], esp
    mov [__cothread_curr_regs._ebp_], ebp
    mov [__cothread_curr_regs._eax_], eax
    mov [__cothread_curr_regs._ebx_], ebx
    mov [__cothread_curr_regs._ecx_], ecx
    mov [__cothread_curr_regs._edx_], edx
    mov [__cothread_curr_regs._esi_], esi
    mov [__cothread_curr_regs._edi_], edi
    mov eax, cr3
    mov [__cothread_curr_regs._cr3_], eax
    pushf
    pop dword [__cothread_curr_regs._eflags_]
    extern __cothread_do_switch
    call __cothread_do_switch
    mov eax, [__cothread_curr_regs._cr3_]
    mov cr3, eax
    mov esp, [__cothread_curr_regs._esp_]
    mov ebp, [__cothread_curr_regs._ebp_]
    mov eax, [__cothread_curr_regs._eax_]
    mov ebx, [__cothread_curr_regs._ebx_]
    mov ecx, [__cothread_curr_regs._ecx_]
    mov edx, [__cothread_curr_regs._edx_]
    mov esi, [__cothread_curr_regs._esi_]
    mov edi, [__cothread_curr_regs._edi_]
    push dword [__cothread_curr_regs._eflags_]
    popf
    ret

global _cothread_idle
_cothread_idle:
    hlt
    ret

section .bss
global __cothread_curr_regs
__cothread_curr_regs:
    ._cr3_:
    resd 1
    ._edi_:
    resd 1
    ._esi_:
    resd 1
    ._ebp_:
    resd 1
    ._esp_:
    resd 1
    ._ebx_:
    resd 1
    ._edx_:
    resd 1
    ._ecx_:
    resd 1
    ._eax_:
    resd 1
    ._eflags_:
    resd 1
    .end: