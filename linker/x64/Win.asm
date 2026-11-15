;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

extern f.main
global _start

global f.stdout
global f.stderr
global f.stdin

; Windows x64 function ABI adapter
; IRVM: RSI, RDI, R8, R9, R10, R11, stack
; Windows: RCX, RDX, R8, R9, stack

%macro WIN_WRAPPER 3
global %1
extern %2

%1:
    push rbp
    mov rbp, rsp
    sub rsp, 0x60

    ; First four arguments
    mov rcx, rsi
    mov rdx, rdi
    ; mov r8, r8 | R8  = a2 (already correct)
    ; mov r9, r9 | R9  = a3 (already correct)

    ; Fifth and sixth arguments
%if %3 >= 5
    mov [rsp + 0x20], r10
%endif
%if %3 >= 6
    mov [rsp + 0x28], r11
%endif

    ; IRVM places stack arguments in reverse order after the return address.
%if %3 > 6
%assign __arg 7
%rep %3 - 6
    mov rax, [rbp + 16 + 8 * (%3 - __arg)]
    mov [rsp + 0x20 + 8 * (__arg - 5)], rax
%assign __arg __arg + 1
%endrep
%endif

    call %2

    ; Extend the 32-bit API result to the internal 64-bit return register.
    movsxd rbx, eax

    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .text

WIN_WRAPPER f.NtReadFile, NtReadFile, 9
WIN_WRAPPER f.NtWriteFile, NtWriteFile, 9
WIN_WRAPPER f.NtCreateFile, NtCreateFile, 11
WIN_WRAPPER f.NtClose, NtClose, 1
WIN_WRAPPER f.NtDeviceIoControlFile, NtDeviceIoControlFile, 10
WIN_WRAPPER f.NtAllocateVirtualMemory, NtAllocateVirtualMemory, 6
WIN_WRAPPER f.NtProtectVirtualMemory, NtProtectVirtualMemory, 5
WIN_WRAPPER f.NtFreeVirtualMemory, NtFreeVirtualMemory, 4
WIN_WRAPPER f.NtQueryInformationProcess, NtQueryInformationProcess, 5
WIN_WRAPPER f.NtTerminateProcess, NtTerminateProcess, 2
WIN_WRAPPER f.NtWaitForSingleObject, NtWaitForSingleObject, 3
WIN_WRAPPER f.NtDelayExecution, NtDelayExecution, 2
WIN_WRAPPER f.NtQueryPerformanceCounter, NtQueryPerformanceCounter, 2

f.stdout:
    mov rax, gs:[0x60]       ; PEB
    mov rax, [rax + 0x20]    ; ProcessParameters
    mov rbx, [rax + 0x28]    ; StandardOutput
    ret

f.stderr:
    mov rax, gs:[0x60]
    mov rax, [rax + 0x20]
    mov rbx, [rax + 0x30]
    ret

f.stdin:
    mov rax, gs:[0x60]
    mov rax, [rax + 0x20]
    mov rbx, [rax + 0x20]
    ret

_start:
	mov rbp,rsp
	call f.main
	mov	rsi,-1
	mov	rdi,rbx
	call f.NtTerminateProcess
    ud2